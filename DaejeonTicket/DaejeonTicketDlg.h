
// DaejeonTicketDlg.h : ��� ����
//

#pragma comment(lib, "vfw32.lib")

#include "vfw.h"
#include "afxwin.h"

#define    _DEF_WEBCAM        1000

LRESULT CALLBACK CallbackOnFrame(HWND hWnd, LPVIDEOHDR lpVHdr);
LRESULT CALLBACK CallbackOnFrame2(HWND hWnd, LPVIDEOHDR lpVHdr);

// CDaejeonTicketDlg ��ȭ ����
class CDaejeonTicketDlg : public CDialogEx
{
	void OnDestroy();
// �����Դϴ�.
public:
//	afx_msg void OnBnClickedOk();

	HWND m_Cap;
//	CStatic m_stDisplay;
	CDaejeonTicketDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DAEJEONTICKET_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	HWND m_Cap2;
	HWND m_Cap3;
	afx_msg void OnBnClickedOk6();
	afx_msg void OnBnClickedOk3();
	afx_msg void OnBnClickedOk4();
	afx_msg void OnBnClickedOk5();
};
