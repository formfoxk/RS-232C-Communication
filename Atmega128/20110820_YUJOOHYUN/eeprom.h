/*
* EEPROM.c
*
* Created: 2015-06-25 오후 10:22:00
*  Author: YuJoo
*/

#ifndef EEPROM_H_
#define EEPROM_H_

#include <avr/io.h>

//----------------------- EEPROM ----------------------------------------//

void EEPROM_write(unsigned int uiAddress, unsigned char ucData) {

	while (EECR & (1<<EEWE));     // 앞의 기록이 완료될 때가지 기다린다.
	EEAR = uiAddress;             // 기록할 주소를 EEAR에 지정한다.
	EEDR = ucData;                // 데이터를 EEDR에 기록한다.
	
	cli();
	EECR |=(1<<EEMWE);            // EEMWE에 1을 기록한다.
	EECR |=(1<<EEWE);             // EEWE를 세트 함으로써 EEPROM 기록을 시작.
	sei();

}

unsigned char EEPROM_read(unsigned int uiAddress) {

	while (EECR & (1<<EEWE));     // 이전의 Write가 완료될 때가지 기다린다.
	EEAR = uiAddress;             // 읽어 들일 주소를 지정한다.
	EECR |= (1<<EERE);            // EERE에 1을 기록함으로써 판독 시작한다.
	return EEDR;                  // EEDR의 데이터를 리턴 한다.
}

#endif