/*
* lcd.h
*
* Created: 2015-06-25 오후 10:28:35
*  Author: YuJoo
*/


#ifndef LCD_H_
#define LCD_H_

#define lcd_data *((volatile unsigned char *) 0xC000)   // PC7 : Enable, RS = 1 : Data
#define lcd_inst *((volatile unsigned char *) 0x8000)   // PC7 : Enable, RS = 0 : Instruction,


void led_init(void) {
	DDRB    = 0xFF;           // Set LED PORTB as Output
	PORTB   = 0xFF;           // Give PORTB and the User LEDs an Initial Start Value, All LEDs ara Off
}

void check_busy(void) {
	// check busy
	_delay_ms(5);
	while( ((lcd_inst) & 0x80) ) ;
}

void lcd_cmd(unsigned char cmd) {
	check_busy();
	lcd_inst = cmd;
}

void lcd_char(unsigned char ch) {
	check_busy();
	lcd_data = ch;
}

void lcd_position(unsigned char y, unsigned char x) {
	// LCD에 Display할 위치 지정(이동). DD Ram Address만 설정,
	// 첫째줄(y = 1), 둘째줄(y = 2), 첫째칸(x = 1), 마지막 칸(x = 16)
	unsigned char Position;
	
	if (x > 16) x = 16;
	if (y > 2) y = 2;
	Position = 0x40 * (y - 1) + (x - 1);      // DD Ram Address 계산
	//LCD_Busy();
	lcd_cmd(Position | 0x80);            // Set DD Ram Address 명령
}

void lcd_string(char *str) {
	while(*str) {
		lcd_char(*str);
		str++;
	}
}

void lcd_init(void) {
	
	// MCUCR(MCU Control Register)   D(7) : SRE: External SRAM/XMEM Enable,
	MCUCR = (1 << SRE);                  // or MCUCR = 0xC0;

	// Display Clear
	lcd_cmd(0x01);
	// LCD Function Set
	lcd_cmd(0x38);                     // 8Bit, 2 Line, Small Font */
	// Entry Mode Set
	lcd_cmd(0x06);                     // Increment & Display Freeze */
	// Display On/Off Control
	lcd_cmd(0x10);                     // Cursor Move, Not Shift
	// Display On, Cursor Off, Blink Off
	lcd_cmd(0x0C);


	_delay_ms(10);
}

	
void Set_font1(void)      // 한글 폰트 출력 함수
{
	unsigned char font[] = {
		0x1d,0x05,0x1d,0x0b,0x11,0x0e,0x0a,0x0e,
		0x04,0x0a,0x11,0x00,0x04,0x04,0x04,0x1f,
		0x1f,0x01,0x05,0x04,0x1f,0x04,0x0a,0x04,
		0x00,0x00,0x0a,0x15,0x11,0x0a,0x04,0x00,
		0x12,0x12,0x1e,0x12,0x1e,0x00,0x0f,0x01,
		0x10,0x10,0x18,0x10,0x10,0x00,0x00,0x00,
		0x01,0x09,0x1d,0x09,0x15,0x15,0x01,0x00,
		0x01,0x1d,0x15,0x15,0x1d,0x01,0x08,0x0f,
	};
	
	lcd_cmd(0x40);
	for(int i=0;i<64;i++)
		lcd_char(font[i]);
}
	

#endif /* LCD_H_ */