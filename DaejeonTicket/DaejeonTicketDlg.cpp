
// DaejeonTicketDlg.cpp : ���� ����
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

// ī�޶� ���� �� ������ �����ϱ� ���� ���� ����
BITMAPINFO BmInfo;
LPBYTE pImgBuffer;

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
	RECT m_Rect = { 0, 0, 640, 480 };     // �����ϰ� ���� ����� RECT ������ �ʱ�ȭ. 
										  //  ���⼭�� 1024x768�� �����Ͽ���.
	AdjustWindowRect(&m_Rect, WS_OVERLAPPEDWINDOW, FALSE);
	int width = m_Rect.right - m_Rect.left;
	int height = m_Rect.bottom - m_Rect.top;
	this->SetWindowPos(NULL, 0, 0, width, height, SWP_NOSIZE);

	// ������ ����
	m_Cap = capCreateCaptureWindow(TEXT("Image Test"), WS_CHILD
		| WS_VISIBLE, 0, 0, 640, 480, this->m_hWnd, NULL);

	// �ݹ��Լ� ����
	if (capSetCallbackOnFrame(m_Cap, CallbackOnFrame) == FALSE) {
		return FALSE;
	}

	// ī�޶� ����̹��� ����
	if (capDriverConnect(m_Cap, 0) == FALSE) {
		return FALSE;
	}

	capPreviewRate(m_Cap, 33);    // �ʴ� ������ ����
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

	if (pImgBuffer != NULL) {
		delete[] pImgBuffer;
	}
}

LRESULT CALLBACK CallbackOnFrame(HWND hWnd, LPVIDEOHDR lpVHdr)
{
	/*
	���� �����ʹ�
	lpVHdr->lpData �� 1���� �迭�� ����Ǿ� �ִ�.
	���⿡ ����ó�� �ڵ带 ������ �ȴ�.
	*/
	BYTE pixel;
	int  i, j, index, counter = 0;

	if (pImgBuffer == NULL)
		pImgBuffer = (LPBYTE)new BYTE[BmInfo.bmiHeader.biHeight*BmInfo.bmiHeader.biWidth];

	for (i = 0; i <BmInfo.bmiHeader.biWidth*BmInfo.bmiHeader.biHeight; i++)
	{
		pixel = (*(lpVHdr->lpData + (i * 3)) +
			*(lpVHdr->lpData + (i * 3) + 1) +
			*(lpVHdr->lpData + (i * 3) + 2)) / 3;

		if (pixel > 200) *(pImgBuffer + i) = 255;
		else *(pImgBuffer + i) = 0;
	}

	int xCenter = 0, yCenter = 0;
	for (i = 0; i<BmInfo.bmiHeader.biHeight; i++)
	{
		index = i*BmInfo.bmiHeader.biWidth;
		for (j = 0; j<BmInfo.bmiHeader.biWidth; j++)
		{
			if (*(pImgBuffer + index + j) == 255)
			{
				xCenter += i;
				yCenter += j;
				counter++;
			}
		}
	}
	xCenter = (int)((float)xCenter / (float)counter);
	yCenter = (int)((float)yCenter / (float)counter);
	int kasd = 0;
	kasd = (float)xCenter + (int)30;
	int kase = 0;
	kase = (float)xCenter - (int)30;
	for (i = xCenter - 15; i <= xCenter + 15; i++)
	{
		if (i<0 || i >= BmInfo.bmiHeader.biHeight) continue;
		index = i*BmInfo.bmiHeader.biWidth;
		*(lpVHdr->lpData + 3 * (index + yCenter)) = 0;
		*(lpVHdr->lpData + 3 * (index + yCenter) + 1) = 0;
		*(lpVHdr->lpData + 3 * (index + yCenter) + 2) = 255;
	}

	index = xCenter*BmInfo.bmiHeader.biWidth;
	for (j = yCenter - 15; j <= yCenter + 15; j++)
	{
		if (j<0 || j >= BmInfo.bmiHeader.biWidth) continue;
		*(lpVHdr->lpData + 3 * (index + j)) = 0;
		*(lpVHdr->lpData + 3 * (index + j) + 1) = 0;
		*(lpVHdr->lpData + 3 * (index + j) + 2) = 255;
	}

	
	for (j = yCenter - 30; j <= yCenter + 30; j++)
	{
		int kase = 0;
		kase = xCenter + 30;
		index = kase*BmInfo.bmiHeader.biWidth;
		if ((j + index)  < 0 && (j + index) >= 640 * 480) {
			continue;
		}
		else {
			*(lpVHdr->lpData + 3 * (index + j)) = 0;
			*(lpVHdr->lpData + 3 * (index + j) + 1) = 255;
			*(lpVHdr->lpData + 3 * (index + j) + 2) = 0;
		}
		//if (j<0 || j >= BmInfo.bmiHeader.biWidth) continue;

	}

	
	for (j = yCenter - 30; j <= yCenter + 30; j++)
	{
		int kase = 0;
		kase = xCenter - 30;
		index = kase*BmInfo.bmiHeader.biWidth;
		//if (j<0 || j >= BmInfo.bmiHeader.biWidth) continue;
		if ( (j+index)  < 0 &&  (j+index) >= 640 * 480) {
			continue;
		}
		else {
			*(lpVHdr->lpData + 3 * (index + j)) = 0;
			*(lpVHdr->lpData + 3 * (index + j) + 1) = 255;
			*(lpVHdr->lpData + 3 * (index + j) + 2) = 0;
		}
	}
	
	for (j = xCenter - 30; j <= xCenter + 30; j++)
	{
		int kase = 0;
		kase = yCenter - 30;

		index = j*BmInfo.bmiHeader.biWidth;
		//if (j<0 || j >= BmInfo.bmiHeader.biWidth) continue;
		if ((kase + index)  < 0 && (kase + index) >= 640 * 480) {
			continue;
		}
		else {
			*(lpVHdr->lpData + 3 * (index + kase)) = 0;
			*(lpVHdr->lpData + 3 * (index + kase) + 1) = 255;
			*(lpVHdr->lpData + 3 * (index + kase) + 2) = 0;
		}
	}


	for (j = xCenter - 30; j <= xCenter + 30; j++)
	{
		int kase = 0;
		kase = yCenter + 30;

		index = j*BmInfo.bmiHeader.biWidth;
		//if (j<0 || j >= BmInfo.bmiHeader.biWidth) continue;
		if ((kase + index)  < 0 && (kase + index) >= 640 * 480) {
			continue;
		}
		else {
			*(lpVHdr->lpData + 3 * (index + kase)) = 0;
			*(lpVHdr->lpData + 3 * (index + kase) + 1) = 255;
			*(lpVHdr->lpData + 3 * (index + kase) + 2) = 0;
		}
	}





	// ���̰� ���� ȭ���� ���� caption bar�� ǥ��
	CString  strTitle;
	strTitle.Format(_T("Binary Tracker (%d,%d)"), xCenter, yCenter);
	AfxGetMainWnd()->SetWindowText(strTitle);


	return (LRESULT)true;

}
