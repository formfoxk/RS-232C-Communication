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
#include <stdlib.h>

#include "eeprom.h"
#include "usart.h"
#include "timer.h"
#include "lcd.h"
#include "adc.h"

#define TRUE    1
#define FALSE   0

void cmd_ack();
void cmd_connect();
void cmd_send_led();
void cmd_change_led();
void cmd_voltage();
void cmd_time();


// packet Variable
//char packet[51] = {0};		// 0=STX, 1=SRC, 2=DES, 3,4=CMD, 5,6=SIZE  7~48= DATA,  49=CHK , 50=ETX
//struct Packet packet;

// Current Time Variable
char time1 = '0';
char time2 = '0';
char min1 = '0';
char min2 = '0';
char sec1 = '0';
char sec2 = '0';

// Led State Variable
volatile char led_arr[9] = "00000000";

//-----------------------Button board----------------------------------------//
char debounce(char last) {
	volatile char current = PIND;
	if (last != current) {
		_delay_ms(5);
		current = PIND;
	}
	return current;
}

unsigned char readButton(void)
{
	return ~(PIND);
}

//------------------------------- PactmpCmdet Init ------------------------------------//
void setData(struct Packet* p, char *data, int size)
{
	int i = 0;
	for (i = 0; i < size; i++)
	{
		p->mData[i] = data[i];
	}
}

int numlen(int num)
{
	int i = 1;
	int len = 0;
	for (i = 1; num / i > 0; i *= 10)
		len++;

	return len;
}

//----------------------- Time ----------------------------------------//
void time()
{
	char str[10] = "  :  :   ";
	long int temp;

	// 시
	time_count = time_count % (60 * 60 * 24);
	temp = time_count / (60 * 60);
	if(temp >= 10) {
		str[0] = (char)(temp/10 + '0');
	} else{
		str[0] = '0';
	}
	str[1] = (char)(temp%10 + '0');
	str[2] = ':';

	// 분
	time_count = time_count%(60*60);
	temp = time_count/(60);
	if(temp >= 10) {
		str[3] = (char)(temp/10 + '0');
	} else {
		str[3] = '0';
	}
	str[4] = (char)(temp%10 + '0');
	str[5] = ':';

	// 초
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
}

void readTimeFromEEPROM()
{
	// 초기 EEPROM에서 시간값 얻어오기
	char str[9] = "  :  :  ";
	time1 = str[0] = EEPROM_read(0);
	time2 = str[1] = EEPROM_read(1);
	min1 = str[3] = EEPROM_read(2);
	min2 = str[4] = EEPROM_read(3);
	sec1 = str[6] = EEPROM_read(4);
	sec2 = str[7] = EEPROM_read(5);
	
	time_count = 0;
	time_count += (int)(EEPROM_read(6) << 24);
	time_count += (int)(EEPROM_read(7) << 16);
	time_count += (int)(EEPROM_read(8) <<  8);
	time_count += (int)(EEPROM_read(9) <<  0);
	
	//time();
	
	// print time to LCD
	lcd_position(1, 3);
	lcd_string(str);
}

//----------------------- Connect ----------------------------------------//
volatile char connect = 0;
void connect_check()
{
	// 연결 여부를 3초마다 한번씩 체크한다
	// 패킷 초기화
	cmd_ack();
	//while (rx_complete == 0);
	
	if (recvPacket.mCmd > START_PROTOCOL && recvPacket.mCmd < END_PROTOCOL)
	{
		if(recvPacket.mCmd == RACK)
		{
			lcd_position(1, 1);
			lcd_char('C');
			
			// 초기 LED 정보 보내기
			cmd_send_led();
		}
	
		// 사용후 패킷 제거
		memset(&recvPacket, 0, sizeof(struct Packet));
		memset(packBuffer, 0, sizeof(packBuffer));
	}
	else if (recvPacket.mStx == 0 && recvPacket.mEtx == 0)
	{
		lcd_position(1, 1);
		lcd_char('X');
	}
}

//-------------------------- LCD ------------------------------//
void printAdcToLCD(int temp_adcValue)
{
	char str[6] = "V:  %";
	static int lastAdcValue = 0;
	
	if(temp_adcValue == 100)
		temp_adcValue = 99;
	
	// send ADC PactmpCmdet
	cmd_voltage(temp_adcValue);
	
	// print to LCD
	lcd_position(1, 12);
	str[0] = 'V';
	str[1] = ':';
	str[2] = ((temp_adcValue / 10)|0x30);
	temp_adcValue = temp_adcValue % 10;
	str[3] = ((temp_adcValue / 1)|0x30);
	str[4] = '%';
	lcd_string(str);
	
	lastAdcValue = temp_adcValue;
}

void printButtonToLCD()
{
	char str[13] = "            ";
	
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
	str[8] = 0;
	
	lcd_string(str);
}

void printStandby()
{
	lcd_position(2, 13);
	lcd_string("STOP");
}

void printRunby()
{
	lcd_position(2, 14);
	lcd_string("RUN");
}

//----------------------- ADC ----------------------------------------//
void initAdc()
{
	// REFS1/0(D7/6) = 0 1(AVref, Vcc 이용), ADLAR(D5) = 0(Align, Right),
	// MUX4/3/2/1/0(D4/3:Single Ended = 0 0, D2/1/0: Channel = 0
	ADMUX = (0 << REFS1) |
		(1 << REFS0) |
		(0 << ADLAR) |
		(0 << MUX4) |
		(0 << MUX3) |
		(0 << MUX2) |
		(0 << MUX1) |
		(0 << MUX0);
	
	ADCSRA= 0xad;// ADC enable
	// ADEN(D7, AD Enable) = 1, ADSC(D6, Start) = 0, ADFR(D5, Free Run) = 0(Single Conversion),
	// ADIF(D4, Int Flag), ADIE(D3, Int Enable) = 0, ADPS2/1/0(D2/1/0, Prescale) = 1 1 0(64분주,
	// 8MHz/64->125kHz(Range 50~200Khz), 13 AD Cycle당 1회 샘플링 - 약 9.6kHz
	// Single Conversion Mode : 매번 ADC할 경우 ADSC Start, ADIF가 1이면 값을 읽고,
	// ADIF에 1을 씀으로 ADIF를 Clear함
	// Free Running Mode : 한번 Start시키면 계속 Conversion 수행, 임의의 시점에서 값을 읽음
	ADCSRA = (1 << ADEN) |
		(0 << ADSC) |
		(1 << ADFR) |
		(0 << ADIF) |
		(1 << ADIE) |
		(1 << ADPS2) |
		(0 << ADPS1) |
		(1 << ADPS0);
	
	ADCSRA = ADCSRA | (1 << ADSC);			// AD Conversion Start
	ADCSRA = ADCSRA | (1 << ADIF);
}

void printAdc()
{
	initAdc();
	
	ADCSRA = ADCSRA | (1 << ADSC);         // AD Conversion Start
	_delay_ms(10);
		
	ADCSRA |= (0 << ADIF);
	
	unsigned int temp_adcValue = (Adc_Value/(float)(1023)) * 100;
	
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
	int i = 0;
	char state = readButton();
	static char last = 0;
	
	for (i = 0; i < 8; i++)
	{
		int shift = 1 << i;
		// 체크하려는 i 번째 버튼이 눌렸는지?
		if ((state & shift) == shift)
		{
			// 눌린 버튼이 한번 더 눌렸는지?
			if ((led_state & shift) != shift)
			{
				led_arr[i] = '0';
			}
			else
			{
				led_arr[i] = '1';
			}
			// 눌려진 버튼만 xor 연산
			led_state ^= shift;
		}
	}
	
	PORTB = led_state;
	
	// Led State Transit
	if (last != state)
		cmd_send_led();

	last = state;
	
	//button = state;
	printButtonToLCD();
}

//------------------------------ Cmd ------------------------------------//
// ACK
void cmd_ack()
{
	struct Packet p;
	
	p.mStx = 0x02;
	p.mSrc = 0;
	p.mDes = 0;
	p.mCmd = ACK;
	p.mChk = 0x04;
	p.mEtx = 0x03;
	
	// send packet
	char* strPacket = serialization(&p);
	usart0_string(strPacket, sizeof(p));
}

// Connection CheckCmd
void cmd_connect(){
	struct Packet p;
	
	p.mStx = 0x02;
	p.mSrc = 0;
	p.mDes = 0;
	p.mCmd = CONNECT;
	p.mChk = 0x04;
	p.mEtx = 0x03;
	
	// send packet
	char* strPacket = serialization(&p);
	usart0_string(strPacket, sizeof(p));
}

