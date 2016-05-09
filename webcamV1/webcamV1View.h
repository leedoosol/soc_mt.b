
// webcamV1View.h : CwebcamV1View Ŭ������ �������̽�
//

#pragma once


class CwebcamV1View : public CView
{
protected: // serialization������ ��������ϴ�.
	CwebcamV1View();
	DECLARE_DYNCREATE(CwebcamV1View)

// Ư���Դϴ�.
public:
	CwebcamV1Doc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// �����Դϴ�.
public:
	virtual ~CwebcamV1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // webcamV1View.cpp�� ����� ����
inline CwebcamV1Doc* CwebcamV1View::GetDocument() const
   { return reinterpret_cast<CwebcamV1Doc*>(m_pDocument); }
#endif

