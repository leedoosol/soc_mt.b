
// DaejeonTicketDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "DaejeonTicket.h"
#include "DaejeonTicketDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <Windows.h>
using namespace std;

#define COL 640
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

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
	m_Cap = capCreateCaptureWindow(TEXT("original_image"), WS_CHILD
		| WS_VISIBLE, 0, 0, 640, 480, this->m_hWnd, NULL);


	// �ݹ��Լ� ����
	if (capSetCallbackOnFrame(m_Cap, CallbackOnFrame) == FALSE) {
		return FALSE;
	}


	// ī�޶� ����̹��� ����
	if (capDriverConnect(m_Cap, 0) == FALSE) {
		return FALSE;
	}
	

	// ī�޶� ����
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
	*/
	double fB, fG, fR, fH, fS, fV;
	int  i, j, index, counter = 0;
	pImgBuffer = (LPBYTE)new BYTE[BmInfo.bmiHeader.biHeight*BmInfo.bmiHeader.biWidth];

	for (i = 0; i < BmInfo.bmiHeader.biWidth*BmInfo.bmiHeader.biHeight; i++) {
		fB = *(lpVHdr->lpData + (i * 3)) / 255.0;
		fG = *(lpVHdr->lpData + (i * 3) + 1) / 255.0;
		fR = *(lpVHdr->lpData + (i * 3) + 2) / 255.0;

		double fCMax = MAX(MAX(fR, fG), fB);
		double fCMin = MIN(MIN(fR, fG), fB);
		double fDelta = fCMax - fCMin;

		if (fDelta > 0) {
			if (fCMax == fR) {
				fH = 60 * (fmod(((fG - fB) / fDelta), 6));
			}
			else if (fCMax == fG) {
				fH = 60 * (((fB - fR) / fDelta) + 2);
			}
			else if (fCMax == fB) {
				fH = 60 * (((fR - fG) / fDelta) + 4);
			}

			if (fCMax > 0) {
				fS = fDelta / fCMax;
			}
			else {
				fS = 0;
			}

			fV = fCMax;
		}
		else {
			fH = 0;
			fS = 0;
			fV = fCMax;
		}

		if (fH < 0) {
			fH = 360 + fH;
		}

		//printf("H:%.2lf | S:%.2lf | V:%.2lf\n", fH, fS, fV);
		//Sleep(1500);

		char option;
		//detect blue
		if ((fH >= 210 && fH <= 270) && (fS >= 0.2 && fS <= 1) && (fV >= 0.5 && fV <= 1)) {
			*(pImgBuffer + i) = 255;
		}
		//else if ((fH >= 30 && fH <= 90) && (fS >= 0.2 && fS <= 1) && (fV >= 0.5 && fV <= 1)) {
		//	*(pImgBuffer + i) = 255;
		//}
		//else if ((fH >= 210 && fH <= 270) && (fS >= 0.2 && fS <= 1) && (fV >= 0.5 && fV <= 1)) {
		//	*(pImgBuffer + i) = 255;
		//}
		else *(pImgBuffer + i) = 0;
	}

	//double pixel_b, pixel_g, pixel_r, var_Min, var_Max, del_Max;
	//double del_R, del_G, del_B;
	//int  i, j, index, counter = 0;

	//double H, S, V;

	//if (pImgBuffer == NULL)
	//	pImgBuffer = (LPBYTE)new BYTE[BmInfo.bmiHeader.biHeight*BmInfo.bmiHeader.biWidth];

	//for (i = 0; i < BmInfo.bmiHeader.biWidth*BmInfo.bmiHeader.biHeight; i++)
	//{
	//	pixel_b = *(lpVHdr->lpData + (i * 3)) / 255.0;
	//	pixel_g = *(lpVHdr->lpData + (i * 3) + 1) / 255.0;
	//	pixel_r = *(lpVHdr->lpData + (i * 3) + 2) / 255.0;

	//	printf("%lf | %lf | %lf \n", pixel_b, pixel_g, pixel_r);

	//	var_Min = MIN(MIN(pixel_b, pixel_g), pixel_r);
	//	var_Max = MAX(MAX(pixel_b, pixel_g), pixel_r);
	//	del_Max = var_Max - var_Min;
	//	printf("var_Min : %lf\n", var_Min);
	//	printf("var_Max : %lf\n", var_Max);
	//	printf("delmax : %lf\n", del_Max);



	//	V = var_Max;
	//	printf("V : %lf\n", V);

	//	if (del_Max == 0)
	//	{
	//		H = S = 0;
	//	}
	//	else
	//	{
	//		S = del_Max / var_Max;
	//		printf("S : %lf\n", S);

	//		del_R = (((var_Max - pixel_r) / 6) + (del_Max / 2)) / del_Max;
	//		del_G = (((var_Max - pixel_g) / 6) + (del_Max / 2)) / del_Max;
	//		del_B = (((var_Max - pixel_b) / 6) + (del_Max / 2)) / del_Max;

	//		if (pixel_r == var_Max) H = del_B - del_G;
	//		else if (pixel_g == var_Max) H = (1 / 3) + del_R - del_B;
	//		else if (pixel_b == var_Max) H = (2 / 3) + del_G - del_R;

	//		if (H < 0) H += 1;
	//		if (H > 1) H -= 1;
	//	}
	//	printf("H : %lf\n", H);

	//	Sleep(1500);
	//	if ((H >= 210 && H <= 270) && (S >= 0.2 && S <= 1) && (V >= 0 && V <= 1)) {
	//		*(pImgBuffer + i) = 255;
	//	}
	//	cout << endl;

	//}


	/*for (i = 0; i <BmInfo.bmiHeader.biWidth*BmInfo.bmiHeader.biHeight; i++)
	{
		pixel_b = *(lpVHdr->lpData + (i * 3));
		pixel_g = *(lpVHdr->lpData + (i * 3) + 1);
		pixel_r = *(lpVHdr->lpData + (i * 3) + 2);


		if (pixel_b > 70&& pixel_g + pixel_r < 10) {
			*(pImgBuffer + i) = 255;
			printf("%d || %d || %d\n", pixel_b, pixel_g, pixel_r);
		}
		else if (pixel_b > 100 && pixel_g + pixel_r < 150 && pixel_r-pixel_g > 20) {
			*(pImgBuffer + i) = 255;
				printf("%d || %d || %d\n", pixel_b, pixel_g, pixel_r);
		}
		else if (pixel_b > 50 && pixel_g + pixel_r < 10) {
			*(pImgBuffer + i) = 255;
				printf("%d || %d || %d\n", pixel_b, pixel_g, pixel_r);
		}
		else  if (pixel_b > 200 && pixel_g + pixel_r < 200) {
			*(pImgBuffer + i) = 255;
			printf("%d || %d || %d\n", pixel_b, pixel_g, pixel_r);
		}
		else  if (pixel_b > 150 && pixel_g + pixel_r < 100) {
			*(pImgBuffer + i) = 255;
			printf("%d || %d || %d\n", pixel_b, pixel_g, pixel_r);
		}
		else  if (pixel_b > 100 && pixel_g + pixel_r < 50) {
			*(pImgBuffer + i) = 255;
			printf("%d || %d || %d\n", pixel_b, pixel_g, pixel_r);
		}
		else  if (pixel_b > 50 && pixel_g + pixel_r < 10) {
			*(pImgBuffer + i) = 255;
			printf("%d || %d || %d\n", pixel_b, pixel_g, pixel_r);
		}
		else *(pImgBuffer + i) = 0;
	}*/

	//Sleep(1500);
	//for (i = 0; i <BmInfo.bmiHeader.biWidth*BmInfo.bmiHeader.biHeight; i++)
	//{
	//	pixel =
	//		(*(lpVHdr->lpData + (i * 3)) +
	//			*(lpVHdr->lpData + (i * 3) + 1) +
	//			*(lpVHdr->lpData + (i * 3) + 2)) / 3;
	//	if (pixel > 200) *(pImgBuffer + i) = 255;
	//	else *(pImgBuffer + i) = 0;
	//}

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
	
	//??
	//int kasd = 0;
	//kasd = (float)xCenter + (int)30;
	//int kase = 0;
	//kase = (float)xCenter - (int)30;


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

	//
	//for (j = yCenter - 30; j <= yCenter + 30; j++)
	//{
	//	int kase = 0;
	//	kase = xCenter + 30;
	//	index = kase*BmInfo.bmiHeader.biWidth;
	//	if ((j + index)  < 0 && (j + index) >= 640 * 480) {
	//		continue;
	//	}
	//	else {
	//		*(lpVHdr->lpData + 3 * (index + j)) = 0;
	//		*(lpVHdr->lpData + 3 * (index + j) + 1) = 255;
	//		*(lpVHdr->lpData + 3 * (index + j) + 2) = 0;
	//	}
	//	//if (j<0 || j >= BmInfo.bmiHeader.biWidth) continue;

	//}

	//
	//for (j = yCenter - 30; j <= yCenter + 30; j++)
	//{
	//	int kase = 0;
	//	kase = xCenter - 30;
	//	index = kase*BmInfo.bmiHeader.biWidth;
	//	//if (j<0 || j >= BmInfo.bmiHeader.biWidth) continue;
	//	if ( (j+index)  < 0 &&  (j+index) >= 640 * 480) {
	//		continue;
	//	}
	//	else {
	//		*(lpVHdr->lpData + 3 * (index + j)) = 0;
	//		*(lpVHdr->lpData + 3 * (index + j) + 1) = 255;
	//		*(lpVHdr->lpData + 3 * (index + j) + 2) = 0;
	//	}
	//}
	//
	//for (j = xCenter - 30; j <= xCenter + 30; j++)
	//{
	//	int kase = 0;
	//	kase = yCenter - 30;

	//	index = j*BmInfo.bmiHeader.biWidth;
	//	//if (j<0 || j >= BmInfo.bmiHeader.biWidth) continue;
	//	if ((kase + index)  < 0 && (kase + index) >= 640 * 480) {
	//		continue;
	//	}
	//	else {
	//		*(lpVHdr->lpData + 3 * (index + kase)) = 0;
	//		*(lpVHdr->lpData + 3 * (index + kase) + 1) = 255;
	//		*(lpVHdr->lpData + 3 * (index + kase) + 2) = 0;
	//	}
	//}


	//for (j = xCenter - 30; j <= xCenter + 30; j++)
	//{
	//	int kase = 0;
	//	kase = yCenter + 30;

	//	index = j*BmInfo.bmiHeader.biWidth;
	//	//if (j<0 || j >= BmInfo.bmiHeader.biWidth) continue;
	//	if ((kase + index)  < 0 && (kase + index) >= 640 * 480) {
	//		continue;
	//	}
	//	else {
	//		*(lpVHdr->lpData + 3 * (index + kase)) = 0;
	//		*(lpVHdr->lpData + 3 * (index + kase) + 1) = 255;
	//		*(lpVHdr->lpData + 3 * (index + kase) + 2) = 0;
	//	}
	//}

	


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

	// ���̰� ���� ȭ���� ���� caption bar�� ǥ��
	CString  strTitle;
	//strTitle.Format(_T("Binary Tracker (%d,%d)"), xCenter, yCenter);
	AfxGetMainWnd()->SetWindowText(strTitle);

	return (LRESULT)true;

}

