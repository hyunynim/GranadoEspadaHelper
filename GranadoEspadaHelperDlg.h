
// GranadoEspadaHelperDlg.h: 헤더 파일
//

#pragma once


// CGranadoEspadaHelperDlg 대화 상자
class CGranadoEspadaHelperDlg : public CDialogEx
{
// 생성입니다.
public:
	CGranadoEspadaHelperDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GRANADOESPADAHELPER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	//Var
	struct IMAGE {
		CString name, path;
		Mat img;
	};
	HWND hwndDesktop;		//화면 받아오기
	vector<IMAGE> imgList;	//켜진 버프를 확인할 이미지 리스트
	//Func
	int InitImgList();		//프로그램 실행 시 저장된 이미지 리스트 불러오기
	Mat hwnd2mat(HWND hwnd);
	afx_msg void OnBnClickedOk();
	CListCtrl m_ImgList;
	CString m_imgName;
	CString m_imgPath;
	afx_msg void OnBnClickedFindImagePath();
	afx_msg void OnBnClickedAddImage();
	afx_msg void OnBnClickedSaveImageList();
	afx_msg void OnBnClickedDeleteImage();
};
