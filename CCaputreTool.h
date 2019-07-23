#pragma once


// CCaputreTool 대화 상자

class CCaputreTool : public CDialogEx
{
	DECLARE_DYNAMIC(CCaputreTool)

public:
	CCaputreTool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CCaputreTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAPTURE_TOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	//Var
	HWND gameHwnd;
	Mat res;		//캡처할 영역
	//FUNC
	void ShowCaptureArea();
	int m_sizeX;
	int m_sizeY;
	afx_msg void OnBnClickedSizeYUp();
	afx_msg void OnBnClickedSizeYDown();
	afx_msg void OnBnClickedSizeXDown();
	afx_msg void OnBnClickedSizeXUp();
	int m_areaX;
	int m_areaY;
	afx_msg void OnBnClickedAreaYUp();
	afx_msg void OnBnClickedAreaYDown();
	afx_msg void OnBnClickedAreaXDown();
	afx_msg void OnBnClickedAreaXUp();
	CString m_fileName;
	afx_msg void OnBnClickedSaveImage();
};
