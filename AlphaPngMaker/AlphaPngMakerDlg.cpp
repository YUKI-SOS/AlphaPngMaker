
// AlphaPngMakerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "AlphaPngMaker.h"
#include "AlphaPngMakerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CAlphaPngMakerDlg 대화 상자



CAlphaPngMakerDlg::CAlphaPngMakerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ALPHAPNGMAKER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAlphaPngMakerDlg::AddToList(CString cs)
{
	CListBox* pList = (CListBox*)GetDlgItem(IDC_IMG_LIST);
	int nCount = 0;
	nCount = pList->GetCount();
	pList->InsertString(nCount, cs);
	pList->SetCurSel(nCount);
}

void CAlphaPngMakerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAlphaPngMakerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN_FILEDLG_BTN, &CAlphaPngMakerDlg::OnBnClickedOpenFiledlgBtn)
	ON_BN_CLICKED(IDC_LIST_OUT_BTN, &CAlphaPngMakerDlg::OnBnClickedListOutBtn)
	ON_BN_CLICKED(IDC_PREVIEW_BTN, &CAlphaPngMakerDlg::OnBnClickedPreviewBtn)
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
END_MESSAGE_MAP()


// CAlphaPngMakerDlg 메시지 처리기

BOOL CAlphaPngMakerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	CEdit* pRed = (CEdit*)GetDlgItem(IDC_RED_VAL);
	CEdit* pGreen = (CEdit*)GetDlgItem(IDC_GREEN_VAL);
	CEdit* pBlue = (CEdit*)GetDlgItem(IDC_BLUE_VAL);
	pRed->SetWindowTextW(L"235");
	pGreen->SetWindowTextW(L"235");
	pBlue->SetWindowTextW(L"235");

	bPrevDlg = false;

	m_pPreviewDlg = new CPreviewDlg();
	m_pPreviewDlg->Create(IDD_PREVIEW_DLG);
	m_pPreviewDlg->ShowWindow(SW_HIDE);


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CAlphaPngMakerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CAlphaPngMakerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CAlphaPngMakerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CAlphaPngMakerDlg::OnBnClickedOpenFiledlgBtn()
{
	CString dlgFormat = L"jpg or png|*.jpg;*.png";
	CString pathName;
	CString cslpstrFile;
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT, dlgFormat, this);

	//lpstrFile 이름 리턴
	//nMaxFile lpstrFile의 길이 최소 256
	const int maxFiles = 1000;
	const int buffSize = maxFiles * 256;

	fileDlg.m_ofn.lpstrFile = cslpstrFile.GetBuffer(buffSize);
	fileDlg.m_ofn.nMaxFile = buffSize;

	if (fileDlg.DoModal() == IDOK)
	{
		for (POSITION pos = fileDlg.GetStartPosition(); pos != NULL;)
		{
			pathName = fileDlg.GetNextPathName(pos);
			AddToList(pathName);
		}
	}
}


void CAlphaPngMakerDlg::OnBnClickedListOutBtn()
{
	CListBox* pList = (CListBox*)GetDlgItem(IDC_IMG_LIST);
	int nCount = pList->GetCount();
	int curSel = pList->GetCurSel();

	if (nCount > 0 && curSel != -1)
	{
		pList->DeleteString(curSel);
	}


}


void CAlphaPngMakerDlg::OnBnClickedPreviewBtn()
{
	CListBox* pList = (CListBox*)GetDlgItem(IDC_IMG_LIST);
	int curSel = pList->GetCurSel();
	if (curSel == -1)
		return;
	if (bPrevDlg == false)
	{
		bPrevDlg = true;
		pList->GetText(curSel, origFilePath);
		InitImg(origFilePath);
		EraseWhite(&changedPngImg, channels);
		m_pPreviewDlg->BitmapLoad();
		m_pPreviewDlg->ShowWindow(SW_SHOW);
	}
}

LRESULT CAlphaPngMakerDlg::OnKickIdle(WPARAM wParam, LPARAM lParam)
{
	if (bPrevDlg == true)
		m_pPreviewDlg->OnPaint();
	return LRESULT();
}


void CAlphaPngMakerDlg::GetColorVal()
{
	CEdit* pRed = (CEdit*)GetDlgItem(IDC_RED_VAL);
	CEdit* pGreen = (CEdit*)GetDlgItem(IDC_RED_VAL);
	CEdit* pBlue = (CEdit*)GetDlgItem(IDC_RED_VAL);

	CString csRed;
	CString csGreen;
	CString csBlue;

	pRed->GetWindowTextW(csRed);
	pGreen->GetWindowTextW(csGreen);
	pBlue->GetWindowTextW(csBlue);

	redValue = _ttoi(csRed);
	greenValue = _ttoi(csGreen);
	blueValue = _ttoi(csBlue);

}

