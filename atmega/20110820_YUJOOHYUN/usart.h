/*
 * usart.h
 *
 * Created: 2015-06-25 오후 10:29:18
 *  Author: YuJoo
 */ 


#ifndef USART_H_
#define USART_H_

struct Packet* unserialization(char* buff);
char* serialization(struct Packet* pack);

// UART통신
volatile unsigned char rx_complete = 0;               // 인터럽트를 이용한 USART0 수신에서 수신 완료 플레그
volatile unsigned char rx0_data;
volatile unsigned int rx_flag = 1;
volatile unsigned int rx_cnt = 0;

enum PacketCommand {
	// 프로토콜 시작 값
	START_PROTOCOL = 0x0000,
	// 프로토콜 정의
	ACK, RACK, CONNECT, CHANGE_LED, SEND_LED, VOLTAGE, TIME,
	// 프로토콜 패킷 타입의 끝
	END_PROTOCOL
};

#define PACKED  __attribute__((packed))
struct Packet {
	char mStx;
	char mSrc;
	char mDes;
	short mCmd;
	short mSize;
	char mData[42];
	char mChk;
	char mEtx;
} PACKED;

char packBuffer[sizeof(struct Packet) + 1];
struct Packet recvPacket = {
	.mStx = 0,
	.mSrc = 0,
	.mDes = 0,
	.mCmd = 0,
	.mSize = 0,
	.mData = {0, },
	.mChk = 0,
	.mEtx = 0,
};
volatile unsigned char led_state = 0xff;				// current led state variable

ISR(USART0_RX_vect)
{
	// Old Signal Name = SIG_UART0_RECV
	rx0_data = UDR0;
	packBuffer[rx_cnt++] = rx0_data;
	
	if(rx_cnt >= sizeof(struct Packet)) // 마지막 데이터
	{
		recvPacket = *(unserialization(packBuffer));
		if (recvPacket.mStx != 0x02 || recvPacket.mEtx != 0x03)
		{
			// 손실되거나 잘못된 패킷으로 간주한다.
			memset(&recvPacket, 0, sizeof(struct Packet));
			memset(packBuffer, 0, sizeof(packBuffer));
		}
		rx_cnt = 0;
		rx_complete = 1;
	}
	else if(rx_cnt == 0) // 처음 데이터
	{
		rx_complete = 0;
	}
}

char* serialization(struct Packet* pk)
{
	return (char*)pk;
}

struct Packet* unserialization(char* buff)
{
	return (struct Packet*)buff;
}

void usart0_init(void)
{
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
	//UCSR0C = (0 << UMSEL0) | (0 << UPM01) | (0 << UPM00) | (0 << USBS0) | (1 << UCSZ01) | (1 << UCSZ00);
	//With UCSZ02 = 0

	// 통신 속도 설정 : 보레이트 9600
	UBRR0H = 0;
	UBRR0L = 51;

	SREG = 0x80;                              // 전체 인터럽트를 Enable
}

void usart0_send(unsigned char s_data) {
	// Serial Port 0를 통한 1 문자 송신

	// 전송 가능 여부 검사 - 전송 버퍼가 빌때까지 기다림
	while ((UCSR0A & 0x20) == 0); // UDRE0 == 1 ?
	//while (!(UCSR0A & (1 << UDRE0)));
	
	UDR0 = s_data;
}

void usart0_string(char *str, int size)
{
	// Serial Port 0를 통한 문자열 전송

	for(int i=0; i < size; i++)
	{
		usart0_send(*str);
		str++;
	}
}

#endif /* USART_H_ */