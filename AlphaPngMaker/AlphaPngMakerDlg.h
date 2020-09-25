
// AlphaPngMakerDlg.h: 헤더 파일
//

#pragma once
#include "CPreviewDlg.h"
#include <afxpriv.h>
#include <string>
// CAlphaPngMakerDlg 대화 상자
class CAlphaPngMakerDlg : public CDialogEx
{
// 생성입니다.
public:
	CAlphaPngMakerDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

public:
	void AddToList(CString cs);
	void GetColorVal();
	int FormatChk(CString path);
	//temp폴더로 이미지 복사 및 24비트 jpg면 32비트 png로 변환까지
	bool InitImg(CString path);
	void EraseWhite(cv::Mat* img, cv::Mat* split);
	void EraseWhite_BlueCh(cv::Mat* img, cv::Mat* split);
	bool OutputChannels(std::string path, cv::Mat* split);
	bool OutputImage(std::string path, cv::Mat* split);
	CPreviewDlg* m_pPreviewDlg;
	bool bPrevDlg;

	CString origFilePath;
	CString copyFilePath;

	int redValue;
	int greenValue;
	int blueValue;

	cv::Mat changedPngImg;
	cv::Mat channels[4];
	cv::Mat alphaCh;
	cv::Mat redCh;
	cv::Mat greenCh;
	cv::Mat blueCh;



// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ALPHAPNGMAKER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpenFiledlgBtn();
	afx_msg void OnBnClickedListOutBtn();
	afx_msg void OnBnClickedPreviewBtn();
	afx_msg LRESULT OnKickIdle(WPARAM wParam, LPARAM lParam);

};
