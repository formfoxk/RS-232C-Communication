/*
 * usart.h
 *
 * Created: 2015-06-25 오후 10:29:18
 *  Author: YuJoo
 */ 


#ifndef USART_H_
#define USART_H_

// UART통신
volatile unsigned char rx_complete = 0;               // 인터럽트를 이용한 USART0 수신에서 수신 완료 플레그
volatile unsigned char rx0_data;
volatile unsigned int rx_flag=1;
volatile unsigned int rx_cnt=0;
volatile char arr[25] ={0};


ISR(USART0_RX_vect) {
	// Old Signal Name = SIG_UART0_RECV

	rx0_data = UDR0;
	

	if(rx0_data==0x03) // 마지막 데이터
	{
		arr[rx_cnt]=rx0_data;
		rx_complete = 1;
		rx_cnt=0;
		
	}
	else
	{
		arr[rx_cnt]=rx0_data;
		rx_cnt++;
	}
	
}

void usart0_init(void) {
	
	// USART0 초기화

	SREG = 0x00;                              // Serial Interrupt를 설정하기 전에 전체 인터럽트를 Disable

	// 정상모드, U2X0 = 0(배속을 Disable)
	UCSR0A = 0;

	// ===> 인터럽트 방식
	// ===> 수신완료 인터럽트 인에이블 : RXCIE0(DB7) = 1
	// 송수신 인에이블 : RXEN0 = TXEN0 = 1
	UCSR0B = 0x98;

	// No Parity, 1비트 Stop Bit, 비동기, 데이터 8비트
	// UMSEL0(동기/비동기) = 0, No Parity = UPM01 = UPM00 = 0, Stop = USBS0 = 0(1비트),
	// UCSZ012,01,00 = 0, 1, (8-bit)
	UCSR0C = 0x06;
	//UCSR0C = (0 << UMSEL0) | (0 << UPM01) | (0 << UPM00) | (0 << USBS0) | (1 << UCSZ01) | (1 << UCSZ00);   // With UCSZ02 = 0

	// 통신 속도 설정 : 보레이트 9600
	UBRR0H = 0;
	UBRR0L = 51;

	SREG = 0x80;                              // 전체 인터럽트를 Enable
}
unsigned char usart0_recv(void) {
	// Serial Port 0를 통한 1 문자 수신

	// 데이터 수신 여부 검사 - 수신 버퍼가 찰때까지 기다림
	while (!(UCSR0A & 0x80)) // RXC0 == 1
	//while (!(UCSR0A & (1 << RXC0)));
	;
	return UDR0;
}


void usart0_send(unsigned char s_data) {
	// Serial Port 0를 통한 1 문자 송신

	// 전송 가능 여부 검사 - 전송 버퍼가 빌때까지 기다림
	while (!(UCSR0A & 0x20)) // UDRE0 == 1 ?
	//while (!(UCSR0A & (1 << UDRE0)));
	;
	if(65<=s_data && s_data<=90)
	UDR0 = s_data+32;
	else if(97<=s_data && s_data <= 122)
	UDR0 = s_data-32;
	else
	UDR0 = s_data;


}

void usart0_string(char *str) {
	// Serial Port 0를 통한 문자열 전송

	for(int i=0; i<25; i++)
	{
		usart0_send(*str);
		str++;
	}
}

#endif /* USART_H_ */