
// webcamV1View.cpp : CwebcamV1View 클래스의 구현
//

#include "stdafx.h"
#include <string.h>
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "webcamV1.h"
#endif

#include "webcamV1Doc.h"
#include "webcamV1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CwebcamV1View

IMPLEMENT_DYNCREATE(CwebcamV1View, CView)

BEGIN_MESSAGE_MAP(CwebcamV1View, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CwebcamV1View 생성/소멸

CwebcamV1View::CwebcamV1View()
{
	// TODO: 여기에 생성 코드를 추가합니다.


}

CwebcamV1View::~CwebcamV1View()
{
}

BOOL CwebcamV1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CwebcamV1View 그리기

void CwebcamV1View::OnDraw(CDC* /*pDC*/)
{
	CwebcamV1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;


	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}

void CwebcamV1View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CwebcamV1View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CwebcamV1View 진단

#ifdef _DEBUG
void CwebcamV1View::AssertValid() const
{
	CView::AssertValid();
}

void CwebcamV1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CwebcamV1Doc* CwebcamV1View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CwebcamV1Doc)));
	return (CwebcamV1Doc*)m_pDocument;
}
#endif //_DEBUG


// CwebcamV1View 메시지 처리기
