
// Emulator-MFCDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "Emulator-MFC.h"
#include "Emulator-MFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

char* serialization(struct Packet* pk)
{
   return (char*)pk;
}
struct Packet* unserialization(char* buff)
{
   return (struct Packet*)buff;
}

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
	DDX_Control(pDX, IDC_TIME, m_time);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
	DDX_Control(pDX, IDC_STATELIST, m_stateList);
	DDX_Control(pDX, IDC_PICTURE, m_picture);
	DDX_Control(pDX, IDC_CONN, m_conn);
	DDX_Control(pDX, IDC_DISCONN, m_disConn);
	DDX_Control(pDX, IDC_OFF, m_off);
	DDX_Control(pDX, IDC_ON, m_on);
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
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_DRAWITEM()
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
	checkSec = 3;
	
	// ● 크기 조정
	fontOn.CreatePointFont(150,_T("돋움"));                    
	GetDlgItem(IDC_ON)->SetFont(&fontOn);

	fontOff.CreatePointFont(150,_T("돋움"));                    
	GetDlgItem(IDC_OFF)->SetFont(&fontOff);

	onOffControl(false);

	for(int i = 0; i < 8; i++)
		ledState[i] = false;


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
		//사진 등록
		/*CBitmap bitmap;
		bitmap.LoadBitmap(IDB_BITMAP1);
		m_picture.SetBitmap((HBITMAP)bitmap.GetSafeHandle());*/

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
	if(con)
	{
		Packet *p;
		char str[100];

		if (length < sizeof(Packet))
		{
			int count = 0;
			for (UINT i = 0; i < sizeof(Packet); i++) {
				con->Receive(&(str[i]), 1);
			}
			p = unserialization(str);

			cmdProcess(*p);
			// reset packet
			memset(p, 0, sizeof(Packet));
		}
	}
	return 0;
}

void CEmulatorMFCDlg::cmdProcess(Packet p){
	if(p.mCmd < END_PROTOCOL && p.mCmd > START_PROTOCOL){
		CString alarm;
		alarm = "";

		switch (p.mCmd)
		{
		case ACK:
			alarm.Format(_T("Check receive ACK and send ACK"));
			sendAck();	// ACK가 오면 ACK를 다시 보낸다.
			break;
		case SEND_LED:
			alarm.Format(_T("Receive LED Info"));
			receiveLED(p);
			break;

		case VOLTAGE:
			alarm.Format(_T("Receive VOLTAGE Info"));
			receiveVoltage(p);
			break;

		case TIME:
			alarm.Format(_T("Receive TIME Info"));
			receiveTime(p);
			break;

		default:
			alarm.Format(_T("ERROR"));
			break;
		}
		m_stateList.AddString(alarm);
	}
}
void CEmulatorMFCDlg::sendAck(){
	Packet p;

	p.mStx = 0x02;
	p.mSrc = 0;
	p.mDes = 0;
	p.mCmd = RACK;
	p.mSize = 0;
	p.mChk = 0x04;
	p.mEtx = 0x03;

	con->Send((LPCTSTR)serialization(&p), sizeof(p));
};			
void CEmulatorMFCDlg::receiveLED(Packet p){
	int size = p.mSize;
	if (size > 10) return;
	for(int i = 0; i < size; i++){
		if(p.mData[i] == '0')
			ledState[i] = false;
		else if(p.mData[i] == '1')
			ledState[i] = true;
		/*else
			AfxMessageBox(_T("ERROR : receiveLED(Packet p)"));*/
	}
	Invalidate(false); // 화면 재구성
}
void CEmulatorMFCDlg::receiveVoltage(Packet p){
	CString vol;
	vol = "";

	int size = p.mSize;
	if (size > 2) return;
	for(int i = 0; i < size;i++)
		vol += p.mData[i];
	/*
	CString s;
	s.Format(_T("%s"), vol);
	AfxMessageBox(s);*/

	int pos;
	pos = _ttoi(vol);

	if (pos > 0 && pos < 100) {
		m_progress.SetPos(pos);
	}
}
void CEmulatorMFCDlg::receiveTime(Packet p){
	CString time;

	time.Format(_T("%c%c:%c%c:%c%c"), p.mData[0], p.mData[1], p.mData[2], p.mData[3], p.mData[4], p.mData[5]);
	m_time.SetWindowTextW(time);

	Invalidate();
}



