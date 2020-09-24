// CPreviewDlg.cpp: 구현 파일
//

#include "pch.h"
#include "AlphaPngMaker.h"
#include "AlphaPngMakerDlg.h"
#include "CPreviewDlg.h"
#include "afxdialogex.h"


// CPreviewDlg 대화 상자

IMPLEMENT_DYNAMIC(CPreviewDlg, CDialogEx)

CPreviewDlg::CPreviewDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PREVIEW_DLG, pParent)
{
	CAlphaPngMakerDlg* pMainWnd = (CAlphaPngMakerDlg*)AfxGetMainWnd();

	m_SREngine = new SREngine();

	origBitmap = nullptr;
	alphaChBitmap = nullptr;
	redChBitmap = nullptr;
	greenChBitmap = nullptr;
	blueChBitmap = nullptr;

}

CPreviewDlg::~CPreviewDlg()
{
}

void CPreviewDlg::Render()
{
	m_SREngine->BeginRender();
	ClearWindow();

	DrawOrigImg();
	DrawAlphaCh();

	m_SREngine->EndRender();


}

void CPreviewDlg::ClearWindow()
{
	CRect winPos;
	GetWindowRect(winPos);
	ScreenToClient(winPos);
	m_SREngine->DrawFillRectangle(winPos.left, winPos.top, winPos.right, winPos.bottom, RGB(240, 240, 240));

}




void CPreviewDlg::DrawOrigImg()
{
	CRect origPos;
	GetDlgItem(IDC_ORIG_POS)->GetWindowRect(origPos);
	ScreenToClient(origPos);

	if (origBitmap != nullptr)
		m_SREngine->DrawImage(origPos.left, origPos.top, origPos.right - origPos.left, origPos.bottom - origPos.top, origBitmap);
}

void CPreviewDlg::DrawAlphaCh()
{
	CRect alphaPos;
	GetDlgItem(IDC_ALPHA_POS)->GetWindowRect(alphaPos);
	ScreenToClient(alphaPos);

	if (alphaChBitmap != nullptr)
		m_SREngine->DrawImage(alphaPos.left, alphaPos.top, alphaPos.right - alphaPos.left, alphaPos.bottom - alphaPos.top, alphaChBitmap);
}

void CPreviewDlg::BitmapLoad()
{
	CAlphaPngMakerDlg* pMainWnd = (CAlphaPngMakerDlg*)AfxGetMainWnd();

	BitmapLoad(pMainWnd->copyFilePath, &origBitmap);
	BitmapLoad(L"../temp/tempalphach.png", &alphaChBitmap);
	BitmapLoad(L"../temp/tempredch.png", &redChBitmap);
	BitmapLoad(L"../temp/tempgreench.png", &greenChBitmap);
	BitmapLoad(L"../temp/tempbluech.png", &blueChBitmap);

}

void CPreviewDlg::BitmapLoad(CString path, ID2D1Bitmap** bitmap)
{
	m_SREngine->LoadMyBitMap(path, 0, 0, bitmap);
}

void CPreviewDlg::BitmapUnLoad()
{
	if(origBitmap != nullptr)
	SafeRelease(&origBitmap);
	if(alphaChBitmap != nullptr)
	SafeRelease(&alphaChBitmap);
	if(redChBitmap != nullptr)
	SafeRelease(&redChBitmap);
	if(greenChBitmap != nullptr)
	SafeRelease(&greenChBitmap);
	if(blueChBitmap != nullptr)
	SafeRelease(&blueChBitmap);
}

void CPreviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPreviewDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CPreviewDlg 메시지 처리기


BOOL CPreviewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	RECT rc;

	::GetClientRect(m_hWnd, &rc);
	int screenWidth = rc.right - rc.left;
	int screenHeight = rc.bottom - rc.top;

	m_SREngine->Initialize(AfxGetInstanceHandle(), m_hWnd, screenWidth, screenHeight);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}




void CPreviewDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	Render();

}


void CPreviewDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CAlphaPngMakerDlg* pMainWnd = (CAlphaPngMakerDlg*)AfxGetMainWnd();
	pMainWnd->bPrevDlg = false;
	BitmapUnLoad();
	this->ShowWindow(SW_HIDE);

	CDialogEx::OnClose();
}
