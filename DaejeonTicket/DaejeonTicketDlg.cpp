
// DaejeonTicketDlg.cpp : ���� ����
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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ī�޶� ���� �� ������ �����ϱ� ���� ���� ����
BITMAPINFO BmInfo;
LPBYTE preImage;
CTools tools;
int isChange[WIDTH][HEIGHT] = { 0, };
double preHsv[WIDTH][HEIGHT][3] = { 0, };
int xAverage = 0, yAverage = 0;

char state;

// win32 program���� consoleâ�� ���� ���� ���
// printf�� ����ϱ� ���� stdio.h�� include
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CDaejeonTicketDlg ��ȭ ����



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


// CDaejeonTicketDlg �޽��� ó����

BOOL CDaejeonTicketDlg::OnInitDialog()
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

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	RECT m_Rect = { 0, 0, WIDTH, HEIGHT };     // �����ϰ� ���� ����� RECT ������ �ʱ�ȭ. 
										  //  ���⼭�� 1024x768�� �����Ͽ���.
	AdjustWindowRect(&m_Rect, WS_OVERLAPPEDWINDOW, FALSE);
	int width = m_Rect.right - m_Rect.left;
	int height = m_Rect.bottom - m_Rect.top;
	this->SetWindowPos(NULL, 0, 0, width, height, SWP_NOSIZE);

	// ������ ����
	m_Cap = capCreateCaptureWindow(TEXT("original_image"), WS_CHILD
		| WS_VISIBLE, 0, 0, WIDTH, HEIGHT, this->m_hWnd, NULL);
	cout << "PROGRAM START";

	// �ݹ��Լ� ����
	if (capSetCallbackOnFrame(m_Cap, CallbackOnFrame) == FALSE) {
		return FALSE;
	}


	// ī�޶� ����̹��� ����
	if (capDriverConnect(m_Cap, 0) == FALSE) {
		return FALSE;
	}
	

	// ī�޶� ����
	capPreviewRate(m_Cap, 30);    // �ʴ� ������ ����
	capOverlay(m_Cap, false);
	capPreview(m_Cap, true);        // �̸����� ��� ����

	

	if (BmInfo.bmiHeader.biBitCount != 24) {

		BmInfo.bmiHeader.biBitCount = 24;
		BmInfo.bmiHeader.biCompression = 0;
		BmInfo.bmiHeader.biSizeImage = BmInfo.bmiHeader.biWidth *
			BmInfo.bmiHeader.biHeight * 3;

		capGetVideoFormat(m_Cap, &BmInfo, sizeof(BITMAPINFO));

	}
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CDaejeonTicketDlg::OnPaint()
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


	//--init value--// 
	double fH, fS, fV;
	int index=0, counter = 0;
	int indexH, indexW;
	BYTE currentPixel, currentRed, currentBlue, currentGreen;
	BYTE prePixel, preRed, preGreen, preBlue;

	if(preImage == NULL)
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
			//-- copy currentImage to preImage --//
		}
	}
	


	//--calculate centor position--//
	int xCenter = 0, yCenter = 0;
	
	for (indexH = 0; indexH<BmInfo.bmiHeader.biHeight; indexH++)
	{
		
		for (indexW = 0; indexW<BmInfo.bmiHeader.biWidth; indexW++)
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
					isChange[indexW + 1][indexH - 1] + isChange[indexW-2][indexH-2] + 
					isChange[indexW-2][indexH-1] + isChange[indexW-2][indexH] + 
					isChange[indexW-2][indexH+1] + isChange[indexW-2][indexH+2] + 
					isChange[indexW-1][indexH-2] + isChange[indexW][indexH-2] + 
					isChange[indexW+1][indexH-2] + isChange[indexW+2][indexH-2] + 
					isChange[indexW+2][indexH-1] + isChange[indexW+2][indexH] + 
					isChange[indexW+2][indexH+1] + isChange[indexW+2][indexH+2] + 
					isChange[indexW+1][indexH+2] + isChange[indexW][indexH+2] + 
					isChange[indexW-1][indexH+2]) > 15;

	

			tools.rgb2hsv(lpVHdr, index, fH, fS, fV);
			

			//isMask = 0;
			switch (state) {
			case 'b':
				if ((tools.isBlue(fH,fS,fV) && isMask) ||
					(tools.isBlue(preHsv[indexW][indexH][0], preHsv[indexW][indexH][1], preHsv[indexW][indexH][2]) && isMask) ) 
				{
					xCenter += indexH;
					yCenter += indexW;
					counter++;
					tools.setImage(lpVHdr, index * 3, 255);
					tools.setImage(lpVHdr, index * 3 + 1, 255);
					tools.setImage(lpVHdr, index * 3 + 2, 255);
				}

				break;
			case 'g':
				if ((tools.isGreen(fH, fS, fV) && isMask) ||
					(tools.isGreen(preHsv[indexW][indexH][0], preHsv[indexW][indexH][1], preHsv[indexW][indexH][2]) && isMask))
				{
					xCenter += indexH;
					yCenter += indexW;
					counter++;
					tools.setImage(lpVHdr, index * 3, 255);
					tools.setImage(lpVHdr, index * 3 + 1, 255);
					tools.setImage(lpVHdr, index * 3 + 2, 255);
				}
				break;
			case 'r':
				if ((tools.isRed(fH, fS, fV) && isMask) ||
					(tools.isRed(preHsv[indexW][indexH][0], preHsv[indexW][indexH][1], preHsv[indexW][indexH][2]) && isMask))
				{
					xCenter += indexH;
					yCenter += indexW;
					counter++;
					tools.setImage(lpVHdr, index * 3, 255);
					tools.setImage(lpVHdr, index * 3 + 1, 255);
					tools.setImage(lpVHdr, index * 3 + 2, 255);
				}
				break;
			case 'y':
				if ((tools.isYellow(fH, fS, fV) && (fV >= 0.2 && fV <= 1) && isMask) ||
					(tools.isYellow(preHsv[indexW][indexH][0], preHsv[indexW][indexH][1], preHsv[indexW][indexH][2]) && isMask))
				{
					xCenter += indexH;
					yCenter += indexW;
					counter++;
					tools.setImage(lpVHdr, index * 3, 255);
					tools.setImage(lpVHdr, index * 3 + 1, 255);
					tools.setImage(lpVHdr, index * 3 + 2, 255);
				}
				break;
			default:
				
				break;
			}
			preHsv[indexW][indexH][0] = fH;
			preHsv[indexW][indexH][1] = fS;
			preHsv[indexW][indexH][2] = fV;
		}
	}
	if (counter > 100 && counter < 100000) {
		xAverage = (int)((float)xCenter / (float)counter);
		yAverage = (int)((float)yCenter / (float)counter);
	}
	//--calculate centor position--//

	
	tools.makeCrossPoint(lpVHdr, xAverage, yAverage, BmInfo);
//	cout << counter << endl;
	counter = 0;

	//--set mfc title--//
	CString  strTitle;
	strTitle.Format(_T("Center is (%d,%d)"), xAverage, yAverage);
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