/*
 * timer.h
 *
 * Created: 2015-06-25 오후 10:29:05
 *  Author: YuJoo
 */ 


#ifndef TIMER_H_
#define TIMER_H_

volatile unsigned int timer0_count = 0;
volatile unsigned int timer_flag = 0;
volatile unsigned int time_count = 0;

volatile unsigned int led_flag = 0;
volatile unsigned int connect_flag = 0; // 연결 Check

ISR(TIMER0_OVF_vect){     // 인터럽트 서비스 루틴
	timer0_count++;
	if(timer_flag != 0) timer_flag++;
	if(led_flag != 0) led_flag++;
	if(connect_flag != 0) connect_flag ++;
	TCNT0 = 206;
}

void timer0_OFV_init()
{
	TCCR0 = 0x06;   // 256분주
	TCNT0 = 206; // 256 - 206  = ( 32*50 = 1600us)
	TIMSK |= (1<<TOIE0); // Interrupt Enable
	// Global Interrupt Enable
	sei();
}

#endif /* TIMER_H_ */