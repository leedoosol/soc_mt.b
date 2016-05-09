
// webcamV1View.cpp : CwebcamV1View Ŭ������ ����
//

#include "stdafx.h"
#include <string.h>
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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

// CwebcamV1View ����/�Ҹ�

CwebcamV1View::CwebcamV1View()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.


}

CwebcamV1View::~CwebcamV1View()
{
}

BOOL CwebcamV1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CwebcamV1View �׸���

void CwebcamV1View::OnDraw(CDC* /*pDC*/)
{
	CwebcamV1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;


	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
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


// CwebcamV1View ����

#ifdef _DEBUG
void CwebcamV1View::AssertValid() const
{
	CView::AssertValid();
}

void CwebcamV1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CwebcamV1Doc* CwebcamV1View::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CwebcamV1Doc)));
	return (CwebcamV1Doc*)m_pDocument;
}
#endif //_DEBUG


// CwebcamV1View �޽��� ó����
