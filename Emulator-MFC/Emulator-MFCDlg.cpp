
// Emulator-MFCDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "Emulator-MFC.h"
#include "Emulator-MFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CEmulatorMFCDlg ��ȭ ����



CEmulatorMFCDlg::CEmulatorMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEmulatorMFCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEmulatorMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOGO, m_logo);
	DDX_Control(pDX, IDC_COMBO, m_comboBox);
	DDX_Control(pDX, IDC_PICTURE, m_picture);
	DDX_Control(pDX, IDC_ONOFF, m_onoff);
	DDX_Control(pDX, IDC_TIME, m_time);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
	DDX_Control(pDX, IDC_STATELIST, m_stateList);
}

BEGIN_MESSAGE_MAP(CEmulatorMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_MYRECEIVE, OnReceive)
	ON_MESSAGE(WM_MYCLOSE, OnThreadClosed)
	ON_BN_CLICKED(IDC_BUTTON8, &CEmulatorMFCDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON7, &CEmulatorMFCDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON6, &CEmulatorMFCDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON5, &CEmulatorMFCDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON4, &CEmulatorMFCDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &CEmulatorMFCDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CEmulatorMFCDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CEmulatorMFCDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CONN, &CEmulatorMFCDlg::OnBnClickedConn)
	ON_BN_CLICKED(IDC_DISCONN, &CEmulatorMFCDlg::OnBnClickedDisconn)
END_MESSAGE_MAP()


// CEmulatorMFCDlg �޽��� ó����

BOOL CEmulatorMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CEmulatorMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CEmulatorMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CEmulatorMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CEmulatorMFCDlg::OnThreadClosed(WPARAM length, LPARAM lpara)
{
	//AfxMessageBox(_T("����"));

	//overlapped i/o �ڵ��� �ݴ´�.
	((Comm*)lpara)->HandleClose();
	delete ((Comm*)lpara);

	return 0;
}

LPSTR CEmulatorMFCDlg::UnicodeToAscii(LPWSTR lpData,LPSTR lpReturndata)
{
	WideCharToMultiByte(CP_ACP, 0, lpData, wcslen(lpData), lpReturndata, wcslen(lpData)*2, NULL, NULL);
	
	return lpReturndata;
}

LPWSTR CEmulatorMFCDlg::AsciiToUnicode(LPCSTR lpData,LPWSTR lpwszReturn)
{
	MultiByteToWideChar(CP_ACP, 0, lpData, -1, lpwszReturn, strlen(lpData)*2);
	
	return lpwszReturn;
}

//�޽��� ���� ó��
// 3���̻� �ð��� ������ ���� ������,

LRESULT CEmulatorMFCDlg::OnReceive(WPARAM length, LPARAM lpara)
{  // Target���κ��� �޽����� �޴� �Լ�
	CString temp;
	receive_str.Format(_T(""));
	temp.Format(_T(""));

	char data[2000];
	
	if(con)
	{
		
		con->Receive(data,length);

		for(int i = 0;i<(int)length;i++)
		{
			receive_str += data[i];

			if(data[i]==0x03)  // ������ ������
			{
				parsingMsg();
				cmdProcess();
				receive_str="";		
			}
		}
	}
	return 0;
}

void CEmulatorMFCDlg::parsingMsg()
{
	m_PACKET.stx = receive_str[0];
	m_PACKET.src = receive_str[1];
	m_PACKET.des = receive_str[2];
	m_PACKET.cmd[0] = receive_str[3];
	m_PACKET.cmd[1] = receive_str[4];
	m_PACKET.size[0] = receive_str[5] ;
	m_PACKET.size[1] = receive_str[6];
	m_PACKET.checkSum = receive_str[49];
	m_PACKET.etx = receive_str[50];

	for(int i = 7, j = 0; i < 49; i++, j++)
		m_PACKET.data[j] = receive_str[i];


	CString packet,packetData,packetTail;
	packet.Format(_T("0x0%x %c %c %c%c %c%c "), m_PACKET.stx, m_PACKET.src, m_PACKET.des, m_PACKET.cmd[0],m_PACKET.cmd[1],m_PACKET.size[0],m_PACKET.size[1]);

	CString temp;
	for(int i=0; i<2; i++){    // Data size CStringŸ������ ��ȯ
		temp += m_PACKET.size[i];
	}

	int len = _ttoi(temp);     // �ٽ� CStringŸ���� int������ ��ȯ�Ͽ� ���̸� ����
	for(int i=0; i<len;i++)
	{
		packetData += receive_str[7+i];  // ���̸�ŭ �����͸� out2�� ����
	}

	packetTail.Format(_T(" 0x0%x 0x0%x"),m_PACKET.checkSum,m_PACKET.etx);

	packet += packetData; //��ɾ� ���� �߰�
	packet += packetTail; // CheckSum,ETX �߰�
	m_stateList.AddString(packet);
}


void CEmulatorMFCDlg::OnBnClickedButton8()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CEmulatorMFCDlg::OnBnClickedButton7()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CEmulatorMFCDlg::OnBnClickedButton6()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CEmulatorMFCDlg::OnBnClickedButton5()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CEmulatorMFCDlg::OnBnClickedButton4()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CEmulatorMFCDlg::OnBnClickedButton3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CEmulatorMFCDlg::OnBnClickedButton2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CEmulatorMFCDlg::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CEmulatorMFCDlg::OnBnClickedConn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CEmulatorMFCDlg::OnBnClickedDisconn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

// Member Function
void CEmulatorMFCDlg::onOffControl(boolean flag){

}
void CEmulatorMFCDlg::connectionCheck(){

}
void CEmulatorMFCDlg::adcCheck(){

}					
void CEmulatorMFCDlg::ledStateSend(){

}				
void CEmulatorMFCDlg::ledStateRequest(){

}				
void CEmulatorMFCDlg::adcConvertLed(){

}
void CEmulatorMFCDlg::cmdProcess(){

}
