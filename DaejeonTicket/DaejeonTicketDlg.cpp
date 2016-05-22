
// DaejeonTicketDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "DaejeonTicket.h"
#include "DaejeonTicketDlg.h"
#include "afxdialogex.h"
#include <iostream>
using namespace std;

#define COL 640

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 카메라 영상 및 정보를 저장하기 위한 전역 변수
BITMAPINFO BmInfo;
LPBYTE pImgBuffer;

// win32 program에서 console창을 띄우기 위한 방법
// printf를 사용하기 위해 stdio.h를 include
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDaejeonTicketDlg 대화 상자



CDaejeonTicketDlg::CDaejeonTicketDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DAEJEONTICKET_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDaejeonTicketDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDaejeonTicketDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CDaejeonTicketDlg 메시지 처리기

BOOL CDaejeonTicketDlg::OnInitDialog()
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	RECT m_Rect = { 0, 0, 640, 480 };     // 생성하고 싶은 사이즈를 RECT 변수에 초기화. 
										  //  여기서는 1024x768로 설정하였다.
	AdjustWindowRect(&m_Rect, WS_OVERLAPPEDWINDOW, FALSE);
	int width = m_Rect.right - m_Rect.left;
	int height = m_Rect.bottom - m_Rect.top;
	this->SetWindowPos(NULL, 0, 0, width, height, SWP_NOSIZE);

	// 윈도우 생성
	m_Cap = capCreateCaptureWindow(TEXT("original_image"), WS_CHILD
		| WS_VISIBLE, 0, 0, 640, 480, this->m_hWnd, NULL);


	// 콜백함수 지정
	if (capSetCallbackOnFrame(m_Cap, CallbackOnFrame) == FALSE) {
		return FALSE;
	}


	// 카메라 드라이버와 연결
	if (capDriverConnect(m_Cap, 0) == FALSE) {
		return FALSE;
	}
	

	// 카메라 설정
	capPreviewRate(m_Cap, 33);    // 초당 프레임 지정
	capOverlay(m_Cap, false);
	capPreview(m_Cap, true);        // 미리보기 기능 설정

	

	if (BmInfo.bmiHeader.biBitCount != 24) {

		BmInfo.bmiHeader.biBitCount = 24;
		BmInfo.bmiHeader.biCompression = 0;
		BmInfo.bmiHeader.biSizeImage = BmInfo.bmiHeader.biWidth *
			BmInfo.bmiHeader.biHeight * 3;

		capGetVideoFormat(m_Cap, &BmInfo, sizeof(BITMAPINFO));

	}
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CDaejeonTicketDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CDaejeonTicketDlg::OnPaint()
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
HCURSOR CDaejeonTicketDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDaejeonTicketDlg::OnDestroy()
{
	CDialog::OnDestroy();

	capDriverDisconnect(m_Cap);

	if (pImgBuffer != NULL) {
		delete[] pImgBuffer;
	}
}

LRESULT CALLBACK CallbackOnFrame(HWND hWnd, LPVIDEOHDR lpVHdr)
{

	cout << "test" << endl;
	/*
	영상 데이터는
	lpVHdr->lpData 에 1차원 배열로 저장되어 있다.
	*/
	BYTE pixel_red, pixel_green, pixel_blue;
	int  i, j, index, counter = 0;

	if (pImgBuffer == NULL)
		pImgBuffer = (LPBYTE)new BYTE[BmInfo.bmiHeader.biHeight*BmInfo.bmiHeader.biWidth];

	for (i = 0; i <BmInfo.bmiHeader.biWidth*BmInfo.bmiHeader.biHeight; i++)
	{

		// 인식할 색상을 고르는 코드를 삽입하세요.

		//명도를 인식할때
		/*pixel = (*(lpVHdr->lpData + (i * 3)) +
			*(lpVHdr->lpData + (i * 3) + 1) +
			*(lpVHdr->lpData + (i * 3) + 2)) / 3;*/
			

		// 파랑색이 일정수준 이상일때
		pixel_blue = (*(lpVHdr->lpData + (i * 3)));
		// 초록색이 일정수준 이상일때
		pixel_green = (*(lpVHdr->lpData + (i * 3) + 1));
		// 빨강색이 일정수준 이상일때
		pixel_red = (*(lpVHdr->lpData + (i * 3) + 2));

		if (pixel_blue > 100 && pixel_red < 30 && pixel_green < 30) {
			
			
			*(lpVHdr->lpData + 3 * (i)) = 255;
			*(lpVHdr->lpData + 3 * (i) + 1) = 0;
			*(lpVHdr->lpData + 3 * (i) + 2) = 0;
	
		}
		else {

		}
		//if (pixel > 200) *(pImgBuffer + i) = 255;
		//else *(pImgBuffer + i) = 0;
	}


	//int xCenter = 0, yCenter = 0;
	//for (i = 0; i<BmInfo.bmiHeader.biHeight; i++)
	//{
	//	index = i*BmInfo.bmiHeader.biWidth;
	//	for (j = 0; j<BmInfo.bmiHeader.biWidth; j++)
	//	{
	//		if (*(pImgBuffer + index + j) == 255)
	//		{
	//			xCenter += i;
	//			yCenter += j;
	//			counter++;
	//		}
	//	}
	//}



	//// to make cross point
	//xCenter = (int)((float)xCenter / (float)counter);
	//yCenter = (int)((float)yCenter / (float)counter);
	///*int kasd = 0;
	//kasd = (float)xCenter + (int)30;
	//int kase = 0;
	//kase = (float)xCenter - (int)30;
	//*/
	//for (i = xCenter - 15; i <= xCenter + 15; i++)
	//{
	//	if (i<0 || i >= BmInfo.bmiHeader.biHeight) continue;
	//	index = i*BmInfo.bmiHeader.biWidth;
	//	*(lpVHdr->lpData + 3 * (index + yCenter)) = 0;
	//	*(lpVHdr->lpData + 3 * (index + yCenter) + 1) = 0;
	//	*(lpVHdr->lpData + 3 * (index + yCenter) + 2) = 255;
	//}

	//index = xCenter*BmInfo.bmiHeader.biWidth;
	//for (j = yCenter - 15; j <= yCenter + 15; j++)
	//{
	//	if (j<0 || j >= BmInfo.bmiHeader.biWidth) continue;
	//	*(lpVHdr->lpData + 3 * (index + j)) = 0;
	//	*(lpVHdr->lpData + 3 * (index + j) + 1) = 0;
	//	*(lpVHdr->lpData + 3 * (index + j) + 2) = 255;
	//}

	// 차이가 나는 화소의 수를 caption bar에 표시
	CString  strTitle;
	//strTitle.Format(_T("Binary Tracker (%d,%d)"), xCenter, yCenter);
	AfxGetMainWnd()->SetWindowText(strTitle);

	return (LRESULT)true;

}

