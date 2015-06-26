
// Emulator-MFCDlg.h : 헤더 파일
//

#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include "Comm.h"

#pragma pack(1)
typedef struct
{
	char stx;
	char src;
	char des;
	char cmd[2];
	char size[2];
	char data[42];
	char checkSum;
	char etx;

}PACKET;
#pragma pack()

// CEmulatorMFCDlg 대화 상자
class CEmulatorMFCDlg : public CDialogEx
{
// 생성입니다.
public:
	CEmulatorMFCDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_EMULATORMFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
	LPWSTR AsciiToUnicode(LPCSTR lpData,LPWSTR lpwszReturn);
	LPSTR UnicodeToAscii(LPWSTR lpData, LPSTR lpReturndata);

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// Member Variable
	Comm *con;			// Atmega128 Connection Administer
	int con_flag;		// Connection State
	PACKET m_PACKET;	// Packet 구조체 생성
	boolean led_state[8];	// LED State List
	CString receive_str;// Atmega128의 패킷 문자열을 저장하는 문자열

	// Member Function
	void onOffControl(boolean flag);	// GUI 창의 IDC_CONNECTION_ON/OFF 변경
	void connectionCheck();				// CMD 3  Data[8] send
	void adcCheck();					
	void ledStateSend();				
	void ledStateRequest();				// Led상태 정보 요청					
	void adcConvertLed();				// 버튼의 상태를 담은 정수값을 2진수로 변환하여 GUI에 출력
	void parsingMsg();					// 메시지를 패킷 구조체에 저장
	// 01 : ACK
	// 10 : Connection Check
	// 20 ; Led State Transit
	// 21 : Led State Receive
	// 30 : Volumn Value Transit
	// 40 : Time Transit
	void cmdProcess();					// 패킷을 통해 얻은 cmd를 처리


	// GUI
	CStatic m_logo;				
	CComboBox m_comboBox;
	CStatic m_picture;
	CStatic m_onoff;
	CStatic m_time;
	CProgressCtrl m_progress;
	CListBox m_stateList;

	// 
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedConn();
	afx_msg void OnBnClickedDisconn();

	LRESULT	OnThreadClosed(WPARAM length, LPARAM lpara);
	LRESULT	OnReceive(WPARAM length, LPARAM lpara);
};
