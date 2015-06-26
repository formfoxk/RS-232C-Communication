
// Emulator-MFCDlg.h : ��� ����
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
	PACKET m_PACKET;	// Packet ����ü ����
	boolean led_state[8];	// LED State List
	CString receive_str;// Atmega128�� ��Ŷ ���ڿ��� �����ϴ� ���ڿ�

	// Member Function
	void onOffControl(boolean flag);	// GUI â�� IDC_CONNECTION_ON/OFF ����
	void connectionCheck();				// CMD 3  Data[8] send
	void adcCheck();					
	void ledStateSend();				
	void ledStateRequest();				// Led���� ���� ��û					
	void adcConvertLed();				// ��ư�� ���¸� ���� �������� 2������ ��ȯ�Ͽ� GUI�� ���
	void parsingMsg();					// �޽����� ��Ŷ ����ü�� ����
	// 01 : ACK
	// 10 : Connection Check
	// 20 ; Led State Transit
	// 21 : Led State Receive
	// 30 : Volumn Value Transit
	// 40 : Time Transit
	void cmdProcess();					// ��Ŷ�� ���� ���� cmd�� ó��


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
