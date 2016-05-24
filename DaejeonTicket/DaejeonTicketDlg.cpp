
// DaejeonTicketDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "DaejeonTicket.h"
#include "DaejeonTicketDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <Windows.h>
using namespace std;

#define HEIGHT 480
#define WIDTH 640
#define LENGTH 15
#define Pi				3.141592
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 카메라 영상 및 정보를 저장하기 위한 전역 변수
BITMAPINFO BmInfo;
LPBYTE preImage;
CTools tools;
int isChange[WIDTH][HEIGHT] = { 0, };
double preHsv[WIDTH][HEIGHT][3] = { 0, };
int xAverage[LENGTH] = { 0, };
int yAverage[LENGTH] = { 0, };
char whatColor='N';
char state;



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
	, isGreen(false)
	, isYellow(false)
	, isBlue(false)
	, isRed(false)
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
	ON_BN_CLICKED(IDOK3, &CDaejeonTicketDlg::OnBnClickedOk3)
	ON_BN_CLICKED(IDOK4, &CDaejeonTicketDlg::OnBnClickedOk4)
	ON_BN_CLICKED(IDOK5, &CDaejeonTicketDlg::OnBnClickedOk5)
	ON_BN_CLICKED(IDOK6, &CDaejeonTicketDlg::OnBnClickedOk6)
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
	RECT m_Rect = { 0, 0, WIDTH, HEIGHT };     // 생성하고 싶은 사이즈를 RECT 변수에 초기화. 
										  //  여기서는 1024x768로 설정하였다.
	AdjustWindowRect(&m_Rect, WS_OVERLAPPEDWINDOW, FALSE);
	int width = m_Rect.right - m_Rect.left;
	int height = m_Rect.bottom - m_Rect.top;
	this->SetWindowPos(NULL, 0, 0, width, height, SWP_NOSIZE);

	// 윈도우 생성
	m_Cap = capCreateCaptureWindow(TEXT("original_image"), WS_CHILD
		| WS_VISIBLE, 0, 0, WIDTH, HEIGHT, this->m_hWnd, NULL);
	cout << "PROGRAM START";

	// 콜백함수 지정
	if (capSetCallbackOnFrame(m_Cap, CallbackOnFrame) == FALSE) {
		return FALSE;
	}


	// 카메라 드라이버와 연결
	if (capDriverConnect(m_Cap, 0) == FALSE) {
		return FALSE;
	}
	

	// 카메라 설정
	capPreviewRate(m_Cap, 30);    // 초당 프레임 지정
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

	if (preImage != NULL) {
		delete[] preImage;
	}
}

