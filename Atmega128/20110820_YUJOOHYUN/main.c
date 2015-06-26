/*
* _20110820_YUJOOHYUN.c
*
* Created: 2015-06-25 오후 10:20:21
*  Author: YuJoo
*/

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "eeprom.h"
#include "usart.h"
#include "timer.h"
#include "lcd.h"
#include "adc.h"

void cmd_01();
void cmd_10();
void cmd_20();
void cmd_21(int);
void cmd_30(int);
void cmd_40();

unsigned int isCheckCmd = 0;				// isChectmpCmdExist Time in EEPROM	
volatile unsigned char led_state = 0xff;				// current led state variable
volatile unsigned int button = 0;

// packet Variable
char packet[51] = {0};		// 0=STX, 1=SRC, 2=DES, 3,4=CMD, 5,6=SIZE  7~48= DATA,  49=CHK , 50=ETX
int cmd;					// command variable
char id=0;					// Board ID Variable
char data[42] = {0};		// data variable

// Current Time Variable
char time1='0';
char time2='0';
char min1='0';
char min2='0';
char sec1='0';
char sec2='0';

// Led State Variable
volatile char led_arr[8] = {'0','0','0','0','0','0','0','0'};

//-----------------------Button board----------------------------------------//

unsigned char readButton(void)
{
	unsigned char temp=0;

	if     (PIND==0b11111110) { temp = 1;  }
	else if(PIND==0b11111101) { temp = 2;  }
	else if(PIND==0b11111011) { temp = 3;  }
	else if(PIND==0b11110111) { temp = 4;  }
	else if(PIND==0b11101111) { temp = 5;  }
	else if(PIND==0b11011111) { temp = 6;  }
	else if(PIND==0b10111111) { temp = 7;  }
	else if(PIND==0b01111111) { temp = 8;  }

	return temp;  // adcValue is sum of row and column
}
	
//------------------------------ ID ChectmpCmd ----------------------------------------//
// EEPROM에 ID가 있는지 체크

unsigned int isCheckCmdTime()
{
	// EEPROM에 Time이 저장 되있는 경우
	if(!(EEPROM_read(0) >= 0 && EEPROM_read(0) < 3)) 
		return 0;
	
	return 1;
}

//------------------------------- PactmpCmdet Init ------------------------------------//
void setData(char *data)
{
	for(int i = 7, j = 0; i < 49; i++, j++)
	{
		packet[i] = data[j];
	}
}

void setCommand(char* cmd)
{
	packet[3] = cmd[0];
	packet[4] = cmd[1];
}

void initAckPacket()
{
	// 0=STX, 1=SRC, 2=DES, 3,4=CMD, 5,6=SIZE  7~48= DATA,  49=CHK , 50=ETX

	packet[0] = 0x02;
	packet[1] =  0;
	packet[2] = '0';
	
	packet[5] = '0';
	packet[6] = '1';
	packet[7] =  0;
	
	packet[49] = 0x04;
	packet[50] = 0x03;
}

void initConnectionPacket()
{
	// 0=STX, 1=SRC, 2=DES, 3,4=CMD, 5,6=SIZE  7~48= DATA,  49=CHK , 50=ETX
	
	packet[0] = 0x02;
	packet[1] =  0;
	packet[2] = '0';
	packet[5] = '0';
	packet[6] = '2';
	packet[7] = '0';
	packet[8] = '0';
	packet[49] = 0x04;
	packet[50] = 0x03;
}

int numlen(int num)
{
	int len = 1;
	while (num/=10)

	len++;

	return len;
}

void initAdcPacket(int a_adcValue)           // ADC값
{
	// 0=STX, 1=SRC, 2=DES, 3,4=CMD, 5,6=SIZE  7~48= DATA,  49=CHK , 50=ETX
	
	int num = numlen(a_adcValue);
	
	packet[0] = 0x02;
	packet[1] = id;
	packet[2] = '0';
	
	packet[5] = '0';
	packet[6] = (char)num|0x30;
	
	if(a_adcValue==1)
	{
		packet[7] = '0';
		packet[8] = '0';
	}
	else
	{
		
		packet[7] = (char)(a_adcValue/10|0x30);
		packet[8] = (char)(a_adcValue%10|0x30);
		
	}

	packet[49] = 0x04;
	packet[50] = 0x03;
	
}

