#include "stdafx.h"
#include "CamTest.h"
#include "Vfw.h"
#include "webcamV1Doc.h"
#include "webcamV1View.h"

CCamTest::CCamTest()
{
	CwebcamV1Doc* pDoc;
	CwebcamV1View* pView;
}


CCamTest::~CCamTest()
{
}


void CCamTest::InitCam()
{
	BITMAPINFO BmInfo;
	m_hCam = capCreateCaptureWindow((LPCWCHAR)"Capture Window", WS_CHILD | WS_VISIBLE,
		20, 20, 320, 240, this->m_hWnd, NULL);
	if (capDriverConnect(m_hCam, 0) == 0) {
		return;
	}
	capGetVideoFormat(m_hCam, &BmInfo, sizeof(BITMAPINFO));


}
