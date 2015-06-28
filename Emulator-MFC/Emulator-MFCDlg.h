
// Emulator-MFCDlg.h : ��� ����
//

#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include "afxbutton.h"
#include "Comm.h"

#define NULL    0
#define TRUE    1
#define FALSE   0

enum PacketCommand { 
	START_PROTOCOL = 0x0000,
	ACK, RACK, CONNECT, CHANGE_LED, SEND_LED, VOLTAGE, TIME,
	END_PROTOCOL,
};

#pragma pack(1)
struct Packet {
   char mStx;
   char mSrc;
   char mDes;
   short mCmd;
   short mSize;
   char mData[42];
   char mChk;
   char mEtx;
};
#pragma pack()

struct Packet* unserialization(char* buff);
char* serialization(struct Packet* pack);

// CEmulatorMFCDlg ��ȭ ����
class CEmulatorMFCDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CEmulatorMFCDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_EMULATORMFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.
	LPWSTR AsciiToUnicode(LPCSTR lpData,LPWSTR lpwszReturn);
	LPSTR UnicodeToAscii(LPWSTR lpData, LPSTR lpReturndata);

// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// Member Variable
	Comm *con;			// Atmega128 Connection Administer
	int con_flag;		// Connection State
	boolean ledState[8];	// LED State List
	CString receive_str;// Atmega128�� ��Ŷ ���ڿ��� �����ϴ� ���ڿ�
	int checkSec;		// �� Ȯ��

	// Member Function
	void onOffControl(boolean flag);	// GUI â�� IDC_CONNECTION_ON/OFF ����
	int beforeLCD, currentLCD;
	void sendAck();						// send ACK(01)
	void receiveLED(Packet p);			// Receive LED Info
	void receiveVoltage(Packet p);		// Receive Voltage Info
	void receiveTime(Packet p);			// Receive Time Info
	void sendLedState(int clickedIndex);
	void parsingMsg(Packet m_PACKET);	// �޽����� ��Ŷ ����ü�� ����
	// 01 : ACK
	// 10 : Connection Check
	// 20 ; Led State Transit
	// 21 : Led State Receive
	// 30 : Volumn Value Transit
	// 40 : Time Transit
	void cmdProcess(Packet p);					// ��Ŷ�� ���� ���� cmd�� ó��


	// GUI
	CStatic m_logo;				
	CComboBox m_comboBox;
	CStatic m_time;
	CStatic m_picture;
	CProgressCtrl m_progress;
	CListBox m_stateList;
	CButton m_conn;
	CButton m_disConn;
	CFont fontOn;
	CFont fontOff;
	CStatic m_off;
	CStatic m_on;

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
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	void drawButton(int btnNumber, LPDRAWITEMSTRUCT lpDrawItemStruct);

	LRESULT	OnThreadClosed(WPARAM length, LPARAM lpara);
	LRESULT	OnReceive(WPARAM length, LPARAM lpara);
};