HBRUSH CEmulatorMFCDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	CRect rect;
	UINT nID = pWnd->GetDlgCtrlID();

	if(nCtlColor == CTLCOLOR_STATIC)
	{    
        if(nID == IDC_LOGO)
        {
               pDC->SetBkMode(TRANSPARENT);      // 배경투명
               pDC->SetTextColor(RGB(0, 0, 250));  // 글자 색
               pDC->SetBkColor(RGB(0, 30, 0));  // 배경 색
               return (HBRUSH)::GetStockObject(NULL_BRUSH);
        }
		else if(nID == IDC_ON)
		{
			   pDC->SetBkMode(TRANSPARENT);      // 배경투명
               pDC->SetTextColor(RGB(000, 000, 204));  // 글자 색
               pDC->SetBkColor(RGB(0, 000, 204));  // 배경 색
               return (HBRUSH)::GetStockObject(NULL_BRUSH);
		}
		else if(nID == IDC_OFF)
		{
			   pDC->SetBkMode(TRANSPARENT);      // 배경투명
               pDC->SetTextColor(RGB(255, 0, 0));  // 글자 색
               pDC->SetBkColor(RGB(255, 0, 0));  // 배경 색
               return (HBRUSH)::GetStockObject(NULL_BRUSH);
		}
	}
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


void CEmulatorMFCDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(nIDEvent==1)
	{
		checkSec--;
		if(checkSec <= 0){
			sendAck();
			checkSec = 3;

			KillTimer(1);
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CEmulatorMFCDlg::OnBnClickedConn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	CString port;
	m_comboBox.GetLBText(m_comboBox.GetCurSel(), port);

	if(port.GetLength() == 5)
		port = _T("\\\\.\\") + port;

	con = new Comm(port, _T("9600"), _T("None"), _T("8 Bit"), _T("1 Bit"));

	//포트와 연결 성공시
	if(con->Create(this->m_hWnd)==TRUE)
	{
		m_stateList.AddString(_T("Sucess Port Connection"));
		m_stateList.SetCurSel(m_stateList.GetCount()-1);
		m_conn.EnableWindow(false);
		m_disConn.EnableWindow(true);
		
		sendAck(); //Connection 성공 알림 패킷 전송

		onOffControl(true);
	}
	else
	{
		m_stateList.AddString(_T("Fail Port Connection"));
		m_stateList.SetCurSel(m_stateList.GetCount()-1);

		onOffControl(false);
	}
}

void CEmulatorMFCDlg::OnBnClickedDisconn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString temp;

	con->HandleClose();
	con->Close();

	m_conn.EnableWindow(true);
	m_disConn.EnableWindow(false);

	onOffControl(false);
	m_stateList.AddString(_T("Disconnect Port"));
	m_stateList.SetCurSel(m_stateList.GetCount()-1);


	KillTimer(1);
}

void CEmulatorMFCDlg::OnBnClickedButton8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ledState[7] = !ledState[7];

	sendLedState(7);
}


void CEmulatorMFCDlg::OnBnClickedButton7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ledState[6] = !ledState[6];

	sendLedState(6);
}


void CEmulatorMFCDlg::OnBnClickedButton6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ledState[5] = !ledState[5];

	sendLedState(5);
}


void CEmulatorMFCDlg::OnBnClickedButton5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ledState[4] = !ledState[4];

	sendLedState(4);
}


void CEmulatorMFCDlg::OnBnClickedButton4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ledState[3] = !ledState[3];

	sendLedState(3);
}


void CEmulatorMFCDlg::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ledState[2] = !ledState[2];

	sendLedState(2);
}


