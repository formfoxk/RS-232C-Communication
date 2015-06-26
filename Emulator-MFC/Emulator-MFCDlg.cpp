
// Emulator-MFCDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "Emulator-MFC.h"
#include "Emulator-MFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CEmulatorMFCDlg 대화 상자



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


// CEmulatorMFCDlg 메시지 처리기

BOOL CEmulatorMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CEmulatorMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CEmulatorMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CEmulatorMFCDlg::OnThreadClosed(WPARAM length, LPARAM lpara)
{
	//AfxMessageBox(_T("내용"));

	//overlapped i/o 핸들을 닫는다.
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

//메시지 수신 처리
// 3초이상 시간이 지나도 오지 않으면,

LRESULT CEmulatorMFCDlg::OnReceive(WPARAM length, LPARAM lpara)
{  // Target으로부터 메시지를 받는 함수
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

			if(data[i]==0x03)  // 전송이 끝나면
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
	for(int i=0; i<2; i++){    // Data size CString타입으로 변환
		temp += m_PACKET.size[i];
	}

	int len = _ttoi(temp);     // 다시 CString타입을 int형으로 변환하여 길이를 구함
	for(int i=0; i<len;i++)
	{
		packetData += receive_str[7+i];  // 길이만큼 데이터를 out2에 넣음
	}

	packetTail.Format(_T(" 0x0%x 0x0%x"),m_PACKET.checkSum,m_PACKET.etx);

	packet += packetData; //명령어 길이 추가
	packet += packetTail; // CheckSum,ETX 추가
	m_stateList.AddString(packet);
}


void CEmulatorMFCDlg::OnBnClickedButton8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CEmulatorMFCDlg::OnBnClickedButton7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CEmulatorMFCDlg::OnBnClickedButton6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CEmulatorMFCDlg::OnBnClickedButton5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CEmulatorMFCDlg::OnBnClickedButton4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CEmulatorMFCDlg::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CEmulatorMFCDlg::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CEmulatorMFCDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CEmulatorMFCDlg::OnBnClickedConn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CEmulatorMFCDlg::OnBnClickedDisconn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