LRESULT CALLBACK CallbackOnFrame(HWND hWnd, LPVIDEOHDR lpVHdr)
{
	CButton* blueButton = (CButton*)GetDlgItem(hWnd,IDC_RADIO_BLUE);
	CButton* greenButton = (CButton*)GetDlgItem(hWnd, IDC_RADIO_GREEN);
	CButton* redButton = (CButton*)GetDlgItem(hWnd, IDC_RADIO_RED);
	CButton* yellowButton = (CButton*)GetDlgItem(hWnd, IDC_RADIO_YELLOW);

	

	//--init value--// 
	double fH, fS, fV;
	int index = 0, counter = 0;
	int counterR = 0, counterG = 0, counterB = 0, counterY = 0;
	int indexH, indexW;
	BYTE currentPixel, currentRed, currentBlue, currentGreen;
	BYTE prePixel, preRed, preGreen, preBlue;

	if (preImage == NULL)
		preImage = (LPBYTE)new BYTE[BmInfo.bmiHeader.biHeight*BmInfo.bmiHeader.biWidth];

	//--init value--//

	for (indexH = 0; indexH < BmInfo.bmiHeader.biHeight; indexH++)
	{ // indexH is height
		for (indexW = 0; indexW < BmInfo.bmiHeader.biWidth; indexW++) {
			// indexW is width
			index = indexH*BmInfo.bmiHeader.biWidth + indexW;


			currentBlue = tools.getImage(lpVHdr, index * 3);
			currentGreen = tools.getImage(lpVHdr, index * 3 + 1);
			currentRed = tools.getImage(lpVHdr, index * 3 + 2);
			// copy image to currentImage BYTE value.

			currentPixel = (currentBlue + currentGreen + currentRed) / 3;
			prePixel = *(preImage + index);

			bool logic = 0;
			logic = abs(currentPixel - prePixel) > 20;


			if (logic) {
				isChange[indexW][indexH] = 1;
			}

			else {
				isChange[indexW][indexH] = 0;
			}

			//-- copy currentImage to preImage --//
			*(preImage + index) = currentPixel;
		}
	}



	//--calculate centor position--//
	int xCenter = 0, yCenter = 0;
	int xCenterR = 0, yCenterR = 0;
	int xCenterG = 0, yCenterG = 0;
	int xCenterB = 0, yCenterB = 0;
	int xCenterY = 0, yCenterY = 0;
	for (indexH = 0; indexH < BmInfo.bmiHeader.biHeight; indexH++)
	{

		for (indexW = 0; indexW < BmInfo.bmiHeader.biWidth; indexW++)
		{
			index = indexH*BmInfo.bmiHeader.biWidth + indexW;
			bool isMask = 0;
			if (indexW > 1 && indexH > 1 &&
				indexW < (BmInfo.bmiHeader.biWidth - 2) &&
				indexH < (BmInfo.bmiHeader.biHeight - 2))
				isMask =
				(isChange[indexW][indexH] + isChange[indexW - 1][indexH - 1] +
					isChange[indexW][indexH - 1] + isChange[indexW - 1][indexH] +
					isChange[indexW + 1][indexH + 1] + isChange[indexW + 1][indexH] +
					isChange[indexW][indexH + 1] + isChange[indexW - 1][indexH + 1] +
					isChange[indexW + 1][indexH - 1] + isChange[indexW - 2][indexH - 2] +
					isChange[indexW - 2][indexH - 1] + isChange[indexW - 2][indexH] +
					isChange[indexW - 2][indexH + 1] + isChange[indexW - 2][indexH + 2] +
					isChange[indexW - 1][indexH - 2] + isChange[indexW][indexH - 2] +
					isChange[indexW + 1][indexH - 2] + isChange[indexW + 2][indexH - 2] +
					isChange[indexW + 2][indexH - 1] + isChange[indexW + 2][indexH] +
					isChange[indexW + 2][indexH + 1] + isChange[indexW + 2][indexH + 2] +
					isChange[indexW + 1][indexH + 2] + isChange[indexW][indexH + 2] +
					isChange[indexW - 1][indexH + 2]) > 15;



			tools.rgb2hsv(lpVHdr, index, fH, fS, fV);


			//isMask = 0;
			switch (state) {
			case 'b':
				if(isMask)
					if (tools.isBlue(fH, fS, fV) ||
						tools.isBlue(preHsv[indexW][indexH][0], preHsv[indexW][indexH][1], preHsv[indexW][indexH][2]))
					{
						whatColor = 'B';
						xCenter += indexH;
						yCenter += indexW;
						counter++;
						tools.setImage(lpVHdr, index * 3, 255);
						tools.setImage(lpVHdr, index * 3 + 1, 255);
						tools.setImage(lpVHdr, index * 3 + 2, 255);
					}

				break;
			case 'g':
				if(isMask)
					if (tools.isGreen(fH, fS, fV) ||
						tools.isGreen(preHsv[indexW][indexH][0], preHsv[indexW][indexH][1], preHsv[indexW][indexH][2]))
					{
						whatColor= 'G';
						xCenter += indexH;
						yCenter += indexW;
						counter++;
						tools.setImage(lpVHdr, index * 3, 255);
						tools.setImage(lpVHdr, index * 3 + 1, 255);
						tools.setImage(lpVHdr, index * 3 + 2, 255);
					}
				break;
			case 'r':
				if(isMask)
					if (tools.isRed(fH, fS, fV) ||
						tools.isRed(preHsv[indexW][indexH][0], preHsv[indexW][indexH][1], preHsv[indexW][indexH][2]))
					{
						whatColor = 'R';
						xCenter += indexH;
						yCenter += indexW;
						counter++;
						tools.setImage(lpVHdr, index * 3, 255);
						tools.setImage(lpVHdr, index * 3 + 1, 255);
						tools.setImage(lpVHdr, index * 3 + 2, 255);
					}
				break;
			case 'y':
				if(isMask)
					if (tools.isYellow(fH, fS, fV) ||
						tools.isYellow(preHsv[indexW][indexH][0], preHsv[indexW][indexH][1], preHsv[indexW][indexH][2]))
					{
						whatColor='Y';
						xCenter += indexH;
						yCenter += indexW;
						counter++;
						tools.setImage(lpVHdr, index * 3, 255);
						tools.setImage(lpVHdr, index * 3 + 1, 255);
						tools.setImage(lpVHdr, index * 3 + 2, 255);
					}
				break;
			default: 
				if(isMask)
					if (tools.isBlue(fH, fS, fV) ||
						tools.isBlue(preHsv[indexW][indexH][0], preHsv[indexW][indexH][1], preHsv[indexW][indexH][2]))
					{
						xCenterB += indexH;
						yCenterB += indexW;
						counterB++;
					}
					else if (tools.isGreen(fH, fS, fV) ||
						tools.isGreen(preHsv[indexW][indexH][0], preHsv[indexW][indexH][1], preHsv[indexW][indexH][2]))
					{
						xCenterG += indexH;
						yCenterG += indexW;
						counterG++;

					}
					else if (tools.isRed(fH, fS, fV) ||
						tools.isRed(preHsv[indexW][indexH][0], preHsv[indexW][indexH][1], preHsv[indexW][indexH][2]))
					{
						xCenterR += indexH;
						yCenterR += indexW;
						counterR++;

					}
					else if (tools.isYellow(fH, fS, fV) ||
						tools.isYellow(preHsv[indexW][indexH][0], preHsv[indexW][indexH][1], preHsv[indexW][indexH][2]))
					{
						xCenterY += indexH;
						yCenterY += indexW;
						counterY++;
					}
					
				break;
			}
			// save previous HSV value
			preHsv[indexW][indexH][0] = fH;
			preHsv[indexW][indexH][1] = fS;
			preHsv[indexW][indexH][2] = fV;
		}
	}
	if (state == 'r' || state == 'g' || state == 'b' || state == 'y') {

		if (counter > 100 && counter < 100000) {
			xAverage[0] = (int)((float)xCenter / (float)counter);
			yAverage[0] = (int)((float)yCenter / (float)counter);
		}

	}
	else{
		int maxC = MAX(counterR, MAX(counterG, MAX(counterB, counterY)));

		if (maxC == counterR && counterR > 100 && counterR < 100000) {
				whatColor = 'R';
				xAverage[0] = (int)((float)xCenterR / (float)counterR);
				yAverage[0] = (int)((float)yCenterR / (float)counterR);
		}
		else if (maxC == counterG  && counterG > 100 && counterG < 100000) {
				whatColor = 'G';
				xAverage[0] = (int)((float)xCenterG / (float)counterG);
				yAverage[0] = (int)((float)yCenterG / (float)counterG);
		}
		else if (maxC == counterB  && counterB > 100 && counterB < 100000) {
				whatColor = 'B';
				xAverage[0] = (int)((float)xCenterB / (float)counterB);
				yAverage[0] = (int)((float)yCenterB / (float)counterB);
		}
		else if (maxC == counterY && counterY > 100 && counterY < 100000) {
				whatColor = 'Y';
				xAverage[0] = (int)((float)xCenterY / (float)counterY);
				yAverage[0] = (int)((float)yCenterY / (float)counterY);
		}
		else {
			whatColor = 'N';
			xAverage[0] = 0;
			yAverage[0] = 0;
		}
	}
	//--calculate centor position--//


	//--trace center--//
	for (index = 0; index < LENGTH; index++) {
		tools.makeCrossPoint(lpVHdr, xAverage[index], yAverage[index], BmInfo);
		int xStart, xEnd, yStart, yEnd;
		if (index != (LENGTH - 1)) {
			xStart = xAverage[index];
			xEnd = xAverage[index + 1];
			yStart = yAverage[index];
			yEnd = yAverage[index + 1];
			if(xStart!=0 && xEnd!=0 && yStart!=0 && yEnd!=0)
				tools.makeLine(lpVHdr, BmInfo, xStart, xEnd, yStart, yEnd);
		}
	}
	int x = xAverage[3];
	int xp = xAverage[0];
	int y = yAverage[3];
	int yp = yAverage[0];
	int xs, ys;
	int rx, ry;
	float angle;

	xs = xp - x;
	if (xs == 0)xs = xs + 1;
	angle = atan2(yp - y, xs);
	if (angle >0) {
		angle = angle * 180 / Pi;
	}
	else {
		angle = -angle;
		angle = 360 - angle * 180 / Pi;
	}
	angle = 360 - angle;
	rx = 30 * cos(angle*Pi / 180);
	ry = 30 * sin(angle * Pi / 180);


	tools.makeLine(lpVHdr, BmInfo, 400, 400 + rx, 100, 100 - ry);



	for (index = LENGTH-1; index > 0; index--) {
		xAverage[index] = xAverage[index-1];
		yAverage[index] = yAverage[index-1];
	}
	counter = 0;
	//--trace center--//


	//--set mfc title--//
	CString  strTitle;
	strTitle.Format(_T("Center : %d,%d, Color :%c"), xAverage[0], yAverage[0], whatColor);
	AfxGetMainWnd()->SetWindowText(strTitle);
	//--set mfc title--//



	return (LRESULT)true;

}


void CDaejeonTicketDlg::OnBnClickedOk3()
{
	state = 'b';
}


void CDaejeonTicketDlg::OnBnClickedOk4()
{
	state = 'g';
}


void CDaejeonTicketDlg::OnBnClickedOk5()
{
	state = 'r';
}

void CDaejeonTicketDlg::OnBnClickedOk6()
{
	state = 'y';
}





void CDaejeonTicketDlg::OnBnClickedRadio5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
