#pragma once
#include "SREngine.h"


// CPreviewDlg 대화 상자

class CPreviewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPreviewDlg)

public:
	CPreviewDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CPreviewDlg();

	void Render();
	void ClearWindow();
	void DrawOrigImg();
	void DrawAlphaCh();
	void BitmapLoad();
	void BitmapLoad(CString path, ID2D1Bitmap** bitmap);
	void BitmapUnLoad();

public:
	IDREngine* m_SREngine;

	ID2D1Bitmap* origBitmap;
	ID2D1Bitmap* alphaChBitmap;
	ID2D1Bitmap* redChBitmap;
	ID2D1Bitmap* greenChBitmap;
	ID2D1Bitmap* blueChBitmap;


// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PREVIEW_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.


	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnPaint();
	afx_msg void OnClose();
};
