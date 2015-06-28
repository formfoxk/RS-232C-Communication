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

ISR(ADC_vect)
{
	Adc_Value = (int)ADCL;
	Adc_Value = Adc_Value + ((int)ADCH << 8);
	/*
	ADCSRA = (1 << ADEN) | (0 << ADSC) | (0 << ADFR) | (0 << ADIF) | (0 << ADIE) | (1 << ADPS2)
	| (1 << ADPS1) | (0 << ADPS0);

	ADCSRA = ADCSRA | (1 << ADSC);			// AD Conversion Start
	while((ADCSRA & 0x10) == 0);			// Wait till ADIF=1(AD Complete & Data Reg. Updated)
	Adc_Value = (int)ADCL;		// Read시 하위 먼저
	Adc_Value = Adc_Value + ((int)ADCH << 8);	// A/D 변환값 읽기

	ADCSRA = ADCSRA | (1 << ADIF);			// ADIF를 Clear, ADIF is Cleared by Writing One to Flag.
	*/
}

#endif /* ADC_H_ */