// Led State Transit
void cmd_send_led(){
	struct Packet p;
	
	p.mStx = 0x02;
	p.mSrc = 0;
	p.mDes = 0;
	p.mCmd = SEND_LED;
	p.mSize = sizeof(led_arr);
	setData(&p, led_arr, sizeof(led_arr));
	/*
	p.mData[0] = led_arr[0];
	p.mData[1] = led_arr[1];
	p.mData[2] = led_arr[2];
	p.mData[3] = led_arr[3];
	p.mData[4] = led_arr[4];
	p.mData[5] = led_arr[5];
	p.mData[6] = led_arr[6];
	p.mData[7] = led_arr[7];
	p.mData[8] = 0;
	*/
	p.mChk = 0x04;
	p.mEtx = 0x03;
	
	// send packet
	char* strPacket = serialization(&p);
	usart0_string(strPacket, sizeof(p));
}

// Led State Receive
void cmd_change_led()
{
	if (recvPacket.mCmd == CHANGE_LED)
	{
		int c = 0;
		int i = 0;
		for (i = 0; i < 8; i++)
		{
			if (recvPacket.mData[i] == '1')
			{
				c++;
			}
		}
		
		// 잘못된 데이터 제외
		if (c != 1) return;
		
		for (i = 0; i < 8; i++)
		{
			if (recvPacket.mData[i] == '1')
			{
				led_state ^= (1 << i);
				led_arr[i] = ((led_arr[i] == '1')? '0': '1');
				PORTB = led_state;
				
				// 사용후 패킷 제거
				memset(&recvPacket, 0, sizeof(struct Packet));
				memset(packBuffer, 0, sizeof(packBuffer));
				return;
			}
		}
	}
}

// Volumn Value Transit
void cmd_voltage(int temp_adcValue){
	struct Packet p;
	
	p.mStx = 0x02;
	p.mSrc = 0;
	p.mDes = 0;
	p.mCmd = VOLTAGE;
	p.mSize = 2;
	p.mData[0] = ((temp_adcValue / 10)|0x30);
	temp_adcValue = temp_adcValue % 10;
	p.mData[1] = ((temp_adcValue / 1)|0x30);
	p.mChk = 0x04;
	p.mEtx = 0x03;
	
	// send packet
	char* strPacket = serialization(&p);
	usart0_string(strPacket, sizeof(p));
}

// Time Transit
void cmd_time(){
	struct Packet p;
	char buff[7] = { time1, time2, min1, min2, sec1, sec2, 0 };
	
	p.mStx = 0x02;
	p.mSrc = 0;
	p.mDes = 0;
	p.mCmd = TIME;
	p.mSize = sizeof(buff);
	setData(&p, buff, sizeof(buff));
	/*
	p.mData[0] = buff[0];
	p.mData[1] = buff[1];
	p.mData[2] = buff[2];
	p.mData[3] = buff[3];
	p.mData[4] = buff[4];
	p.mData[5] = buff[5];
	p.mData[6] = 0;
	*/
	p.mChk = 0x04;
	p.mEtx = 0x03;
	
	// send packet
	char* strPacket = serialization(&p);
	usart0_string(strPacket, sizeof(p));
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

	// Standby Status
 	printStandby();
	// SW0 Check
	while(1)
	{
		char sw0 = readButton();
		if (sw0 & 0b00000001)
			break;
	}
	
	// X -> C
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

				break;
			}
		}
	}
	
	readTimeFromEEPROM();
	// Send Connection Packet
	//cmd_connect();
	
	while(1)
	{	
		if(timer0_count >= 625) // 1초 증가
		{
			time_count++;
			timer0_count = 0;
			time();
			cmd_time();
		}
		
		if (time_count % 3 == 0)
		{
			connect_check(&recvPacket);
		}
		
		if(timer_flag >= 2500) // 4초를 경과한 경우
		{
			// print time & adc
			time();
			timer_flag = 0;
			
			// write time to EEPROM
			EEPROM_write(0, time1);
			EEPROM_write(1, time2);
			EEPROM_write(2, min1);
			EEPROM_write(3, min2);
			EEPROM_write(4, sec1);
			EEPROM_write(5, sec2);
			EEPROM_write(6, (time_count >> 24) & 0xFF);
			EEPROM_write(7, (time_count >> 16) & 0xFF);
			EEPROM_write(8, (time_count >>  8) & 0xFF);
			EEPROM_write(9, (time_count >>  0) & 0xFF);
		}
		if(led_flag >= 1250)	// 3초를 경과한 경우
		{
			// print time & adc
			time();
		}
		
		// ADC 출력
		printAdc();
		
		// Button 입력
		Led_Control();
		
		// 실행중 상태 : RUN 출력
		printRunby();

		// ADC 화면 출력
		printAdcToLCD(adcValue);
		
		cmd_change_led();
	}
}