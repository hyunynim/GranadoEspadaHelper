
// GranadoEspadaHelperDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "GranadoEspadaHelper.h"
#include "GranadoEspadaHelperDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGranadoEspadaHelperDlg 대화 상자



CGranadoEspadaHelperDlg::CGranadoEspadaHelperDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GRANADOESPADAHELPER_DIALOG, pParent),
	hwndDesktop(::FindWindow(0, "Granado Espada"))
	, m_imgName(_T(""))
	, m_imgPath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGranadoEspadaHelperDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMAGE_LIST, m_ImgList);
	DDX_Text(pDX, IDC_IMAGE_NAME, m_imgName);
	DDX_Text(pDX, IDC_IMAGE_PATH, m_imgPath);
}

BEGIN_MESSAGE_MAP(CGranadoEspadaHelperDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CGranadoEspadaHelperDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_FIND_IMAGE_PATH, &CGranadoEspadaHelperDlg::OnBnClickedFindImagePath)
	ON_BN_CLICKED(IDC_ADD_IMAGE, &CGranadoEspadaHelperDlg::OnBnClickedAddImage)
	ON_BN_CLICKED(IDC_SAVE_IMAGE_LIST, &CGranadoEspadaHelperDlg::OnBnClickedSaveImageList)
	ON_BN_CLICKED(IDC_DELETE_IMAGE, &CGranadoEspadaHelperDlg::OnBnClickedDeleteImage)
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

Mat CGranadoEspadaHelperDlg::hwnd2mat(HWND hwnd) {
	HDC hwindowDC, hwindowCompatibleDC;

	int height, width, srcheight, srcwidth;
	HBITMAP hbwindow;
	Mat src;
	BITMAPINFOHEADER  bi;

	hwindowDC = ::GetDC(hwnd);
	hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
	SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

	RECT windowsize;    // get the height and width of the screen
	::GetClientRect(hwnd, &windowsize);

	srcheight = windowsize.bottom;
	srcwidth = windowsize.right;
	height = windowsize.bottom / 1;  //change this to whatever size you want to resize to
	width = windowsize.right / 1;

	src.create(height, width, CV_8UC4);

	// create a bitmap
	hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
	bi.biSize = sizeof(BITMAPINFOHEADER);    //http://msdn.microsoft.com/en-us/library/windows/window/dd183402%28v=vs.85%29.aspx
	bi.biWidth = width;
	bi.biHeight = -height;  //this is the line that makes it draw upside down or not
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	// use the previously created device context with the bitmap
	SelectObject(hwindowCompatibleDC, hbwindow);
	// copy from the window device context to the bitmap device context
	StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, 0, 0, srcwidth, srcheight, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
	GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO*)& bi, DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow

	// avoid memory leak
	DeleteObject(hbwindow);
	DeleteDC(hwindowCompatibleDC);
	::ReleaseDC(hwnd, hwindowDC);

	return src;
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
	Mat* tmpMat;
	while (~fscanf(fp, "%s %s", name, path)) {
		tmpFp = fopen(path, "r");
		if (tmpFp == NULL)continue;
		fclose(tmpFp);
		m_ImgList.InsertItem(imgList.size(), "OFF");
		m_ImgList.SetItem(imgList.size(), 1, LVIF_TEXT, name, 0, 0, 0, 0);
		m_ImgList.SetItem(imgList.size(), 2, LVIF_TEXT, path, 0, 0, 0, 0);
		tmpMat = new Mat();
		*tmpMat = imread(path, IMREAD_UNCHANGED);
		imgList.push_back({ name, path, *tmpMat });
		delete tmpMat;
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
	static TCHAR BASED_CODE szFilter[] = "이미지 파일(*.BMP, *.GIF, *.JPG, *.PNG) | *.BMP;*.GIF;*.JPG;*.bmp;*.jpg;*.gif;*.png |모든파일(*.*)|*.*||";
	CFileDialog dlg(TRUE, "*.jpg", "image", OFN_HIDEREADONLY, szFilter);
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
		m_ImgList.InsertItem(imgList.size(), "OFF");
		m_ImgList.SetItem(imgList.size(), 1, LVIF_TEXT, m_imgName, 0, 0, 0, 0);
		m_ImgList.SetItem(imgList.size(), 2, LVIF_TEXT, m_imgPath, 0, 0, 0, 0);
		imgList.push_back({ m_imgName, m_imgPath, imread((LPCSTR)m_imgPath, IMREAD_UNCHANGED) });
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