void initLedPacket()
{
	// 0=STX, 1=SRC, 2=DES, 3,4=CMD, 5,6=SIZE  7~48= DATA,  49=CHK , 50=ETX
	
	packet[0] = 0x02;
	packet[1] = id;
	packet[2] = '0';
	
	packet[5] = '0';
	packet[6] = '8';
	packet[7] = led_arr[0];
	packet[8] = led_arr[1];
	packet[9] = led_arr[2];
	packet[10] = led_arr[3];
	packet[11] = led_arr[4];
	packet[12] = led_arr[5];
	packet[13] = led_arr[6];
	packet[14] = led_arr[7];
	
	packet[49] = 0x04;
	packet[50] = 0x03;
}

void initTimePacket()
{
	// 0=STX, 1=SRC, 2=DES, 3,4=CMD, 5,6=SIZE  7~48= DATA,  49=CHK , 50=ETX
	
	packet[0] = 0x02;
	packet[1] = id;
	packet[2] = '0';
	
	packet[5] = '0';
	packet[6] = '6';
	packet[7] =  time1;
	packet[8] =  time2;
	packet[9] =  min1;
	packet[11] = min2;
	packet[12] = sec1;
	packet[10] = sec2;
	
	packet[49] = 0x04;
	packet[50] = 0x03;
	
}

//----------------------- Time ----------------------------------------//
void time()
{
	char str[9] = "  :  :   ";
	long int temp;

	time_count = time_count % (60*60*24);
	temp = time_count / (60*60);
	if(temp >= 10) {
		str[0] = (char)(temp/10 + '0');
	} else{
		str[0] = '0';
	}
	str[1] = (char)(temp%10 + '0');
	str[2] = ':';

	time_count = time_count%(60*60);
	temp = time_count/(60);
	if(temp >= 10) {
		str[3] = (char)(temp/10 + '0');
	} else {
		str[3] = '0';
	}
	str[4] = (char)(temp%10 + '0');
	str[5] = ':';

	time_count = time_count%(60);
	temp = time_count;
	if(temp >= 10) {
		str[6] =  (char)(temp/10 + '0');
	} else {
		str[6] = '0';
	}
	str[7] = (char)(temp%10 + '0');
	str[8] = ' ';
	
	// time char settintmpLen(use packet format)
	time1 = str[0];
	time2 = str[1];
	min1 = str[3];
	min2 = str[4];
	sec1 = str[6];
	sec2 = str[7];
	
	// print str to lcd
	lcd_position(1, 3);
	lcd_string(str);
	
	cmd_40();
}

//----------------------- Connect ----------------------------------------//

void connect_check()
{
	if(connect_flag >= 1875 || connect_flag == 0)
	{
		lcd_position(1,1);
		lcd_char('X');
		
		connect_flag = 0;
		
	}
	else if(connect_flag  > 0 && connect_flag < 1875)
	{
		lcd_position(1,1);
		lcd_char('C');
		
		connect_flag = 1;
	}
}

//-------------------------- LCD ------------------------------//
void printAdcToLCD(int temp_adcValue)
{
	char str[5] = "V:  %";
	
	if(temp_adcValue == 100)
	temp_adcValue = 99;
	
	// send ADC PactmpCmdet
	cmd_30(temp_adcValue);
	
	// print to LCD
	lcd_position(1, 12);
	str[0] = 'V';
	str[1] = ':';
	str[2] = ((temp_adcValue / 10)|0x30);
	temp_adcValue = temp_adcValue % 10;
	str[3] = ((temp_adcValue / 1)|0x30);
	str[4] = '%';
	lcd_string(str);
}

void printButtonToLCD(){
	char str[12] = "            ";
	
	// print to LCD
	lcd_position(2, 1);
	str[0] = led_arr[0];
	str[1] = led_arr[1];
	str[2] = led_arr[2];
	str[3] = led_arr[3];
	str[4] = led_arr[4];
	str[5] = led_arr[5];
	str[6] = led_arr[6];
	str[7] = led_arr[7];
	str[8] = ' ';
	str[9] = ' ';
	str[10] = ' ';
	str[11] = ' ';
	
	lcd_string(str);
}

//----------------------- ADC ----------------------------------------//
void initAdc()
{
	ADMUX = 0x40;
	ADCSRA= 0xad;// ADC enable // auto tritmpLentmpLener // 분주비 //
}