int CAlphaPngMakerDlg::FormatChk(CString path)
{
	int nCount = path.ReverseFind('.');
	CString format = path.Mid(nCount + 1);
	if (format.CompareNoCase(L"jpg") == 0)
		return 1;
	if (format.CompareNoCase(L"png") == 0)
		return 2;
	return 0;
}

bool CAlphaPngMakerDlg::InitImg(CString path)
{
	m_pPreviewDlg->BitmapUnLoad();
	int format = FormatChk(path);
	if (format == 0)
		return false;
	if (format == 1)
	{
		if (!CopyFile(path, L"../temp/temp.jpg", false))
		{
			return false;
		}
		copyFilePath = L"../temp/temp.jpg";
		cv::Mat img;
		cv::Mat alphaImg;
		img = cv::imread("../temp/temp.jpg", cv::IMREAD_UNCHANGED);
		cv::cvtColor(img, alphaImg, cv::COLOR_RGB2RGBA);
		changedPngImg = alphaImg;
		cv::imwrite("../temp/temp.png", changedPngImg);

	}
	if (format == 2)
	{
		if (!CopyFile(path, L"../temp/temp.png", false))
		{
			return false;
		}
		copyFilePath = L"../temp/temp.png";
		cv::Mat img;
		img = cv::imread("../temp/temp.png", cv::IMREAD_UNCHANGED);
		if (img.channels() == 3)
		{
			cv::Mat alphaImg;
			cv::cvtColor(img, alphaImg, cv::COLOR_RGB2RGBA);
			changedPngImg = alphaImg;
		}
		if (img.channels() == 4)
		{
			changedPngImg = img;
		}
		cv::imwrite("../temp/temp.png", changedPngImg);

	}

	return true;

}


void CAlphaPngMakerDlg::EraseWhite(cv::Mat* img, cv::Mat* split)
{

	GetColorVal();
	int row = img->rows;
	int col = img->cols;
	cv::split(*img, split);

	for (int frow = 0; frow < row; frow++)
	{
		for (int fcol = 0; fcol < col; fcol++)
		{
			//이미지 최 외곽은 일단 날리고 상하좌우 찾기 힘드니까 넘기자
			if (frow == 0 || frow == row - 1 || fcol == 0 || fcol == col - 1)
			{
				//changedPngImg.at<cv::Vec4b>(frow, fcol)[3] = 0;
				split[3].at<uchar>(frow, fcol) = 0;
				continue;
			}
			//상하좌우 중 하나에 흰색범위가 아닌 픽셀이 있으면 알파 날리지 말고 넘기자.
			if (split[0].at<uchar>(frow - 1, fcol) < blueValue ||
				split[0].at<uchar>(frow + 1, fcol) < blueValue ||
				split[0].at<uchar>(frow, fcol - 1) < blueValue ||
				split[0].at<uchar>(frow, fcol + 1) < blueValue)
			{
				continue;
			}
			if (split[1].at<uchar>(frow - 1, fcol) < greenValue ||
				split[1].at<uchar>(frow + 1, fcol) < greenValue ||
				split[1].at<uchar>(frow, fcol - 1) < greenValue ||
				split[1].at<uchar>(frow, fcol + 1) < greenValue)
			{
				continue;
			}
			if (split[2].at<uchar>(frow - 1, fcol) < redValue ||
				split[2].at<uchar>(frow + 1, fcol) < redValue ||
				split[2].at<uchar>(frow, fcol - 1) < redValue ||
				split[2].at<uchar>(frow, fcol + 1) < redValue)
			{
				continue;
			}
			uchar pixelb = changedPngImg.at<cv::Vec4b>(frow, fcol)[0];
			uchar pixelg = changedPngImg.at<cv::Vec4b>(frow, fcol)[1];
			uchar pixelr = changedPngImg.at<cv::Vec4b>(frow, fcol)[2];
			if (pixelr >= redValue && pixelg >= greenValue && pixelb >= blueValue)
			{
				//changedPngImg.at<cv::Vec4b>(frow, fcol)[3] = 0;
				split[3].at<uchar>(frow, fcol) = 0;
			}
			uchar pixela = changedPngImg.at<cv::Vec4b>(frow, fcol)[3];

		}
	}

	//미리보기 위해서 temp폴더에 채널별 파일 생성.
	OutputChannels("../temp", split);

}

bool CAlphaPngMakerDlg::OutputChannels(std::string path, cv::Mat* ch)
{
	if (!cv::imwrite(path + "/tempredch.png", ch[2]))
		return false;
	if (!cv::imwrite(path + "/tempgreench.png", ch[1]))
		return false;
	if (!cv::imwrite(path + "/tempbluech.png", ch[0]))
		return false;
	if (!cv::imwrite(path + "/tempalphach.png", ch[3]))
		return false;

	return true;
}
