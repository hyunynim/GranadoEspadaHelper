﻿
// GranadoEspadaHelperDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "GranadoEspadaHelper.h"
#include "GranadoEspadaHelperDlg.h"
#include"CCaputreTool.h"
#include "afxdialogex.h"
#define DBUG
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGranadoEspadaHelperDlg 대화 상자


Mat gameSrc;			//게임 화면

CGranadoEspadaHelperDlg::CGranadoEspadaHelperDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GRANADOESPADAHELPER_DIALOG, pParent),
	hwndDesktop(::FindWindow(0, "Granado Espada"))
	, m_imgName(_T(""))
	, m_imgPath(_T(""))
	, m_imgCheckDuration(1000)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGranadoEspadaHelperDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMAGE_LIST, m_ImgList);
	DDX_Text(pDX, IDC_IMAGE_NAME, m_imgName);
	DDX_Text(pDX, IDC_IMAGE_PATH, m_imgPath);
	DDX_Text(pDX, IDC_IMAGE_CHECK_DURATION, m_imgCheckDuration);
}

BEGIN_MESSAGE_MAP(CGranadoEspadaHelperDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CGranadoEspadaHelperDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_FIND_IMAGE_PATH, &CGranadoEspadaHelperDlg::OnBnClickedFindImagePath)
	ON_BN_CLICKED(IDC_ADD_IMAGE, &CGranadoEspadaHelperDlg::OnBnClickedAddImage)
	ON_BN_CLICKED(IDC_SAVE_IMAGE_LIST, &CGranadoEspadaHelperDlg::OnBnClickedSaveImageList)
	ON_BN_CLICKED(IDC_DELETE_IMAGE, &CGranadoEspadaHelperDlg::OnBnClickedDeleteImage)
	ON_BN_CLICKED(IDC_CHECK_IMAGE_START, &CGranadoEspadaHelperDlg::OnBnClickedCheckImageStart)
	ON_BN_CLICKED(IDC_CHECK_IMAGE_STOP, &CGranadoEspadaHelperDlg::OnBnClickedCheckImageStop)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CAPTURE_TOOL, &CGranadoEspadaHelperDlg::OnBnClickedCaptureTool)
END_MESSAGE_MAP()


// CGranadoEspadaHelperDlg 메시지 처리기

BOOL CGranadoEspadaHelperDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	InitImgList();
	gameSrc = hwnd2mat(hwndDesktop);
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CGranadoEspadaHelperDlg::OnPaint()
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
HCURSOR CGranadoEspadaHelperDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*
InitImgList
return 불러온 이미지 개수
해당 image가 존재하는 경우에만 읽고 추가
*/
int CGranadoEspadaHelperDlg::InitImgList() {
	/*m_ImgList init*/
	CRect r;
	m_ImgList.GetWindowRect(&r);
	m_ImgList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);	//한 행 전체 선택
	m_ImgList.InsertColumn(0, "상태", LVCFMT_CENTER, r.Width() * 0.1);
	m_ImgList.InsertColumn(1, "이름", LVCFMT_CENTER, r.Width() * 0.2);
	m_ImgList.InsertColumn(2, "경로", LVCFMT_CENTER, r.Width() * 0.7);

	/*File read*/
	FILE* fp = fopen("imgList.txt", "r");
	FILE* tmpFp;
	if (fp == NULL)
		fp = fopen("imgList.txt", "w");
	char name[1010], path[1010];
	Mat tmpMat;
	while (~fscanf(fp, "%s %s", name, path)) {
		tmpFp = fopen(path, "r");
		if (tmpFp == NULL)continue;
		fclose(tmpFp);
		m_ImgList.InsertItem(imgList.size(), "OFF");
		m_ImgList.SetItem(imgList.size(), 1, LVIF_TEXT, name, 0, 0, 0, 0);
		m_ImgList.SetItem(imgList.size(), 2, LVIF_TEXT, path, 0, 0, 0, 0);
		tmpMat = imread(path, IMREAD_UNCHANGED);
		imgList.push_back({ name, path, tmpMat });
	}
	UpdateData(false);
	fclose(fp);
	return imgList.size();
}
void CGranadoEspadaHelperDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	Mat Inhance = imread("img.png", IMREAD_UNCHANGED), res;
	Mat expMax = imread("lvlMaster.png", IMREAD_UNCHANGED);
	Mat gray;
	Mat src = hwnd2mat(hwndDesktop);
	Mat dst = src(Range(900, 1080), Range(0, 765));
	imshow("output", dst);
	
	//CDialogEx::OnOK();
}