void printAdc()
{
	initAdc();
	
	ADCSRA = ADCSRA | (1 << ADSC);         // AD Conversion Start
	_delay_ms(10);
		
	ADCSRA |= (0 << ADIF);
	
	unsigned int temp_adcValue = (Adc_Value/(float)(1023))*100;
	
	if(adcValue == temp_adcValue) // ADC값 변화가 없으면 계속 리턴
	{
		return;
	}
	adcValue = temp_adcValue;
	
	led_flag = 1;
	connect_flag = 1;

	printAdcToLCD(adcValue);
	
	ADCSRA |= (1 << ADIF);

}

//----------------------- LED ----------------------------------------//
void Led_Control()
{
	char state;
	state = readButton();
	
	if((state>=1)&&(state<=8))
	{
		if(button == state) return;
		
		if(state==1)
		{
			if(led_state == ( led_state & 0xfe)) { led_state |= 0x01; led_arr[0] = '0'; }
			else { led_state &= 0xfe; led_arr[0] = '1'; } // 1111 1110
		}
		else if(state==2)
		{
			if(led_state == ( led_state & 0xfd)) { led_state |= 0x02; led_arr[1] = '0'; }
			else { led_state &= 0xfd; led_arr[1] = '1'; } // 1111 1101
		}
		else if(state==3)
		{
			if(led_state == ( led_state & 0xfb)) { led_state |= 0x04; led_arr[2] = '0'; }
			else { led_state &= 0xfb; led_arr[2] = '1'; } // 1111 1011
		}
		else if(state==4)
		{
			if(led_state == ( led_state & 0xf7)) { led_state |= 0x08; led_arr[3] = '0'; }
			else { led_state &= 0xf7; led_arr[3] = '1'; } // 1111 0111
		}
		else if(state==5)
		{
			if(led_state == ( led_state & 0xef)) { led_state |= 0x10; led_arr[4] = '0'; }
			else { led_state &= 0xef; led_arr[4] = '1'; } // 1110 1111
		}
		else if(state==6)
		{
			if(led_state == ( led_state & 0xdf)) { led_state |= 0x20; led_arr[5] = '0'; }
			else { led_state &= 0xdf; led_arr[5] = '1'; } // 1101 1111
		}
		else if(state==7)
		{
			if(led_state == ( led_state & 0xbf)) { led_state |= 0x40; led_arr[6] = '0'; }
			else { led_state &= 0xbf; led_arr[6] = '1'; } // 1011 1111
		}
		else if(state==8)
		{
			if(led_state == ( led_state & 0x7f)) { led_state |= 0x80; led_arr[7] = '0'; }
			else { led_state &= 0x7f; led_arr[7] = '1'; } // 0111 1111
		}
		
		PORTB = led_state;
		
		// Led State Transit
		cmd_20();
		
		connect_flag=1;
		
	}
	button = state;
	printButtonToLCD();
}

//------------------------------ Cmd ------------------------------------//
// ACK
void cmd_01(){
	// init packet
	setCommand("01");
	initAckPacket();
	
	// send packet
	usart0_string(packet);
	
	for(int i=0; i < 51; i++)
	arr[i] = 0;
}

// Connection CheckCmd
void cmd_10(){
	// init packet
	initConnectionPacket();
	setCommand("10");
	
	// send packet
	setData("00");
	usart0_string(packet);
	
	for(int i=0; i < 51; i++)
	arr[i] = 0;
}

// Led State Transit
void cmd_20(){
	// init packet
	initLedPacket();
	setCommand("20");
	
	// send packet
	usart0_string(packet);
	
	for(int i=0; i < 51; i++)
	arr[i] = 0;
}

// Led State Receive
void cmd_21(int tmpLen){
	for(int i=0,j=7; i<tmpLen; i++,j++)
	{
		led_arr[i] = arr[j];
		
	}
	if(led_arr[0] == '1') led_state &= 0xfe; // 1111 1110
	else led_state |= 0x01;
	
	if(led_arr[1]=='1')   led_state &= 0xfd; // 1111 1101
	else led_state |= 0x02;
	
	if(led_arr[2]=='1') led_state &= 0xfb;	// 1111 1011
	else led_state |= 0x04;
	
	if(led_arr[3]=='1') led_state &= 0xf7;  // 1111 0111
	else led_state |= 0x08;
	
	if(led_arr[4]=='1') led_state &= 0xef;
	else led_state |= 0x10;
	
	if(led_arr[5]=='1') led_state &= 0xdf;
	else led_state |= 0x20;
	
	if(led_arr[6]=='1') led_state &= 0xbf;
	else led_state |= 0x40;
	
	if(led_arr[7]=='1') led_state &= 0x7f;
	else led_state |= 0x80;
	
	PORTB = led_state;
	Led_Control();
}

