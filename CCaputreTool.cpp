// CCaputreTool.cpp: 구현 파일
//

#include "pch.h"
#include "GranadoEspadaHelper.h"
#include "GranadoEspadaHelperDlg.h"
#include "CCaputreTool.h"
#include "afxdialogex.h"

extern Mat gameSrc;
const int minSizeX = 10;
const int minSizeY = 10;
// CCaputreTool 대화 상자

IMPLEMENT_DYNAMIC(CCaputreTool, CDialogEx)

CCaputreTool::CCaputreTool(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CAPTURE_TOOL, pParent),
	gameHwnd(::FindWindow(0, "Granado Espada"))
	, m_sizeX(10)
	, m_sizeY(10)
	, m_areaX(0)
	, m_areaY(0)
	, m_fileName(_T(""))
{

}

CCaputreTool::~CCaputreTool()
{
}

void CCaputreTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SIZE_X, m_sizeX);
	DDX_Text(pDX, IDC_SIZE_Y, m_sizeY);
	DDX_Text(pDX, IDC_SIZE_X2, m_areaX);
	DDX_Text(pDX, IDC_SIZE_Y2, m_areaY);
	DDX_Text(pDX, IDC_FILENAME, m_fileName);
}


BEGIN_MESSAGE_MAP(CCaputreTool, CDialogEx)
	ON_BN_CLICKED(IDC_SIZE_Y_UP, &CCaputreTool::OnBnClickedSizeYUp)
	ON_BN_CLICKED(IDC_SIZE_Y_DOWN, &CCaputreTool::OnBnClickedSizeYDown)
	ON_BN_CLICKED(IDC_SIZE_X_DOWN, &CCaputreTool::OnBnClickedSizeXDown)
	ON_BN_CLICKED(IDC_SIZE_X_UP, &CCaputreTool::OnBnClickedSizeXUp)
	ON_BN_CLICKED(IDC_AREA_Y_UP, &CCaputreTool::OnBnClickedAreaYUp)
	ON_BN_CLICKED(IDC_AREA_Y_DOWN, &CCaputreTool::OnBnClickedAreaYDown)
	ON_BN_CLICKED(IDC_AREA_X_DOWN, &CCaputreTool::OnBnClickedAreaXDown)
	ON_BN_CLICKED(IDC_AREA_X_UP, &CCaputreTool::OnBnClickedAreaXUp)
	ON_BN_CLICKED(IDC_SAVE_IMAGE, &CCaputreTool::OnBnClickedSaveImage)
END_MESSAGE_MAP()


// CCaputreTool 메시지 처리기


void CCaputreTool::OnBnClickedSizeYUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_sizeY < gameSrc.rows)
		++m_sizeY;
	UpdateData(FALSE);
	ShowCaptureArea();
}


void CCaputreTool::OnBnClickedSizeYDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_sizeY > minSizeY)
		--m_sizeY;
	UpdateData(FALSE);
	ShowCaptureArea();
}


void CCaputreTool::OnBnClickedSizeXDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_sizeX > minSizeX)
		--m_sizeX;
	UpdateData(FALSE);
	ShowCaptureArea();
}


void CCaputreTool::OnBnClickedSizeXUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_sizeX < gameSrc.cols)
		++m_sizeX;
	UpdateData(FALSE);
	ShowCaptureArea();
}


void CCaputreTool::OnBnClickedAreaYUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_areaY > 0)
		--m_areaY;
	UpdateData(FALSE);
	ShowCaptureArea();
}


void CCaputreTool::OnBnClickedAreaYDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_areaY + m_sizeY < gameSrc.rows)
		++m_areaY;
	UpdateData(FALSE);
	ShowCaptureArea();
}


void CCaputreTool::OnBnClickedAreaXDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_areaX > 0)
		--m_areaX;
	UpdateData(FALSE);
	ShowCaptureArea();
}


void CCaputreTool::OnBnClickedAreaXUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_areaX + m_sizeX < gameSrc.cols)
		++m_areaX;
	UpdateData(FALSE);
	ShowCaptureArea();
}

void CCaputreTool::ShowCaptureArea() {
	UpdateData(TRUE);
	gameSrc = hwnd2mat(gameHwnd);
	res.release();
	res = gameSrc(Range(m_areaY, m_areaY + m_sizeY), Range(m_areaX, m_areaX + m_sizeX));
	imshow("Capture Area", res);
}

void CCaputreTool::OnBnClickedSaveImage()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_fileName == "")
		MessageBox("파일 이름을 입력해주세요.");
	else {
		vector<int> cmp;
		cmp.push_back(IMWRITE_PNG_STRATEGY_DEFAULT);
		char name[1010];
		sprintf(name, "%s.png", m_fileName);
		imwrite(name, res, cmp);
		MessageBox("이 프로그램이 있는 폴더에 저장되었습니다.");
	}
}