void CEmulatorMFCDlg::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ledState[1] = !ledState[1];

	sendLedState(1);
}


void CEmulatorMFCDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ledState[0] = !ledState[0];

	sendLedState(0);
}


void CEmulatorMFCDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(nIDCtl == IDC_BUTTON1)
		drawButton(0, lpDrawItemStruct);
	else if(nIDCtl == IDC_BUTTON2)
		drawButton(1, lpDrawItemStruct);
	else if(nIDCtl == IDC_BUTTON3)
		drawButton(2, lpDrawItemStruct);
	else if(nIDCtl == IDC_BUTTON4)
		drawButton(3, lpDrawItemStruct);
	else if(nIDCtl == IDC_BUTTON5)
		drawButton(4, lpDrawItemStruct);
	else if(nIDCtl == IDC_BUTTON6)
		drawButton(5, lpDrawItemStruct);
	else if(nIDCtl == IDC_BUTTON7)
		drawButton(6, lpDrawItemStruct);
	else if(nIDCtl == IDC_BUTTON8)
		drawButton(7, lpDrawItemStruct);
}
void CEmulatorMFCDlg::sendLedState(int clickedIndex){
	Packet p;

	p.mStx = 0x02;
	p.mSrc = 0;
	p.mDes = 0;
	p.mCmd = CHANGE_LED;
	p.mSize = 9;
	for(int i = 0; i < 8; i++)
		p.mData[i] = '0';
	p.mData[clickedIndex] = '1';
	p.mChk = 0x04;
	p.mEtx = 0x03;

	for (int i = 0; i < sizeof(p); i++)
		con->Send((LPCTSTR)(serialization(&p) + i), 1);

	Invalidate();
}

void CEmulatorMFCDlg::drawButton(int btnNumber, LPDRAWITEMSTRUCT lpDrawItemStruct){
	CDC dc;
	RECT rect;
	dc.Attach(lpDrawItemStruct->hDC);						//버튼의 dc구하기
	rect = lpDrawItemStruct->rcItem	;						//버튼영역 구하기

	// LED가 켜진 경우
	if(ledState[btnNumber]){
		dc.Draw3dRect(&rect, RGB(255,102,102), RGB(255,102,102));	//버튼의 외각선 그리기
		dc.FillSolidRect(&rect, RGB(255,102,102));					//버튼색상
	}
	else
	{
		dc.Draw3dRect(&rect, RGB(255,255,255), RGB(255,255,255));	//버튼의 외각선 그리기
		dc.FillSolidRect(&rect, RGB(255,255,255));					//버튼색상
	}



	//Show the Effect of Click Event
	UINT state = lpDrawItemStruct->itemState;				//버튼상태구하기
	if((state & ODS_SELECTED))
	{
		dc.DrawEdge(&rect, EDGE_SUNKEN, BF_RECT);
	}
	else
	{
		dc.DrawEdge(&rect, EDGE_RAISED, BF_RECT);
	}

	//Draw Color Text
	dc.SetBkColor(RGB(255,102,102));						//Setting the Text Background Color
	dc.SetTextColor(RGB(255,255,255));						//Setting the Text Color

	TCHAR buffer[MAX_PATH];					
	ZeroMemory(buffer, MAX_PATH);									//버퍼초기화
	::GetWindowText(lpDrawItemStruct->hwndItem, buffer, MAX_PATH);	//버튼의 text얻기

	dc.DrawText(buffer, &rect, DT_CENTER|DT_VCENTER |DT_SINGLELINE);//버튼의 text넣기
	dc.Detach();													//버튼의 dc 풀어주기

}

// Member Function
void CEmulatorMFCDlg::onOffControl(boolean flag){
	// On인 경우
	if(flag){
		GetDlgItem(IDC_ON)->ShowWindow(TRUE);
		GetDlgItem(IDC_OFF)->ShowWindow(FALSE);	
	}else{
		GetDlgItem(IDC_ON)->ShowWindow(FALSE);	
		GetDlgItem(IDC_OFF)->ShowWindow(TRUE);
	}
}


