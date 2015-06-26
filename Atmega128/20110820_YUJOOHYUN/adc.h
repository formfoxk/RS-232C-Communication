/*
 * adc.h
 *
 * Created: 2015-06-25 오후 10:28:53
 *  Author: YuJoo
 */ 


#ifndef ADC_H_
#define ADC_H_

volatile int Adc_Value;      // ADC 변환값(ADC Data Reg. Read한 값)
volatile int adcValue;          // ADC 변환값 임시저장변수

ISR(ADC_vect)  {      // AD Conversion Interrupt Service Routine
	//   SREG = 0x00;         // Global Interrupt Disable
	Adc_Value = (int)ADCL;         // Read시 하위 먼저
	Adc_Value = Adc_Value + ((int)ADCH << 8);   // A/D 변환값 읽기

	//   SREG = 0x80;         // Global Interrupt Enable
}

#endif /* ADC_H_ */