void CGranadoEspadaHelperDlg::OnBnClickedFindImagePath()
{
	UpdateData(TRUE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static TCHAR BASED_CODE szFilter[] = "이미지 파일(*.BMP, *.GIF, *.JPG, *.PNG) | *.BMP;*.GIF;*.JPG;*.PNG;*.bmp;*.jpg;*.gif;*.png |모든파일(*.*)|*.*||";
	CFileDialog dlg(TRUE, "*.png", "image", OFN_HIDEREADONLY, szFilter);
	if (IDOK == dlg.DoModal()){
		m_imgPath = dlg.GetPathName();
		UpdateData(FALSE);
	}
}


void CGranadoEspadaHelperDlg::OnBnClickedAddImage()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_imgName == "" || m_imgPath == "")
		MessageBox("해당 이미지의 이름 또는 경로를 확인해주세요.");
	else {
		char pathTmp[1010];
		sprintf(pathTmp, "%s", m_imgPath);
		Mat tmpMat = imread(pathTmp, IMREAD_UNCHANGED);
		m_ImgList.InsertItem(imgList.size(), "OFF");
		m_ImgList.SetItem(imgList.size(), 1, LVIF_TEXT, m_imgName, 0, 0, 0, 0);
		m_ImgList.SetItem(imgList.size(), 2, LVIF_TEXT, m_imgPath, 0, 0, 0, 0);
		imgList.push_back({ m_imgName, m_imgPath, tmpMat });
		MessageBox("이미지 추가 완료");
		m_imgName = m_imgPath = "";
		UpdateData(FALSE);
	}
}


void CGranadoEspadaHelperDlg::OnBnClickedSaveImageList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	FILE* fp = fopen("imgList.txt", "w");
	for (int i = 0; i < imgList.size(); ++i)
		fprintf(fp, "%s %s\n", imgList[i].name, imgList[i].path);
	fclose(fp);
	MessageBox("이미지 리스트가 저장되었습니다.");
}


void CGranadoEspadaHelperDlg::OnBnClickedDeleteImage()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	POSITION p;
	p = m_ImgList.GetFirstSelectedItemPosition();
	int i = m_ImgList.GetNextSelectedItem(p);
	m_ImgList.DeleteItem(i);
	imgList.erase(imgList.begin() + i);
	MessageBox("해당 이미지가 삭제되었습니다.");
}


void CGranadoEspadaHelperDlg::OnBnClickedCheckImageStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	SetTimer(imageCheckTimer, m_imgCheckDuration, 0);
}


void CGranadoEspadaHelperDlg::OnBnClickedCheckImageStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KillTimer(imageCheckTimer);
}


void CGranadoEspadaHelperDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	char msg[1010];
	switch (nIDEvent) {
	case imageCheckTimer:
		gameSrc = hwnd2mat(hwndDesktop);
		for (int i = 0; i < imgList.size(); ++i) {
			if (FindImage(imgList[i].img, gameSrc)) {
				m_ImgList.SetItem(i, 0, LVIF_TEXT, "ON", 0, 0, 0, 0);
#ifdef DBUG
				if (i == 1 || i >= 3)
					bark();
#endif
			}
			else {
#ifdef DBUG
				if (i == 0)
					bark();
#endif
				m_ImgList.SetItem(i, 0, LVIF_TEXT, "OFF", 0, 0, 0, 0);
			}
		}
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}
/*
FindImage 
gameSrc로부터 templ 이미지를 검색
result는 결과값 계산을 위한 temporary matrix
dstImage는 gameSrc로부터 찾은 영역에 rect처리 된 matrix
오차 비율(ratio)에 따라 해당 이미지가 존재하는지 여부 판단
*/
bool CGranadoEspadaHelperDlg::FindImage(Mat& templ, Mat& dstImage) {
	const double ratio = 1.5;
	double minVal, maxVal;
	Point minLoc, maxLoc;
	matchTemplate(gameSrc, templ, tmp, TM_SQDIFF);
	minMaxLoc(tmp, &minVal, NULL, &minLoc, NULL);
	Mat tmp = gameSrc(Range(minLoc.y, minLoc.y + templ.rows), Range(minLoc.x, minLoc.x + templ.cols));

	rectangle(dstImage, minLoc,
		Point(minLoc.x + templ.cols, minLoc.y + templ.rows), Scalar(0, 255, 0), 2);
	int cnt = 0;
	for (int i = 0; i < tmp.rows; ++i)
		for (int j = 0; j < tmp.cols; ++j)
			if (tmp.at<Vec3b>(i, j) == templ.at<Vec3b>(i, j))
				++cnt;
	int sz = tmp.rows * tmp.cols;
	if (sz - cnt <= sz / ratio) return 1;
	return 0;
}

void CGranadoEspadaHelperDlg::OnBnClickedCaptureTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CCaputreTool dlg;
	dlg.DoModal();
}