// Volumn Value Transit
void cmd_30(int temp_adcValue){
	// init packet
	setCommand("30");
	initAdcPacket(temp_adcValue);
	
	// send packet
	usart0_string(packet);
	
	for(int i=0; i < 51; i++)
	arr[i] = 0;
}

// Time Transit
void cmd_40(){
	// init packet
	initTimePacket();
	setCommand("40");
	
	// send packet
	usart0_string(packet);
	
	for(int i=0; i < 51; i++)
	arr[i] = 0;
}

int main(void)
{
	int button_setting=0;
	int temp_count=0;
	
	DDRD = 0x00; 
	DDRB = 0xff; 
	
	led_init();
	lcd_init();
	timer0_OFV_init();

	// print initial LCD
	lcd_position(1, 5);
	lcd_string("20110820");
	lcd_position(2, 4);
	lcd_string("YU JOOHYUN");
	
	while(1)
	{
		if(timer0_count >= 625) // 1초
		{
			temp_count++; // 3초를 카운트
			time_count++; // 프로그램 가동시간 체크
			timer0_count = 0;
			
			// 3초 간격으로 연결 상태 확인
			if(temp_count == 3) // 625(1초) * 3
			{
				temp_count = 0;	
				lcd_init();			// Init Lcd
				usart0_init();		// init usart
				time();				// Print Lcd & Send usart
				cmd_10();			// Connection ChectmpCmd(10)

				break;
			}
		}
	}
	
	while(1)
	{
		if(isCheckCmd == 0) // Not Exist Time in EEPROM
		{
			isCheckCmd = isCheckCmdTime(); // isChectmpCmdExist Time in EEPROM	

			// Exist Time in EEPROM 
			if(isCheckCmd == 1)  
			{
				char str[8] = "  :  :  ";
				time1 = str[0] = EEPROM_read(0);
				time2 = str[1] = EEPROM_read(1);
				min1 = str[3] = EEPROM_read(2);
				min2 = str[4] = EEPROM_read(3);
				sec1 = str[6] = EEPROM_read(4);
				sec2 = str[7] = EEPROM_read(5);
				
				// print time to LCD
				lcd_position(1, 3);
				lcd_string(str);
				
				// Send Connection PactmpCmdet
				setCommand("01");
				initAckPacket(id);
				usart0_string(packet);
			}
			else
			{
				// write time to EEPROM
				EEPROM_write(0,time1);
				EEPROM_write(1,time2);
				EEPROM_write(2,min1);
				EEPROM_write(3,min2);
				EEPROM_write(4,sec1);
				EEPROM_write(5,sec2);
			}
		}
		
		if(timer0_count >= 625) // 1초 증가
		{
			time_count++;
			timer0_count = 0;
			time();
		}
		
		if(timer_flag >= 2500) // 4초를 경과한 경우
		{
			// print time & adc
			time();
			timer_flag = 0;
		}
		if(led_flag >= 1250)	// 3초를 경과한 경우
		{
			// print time & adc
			time();
			PORTB = led_state;
		
			cmd_20();
			
			connect_flag = 1;
			led_flag = 0;
		}
		
		// 연결이 되고, 3초가 넘었는지 확인하기 위한 함수
		connect_check();
		
		// ADC 출력
		printAdc();
		
		// Button 입력
		Led_Control();

		if(rx_complete==1)
		{
			printAdcToLCD(adcValue);
			connect_flag = 1;
			int tmpCmd = arr[3] - '0';
			int tmpLen= arr[5] - '0';
			
			// decoding cmd
			tmpCmd *= 10;
			tmpCmd += arr[4] - '0';
			
			// decoding size
			tmpLen *= 10;
			tmpLen += arr[6] - '0';
			
			// Send Ack
			if(tmpCmd == 01)
			{
				cmd_01();
				
				rx_complete = 0;
				
			}
			
			// Connection Check
			if(tmpCmd == 10)
			{
				cmd_10();
				
				connect_flag = 1;
				rx_complete=0;
				
			}
			
			// Led State Transit
			if( tmpCmd == 20)
			{
				cmd_20();			
   
				connect_flag = 1;
				rx_complete=0;
			}
			
			if( tmpCmd == 21)
			{
				cmd_21(tmpLen);
				
				connect_flag = 1;
				rx_complete=0;	
			}
		}
		printAdcToLCD(adcValue);
	}
}