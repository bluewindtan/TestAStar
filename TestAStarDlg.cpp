// TestAStarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestAStar.h"
#include "TestAStarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// �Ի���Ŀ��
#define DLG_WIDTH 900
// �Ի���ĸ߶�
#define DLG_HEIGHT 660

// X������ʼ
#define POS_START_X 120
// Y������ʼ
#define POS_START_Y 1

// ��̬�ı��Ŀ��
#define STATIC_WIDTH 16
// ��̬�ı��ĸ߶�
#define STATIC_HEIGHT 14

// ��ť����
#define BTN_SIZE 18
// ��ťX����ƫ����
#define X_SPAN 0
// ��ťY����ƫ����
#define Y_SPAN 0

// ��ť����ʼID
#define BTN_ID_BEGIN 2000

// �ַ�������
const TCHAR* const g_strNode[NodeStatus_End] = {
	_T(""),		// ��
	_T("#"),	// �ϰ���
	_T("Z"),	// ·��
	_T("B"),	// ���
	_T("E"),	// �յ�
	_T("-"),	// ��:����λ
	_T("+"),	// �ϰ���:����λ
};

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTestAStarDlg dialog




CTestAStarDlg::CTestAStarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestAStarDlg::IDD, pParent)
	, m_nBeginX(0), m_nEndX(0), m_nBeginY(0), m_nEndY(0), m_nFourDirection(0)
	, m_bEditting(false), m_bKeyDownShift(false), m_nCurX(0), m_nCurY(0), m_nColumn(ASTAR_COLUMN_MAX)
	, m_nLine(ASTAR_LINE_MAX), m_nLoadDataFile(1), m_nRange(0), m_nRangeX(0), m_nRangeY(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CTestAStarDlg::~CTestAStarDlg()
{
	DestroyBtnNode();
	DestroyStatic();
}

void CTestAStarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_RUN, m_btnRun);
	DDX_Control(pDX, IDC_BTN_EDIT, m_btnEdit);
	DDX_Control(pDX, IDC_BTN_RELOAD, m_btnReload);
	DDX_Control(pDX, IDC_BTN_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_CHECK_WAY, m_CheckWay);
	DDX_Control(pDX, IDC_BUTTON_FIND, m_btnFind);
	DDX_Text(pDX, IDC_EDIT1, m_nBeginX);
	DDV_MinMaxInt(pDX, m_nBeginX, 0, ASTAR_COLUMN_MAX-1);
	DDX_Text(pDX, IDC_EDIT2, m_nBeginY);
	DDV_MinMaxInt(pDX, m_nBeginY, 0, ASTAR_LINE_MAX-1);
	DDX_Text(pDX, IDC_EDIT3, m_nEndX);
	DDV_MinMaxInt(pDX, m_nEndX, 0, ASTAR_COLUMN_MAX -1);
	DDX_Text(pDX, IDC_EDIT4, m_nEndY);
	DDV_MinMaxInt(pDX, m_nEndY, 0, ASTAR_LINE_MAX-1);
	DDX_Text(pDX, IDC_EDIT5, m_nCurX);
	DDV_MinMaxInt(pDX, m_nCurX, 0, ASTAR_COLUMN_MAX -1);
	DDX_Text(pDX, IDC_EDIT6, m_nCurY);
	DDV_MinMaxInt(pDX, m_nCurY, 0, ASTAR_LINE_MAX -1);
	DDX_Check(pDX, IDC_CHECK_FOUR_DIRECTION, m_nFourDirection);
	DDX_Text(pDX, IDC_EDIT7, m_nColumn);
	DDV_MinMaxInt(pDX, m_nColumn, 1, ASTAR_COLUMN_MAX);
	DDX_Text(pDX, IDC_EDIT8, m_nLine);
	DDV_MinMaxInt(pDX, m_nLine, 1, ASTAR_LINE_MAX);
	DDX_Check(pDX, IDC_CHECK_LOAD, m_nLoadDataFile);
	DDX_Text(pDX, IDC_EDIT9, m_nRangeX);
	DDV_MinMaxInt(pDX, m_nRangeX, 0, ASTAR_COLUMN_MAX-1);
	DDX_Text(pDX, IDC_EDIT10, m_nRangeY);
	DDV_MinMaxInt(pDX, m_nRangeY, 0, ASTAR_LINE_MAX-1);
	DDX_Check(pDX, IDC_CHECK_RANGE, m_nRange);
}

BEGIN_MESSAGE_MAP(CTestAStarDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(BTN_ID_BEGIN, (BTN_ID_BEGIN+ASTAR_COLUMN_MAX*ASTAR_LINE_MAX-1), &CTestAStarDlg::OnBnClickedButton)
	ON_BN_CLICKED(IDC_BTN_RUN, &CTestAStarDlg::OnBnClickedBtnRun)
	ON_BN_CLICKED(IDC_BTN_EDIT, &CTestAStarDlg::OnBnClickedBtnEdit)
	ON_BN_CLICKED(IDC_BTN_RELOAD, &CTestAStarDlg::OnBnClickedBtnReload)
	ON_BN_CLICKED(IDC_BTN_SET, &CTestAStarDlg::OnBnClickedBtnSet)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CTestAStarDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_CHECK_WAY, &CTestAStarDlg::OnBnClickedCheckWay)
	ON_BN_CLICKED(IDC_BUTTON_FIND, &CTestAStarDlg::OnBnClickedButtonFind)
	ON_BN_CLICKED(IDC_CHECK_LOAD, &CTestAStarDlg::OnBnClickedCheckLoad)
	ON_BN_CLICKED(IDC_CHECK_RANGE, &CTestAStarDlg::OnBnClickedCheckRange)
END_MESSAGE_MAP()


// CTestAStarDlg message handlers

BOOL CTestAStarDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	// ��������
	LoadDataFile();

	// ˢ��״̬
	RefreshCtrlStatus();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestAStarDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestAStarDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestAStarDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTestAStarDlg::OnBnClickedButton(UINT nID)
{
	// TODO: Add your control notification handler code here
	if (!m_AStarAlgo.GetInitialzed())
	{
		return;
	}
	// ѡ������
	int nIndex = nID - BTN_ID_BEGIN;
	CString strText;
	// �жϵ�ǰ�Ƿ���Shift��
	if (m_bKeyDownShift)
	{
		// ȡ�������ı�
		CWnd* pWndLink = GetDlgItem(IDC_EDIT_LINK);
		CString strLink;
		pWndLink->GetWindowText(strLink);
		// �Զ���ӽ�ѡ�еİ�ť�����������λ������Ϣ�������
		strText.Format(_T("%s%d%s"),strLink, nIndex, _T(DATA_DELIMITER));
		pWndLink->SetWindowText(strText);
		return; // ������ϣ�������
	}
	// ��ǰû�а���Shift��
	strText.Format(_T("%d"), nIndex);
	GetDlgItem(IDC_STATIC_INDEX)->SetWindowText(strText);
	if (m_AStarAlgo.ConvertIndexToCoordinate(nIndex, m_nCurX, m_nCurY))
	{
		int nValue = m_AStarAlgo.GetMapValue(m_nCurX, m_nCurY);
		m_CheckWay.SetCheck(CAStar::IsNodeBlock(nValue) ? BST_UNCHECKED : BST_CHECKED);
	}

	// ȡ����λ��Ϣ
	const char* sSeatInfo = m_AStarAlgo.GetSeatInfo(nIndex);
	if (sSeatInfo)
	{
		// ��������
		char sTemp[SEAT_TOTAL_LEN];
		strcpy_s(sTemp, SEAT_TOTAL_LEN, sSeatInfo);
		delete[] sSeatInfo;
		// ���������ַ���
		char sIndex[SEAT_TOTAL_LEN];
		sprintf_s(sIndex, SEAT_TOTAL_LEN, "%d%s", nIndex, DATA_DELIMITER);
		// ȥ����ͷ������
		char strAllInfo[SEAT_TOTAL_LEN];
		strcpy_s(strAllInfo, SEAT_TOTAL_LEN, sTemp + strlen(sIndex));
		char* pTemp = strstr(strAllInfo, sIndex);
		// ȥ����β������
		if (pTemp)
		{
			pTemp[0] = 0;
		}
		// ת������ʾ֮
#ifdef UNICODE 
		wchar_t strSeatLink[SEAT_TOTAL_LEN];
		memset(strSeatLink, 0, SEAT_TOTAL_LEN * sizeof(wchar_t));
		int nBufferSize = MultiByteToWideChar(CP_ACP, 0, strAllInfo, -1, strSeatLink, 0);
		MultiByteToWideChar(CP_ACP, 0, strAllInfo, -1, strSeatLink, nBufferSize);
		GetDlgItem(IDC_EDIT_LINK)->SetWindowText(strSeatLink);
#else
		GetDlgItem(IDC_EDIT_LINK)->SetWindowText(strAllInfo);
#endif
	}
	else
	{
		GetDlgItem(IDC_EDIT_LINK)->SetWindowText(_T(""));
	}
	
	m_nRange = 0; // ��Χ���ÿ���һ��Ҫ�ر�
	GetDlgItem(IDC_EDIT9)->EnableWindow(m_bEditting && m_nRange);
	GetDlgItem(IDC_EDIT10)->EnableWindow(m_bEditting && m_nRange);
	UpdateData(FALSE);
}

bool CTestAStarDlg::InitDataByParameters(void)
{
	// ��������
	if (!UpdateData())
	{
		return false;
	}
	// ������ʼ��
	return m_AStarAlgo.InitData(NULL, m_nColumn, m_nLine, 0, m_nLine*m_nColumn - 1, 0);
}

void CTestAStarDlg::OnBnClickedBtnRun()
{
	// TODO: Add your control notification handler code here
	UpdateData(FALSE);
	// Ѱ·��ʼ
	if (!m_AStarAlgo.FindPath())
	{
		AfxMessageBox(_T("Can't find the way!"));
		return;
	}
	// ��ʾ����
	CButton* pBtn = NULL;
	int nValue = 0; // �����е�ֵ
	int nCount = 0; // �߹��ĸ�������(���������յ�)
	for (int i = 0; i < m_AStarAlgo.GetTotalCount(); i++)
	{
		nValue = m_AStarAlgo.GetMapValue(i);
		// ����·������ʾ֮
		if (NodeStatus_Way == nValue)
		{
			nCount++;
			pBtn = m_vecBtnNode[i];
			if (pBtn)
			{
				pBtn->SetWindowText(g_strNode[nValue]);
			}
		}
	}
	// ��ʾ�ɹ�
	CString strText;
	strText.Format(_T("Oh! We did it!\nFind the way with %d steps."), nCount);
	AfxMessageBox(strText);
}

void CTestAStarDlg::OnBnClickedBtnEdit()
{
	// TODO: Add your control notification handler code here
	//
	UpdateData(FALSE);
	//
	if (m_bEditting)
	{
		int nIndex = m_AStarAlgo.ConvertCoordinateToIndex(m_nCurX, m_nCurY);
		//
		CString strText;
		GetDlgItem(IDC_EDIT_LINK)->GetWindowText(strText);
		TCHAR strSeatLink[SEAT_TOTAL_LEN];
		memset(strSeatLink, 0, SEAT_TOTAL_LEN * sizeof(TCHAR));
		_stprintf_s(strSeatLink, SEAT_TOTAL_LEN, _T("%d%s%s%d%s"), nIndex, _T(DATA_DELIMITER), strText.GetBuffer(), nIndex, _T(DATA_DELIMITER));
		strText.ReleaseBuffer();
		// ת���ַ���
		char sSeatInfo[SEAT_TOTAL_LEN];
		memset(sSeatInfo, 0, SEAT_TOTAL_LEN * sizeof(char));
#ifdef UNICODE 
		int nBufferSize = WideCharToMultiByte(CP_ACP, 0, strSeatLink, -1, sSeatInfo, 0, NULL, NULL);
		WideCharToMultiByte(CP_ACP, 0, strSeatLink, -1, sSeatInfo, nBufferSize, NULL, NULL);
#else
		strcpy_s(sSeatInfo, SEAT_TOTAL_LEN, strSeatLink);
#endif
		sSeatInfo[SEAT_TOTAL_LEN - 1] = 0;
		int nStatus = BST_UNCHECKED == m_CheckWay.GetCheck() ? NodeStatus_Block : NodeStatus_None;
		if (!m_AStarAlgo.SetSeatInfo(nIndex, sSeatInfo)
			|| !m_AStarAlgo.SetMapValue(nIndex, nStatus)
			)
		{
			AfxMessageBox(_T("Set the info of seat failed!"));
			return;
		}
	}
	else
	{
		// ע�������ǷǱ༭״̬ʱ����״̬
		// �ı�״̬
		m_bEditting = !m_bEditting;
	}

	RefreshCtrlStatus_Edit(m_bEditting);
}

void CTestAStarDlg::OnBnClickedBtnReload()
{
	// TODO: Add your control notification handler code here

	m_bEditting = false;
	ReloadData();

}

void CTestAStarDlg::OnBnClickedBtnSet()
{
	// TODO: Add your control notification handler code here

	// ȥ��ԭ������ʼ�㡢������
	int nBeginX = m_nBeginX;
	int nBeginY = m_nBeginY;
	int nEndX = m_nEndX;
	int nEndY = m_nEndY;
	int nFourDirection = m_nFourDirection;
	// ��������
	if (!UpdateData())
	{
		return;
	}

	// �ж���Ч��
	if (!IsValidBeginAndEndNode(m_nBeginX, m_nBeginY, m_nEndX, m_nEndY))
	{
		AfxMessageBox(_T("Invalid begin or end. Please reset them!"));

		// �ع�
		m_nBeginX = nBeginX;
		m_nBeginY = nBeginY;
		m_nEndX = nEndX;
		m_nEndY = nEndY;
		m_nFourDirection = nFourDirection;
		UpdateData(FALSE);
		return;
	}

	// ���õ��㷨��
	m_AStarAlgo.SetFourDirection(m_nFourDirection);
	m_AStarAlgo.SetStartIndex(m_nBeginX, m_nBeginY);
	m_AStarAlgo.SetEndIndex(m_nEndX, m_nEndY);

	// ���ԭ���������յ�
	SetBeginAndEndNode(nBeginX, nBeginY, nEndX, nEndY, true);

	// ���������յ�
	SetBeginAndEndNode(m_nBeginX, m_nBeginY, m_nEndX, m_nEndY, false);

}

void CTestAStarDlg::LoadDataFile(void)
{
	if (m_AStarAlgo.GetInitialzed())
	{
		ASSERT(0);
		return;
	}
	// ���ļ�����
	bool bLoad = false;
	CString strText;
	if (m_nLoadDataFile)
	{
		bLoad = m_AStarAlgo.LoadDataFile();
		strText = _T("Failed to load data file.");
	}
	else
	{
		bLoad = InitDataByParameters();
		strText = _T("Failed to initialize data.");
	}
	if (!bLoad)
	{
		AfxMessageBox(strText);
		return;
	}
	// ���غ󣬴�����ť
	CreateBtnNode(m_AStarAlgo.GetTotalCount());
	// ������̬�ı�
	CreateStatic(m_AStarAlgo.GetColumnCount() + m_AStarAlgo.GetLineCount());
	// ˢ��λ��
	RefreshNodeCtrl();

	// ���ݼ��غ���࣬ˢ�½���
	m_AStarAlgo.GetStartIndex(m_nBeginX, m_nBeginY);
	m_AStarAlgo.GetEndIndex(m_nEndX, m_nEndY);
	m_nFourDirection = m_AStarAlgo.GetFourDirection();
	m_nLine = m_AStarAlgo.GetLineCount();
	m_nColumn = m_AStarAlgo.GetColumnCount();
	m_nLoadDataFile = 1; // ��ʼ����һ��ҪĬ�ϼ��������ļ�
	UpdateData(FALSE);

	// ���������յ�
	SetBeginAndEndNode(m_nBeginX, m_nBeginY, m_nEndX, m_nEndY, false);

}

void CTestAStarDlg::ReloadData(void)
{
	//// ȷ���Ƿ���Ҫ�ȱ���һ��
	//if (IDOK == AfxMessageBox(_T("Do you want to save the data before reload it?"), MB_OKCANCEL))
	//{
	//	if (!SaveData()) // ����
	//	{
	//		AfxMessageBox(_T("Cann't saving file!"));
	//		return; // �û���Ҫ���棬ʧ�ܣ��������������
	//	}
	//}
	// ����Ѿ���ʼ��������ɾ��֮
	if (m_AStarAlgo.GetInitialzed())
	{
		m_AStarAlgo.DestroyData();
	}
	// �ټ�������
	LoadDataFile();
	// ˢ��״̬
	RefreshCtrlStatus();
}

bool CTestAStarDlg::SetBeginAndEndNode(int nBeginX, int nBeginY, int nEndX, int nEndY, bool bClear)
{
	// �ж������յ��Ƿ�Ϊ�ϰ���
	if (!IsValidBeginAndEndNode(nBeginX, nBeginY, nEndX, nEndY))
	{
		return false;
	}
	// 
	CButton* pBtn = NULL;
	// �����µ���ʼ�㡢������
	int nIndex = m_AStarAlgo.ConvertCoordinateToIndex(nBeginX, nBeginY);
	if (nIndex >= 0 && nIndex < (int)m_vecBtnNode.size())
	{
		pBtn = m_vecBtnNode[nIndex];
	}
	if (pBtn)
	{
		pBtn->SetWindowText(bClear ? g_strNode[NodeStatus_None] : g_strNode[NodeStatus_Start]);
	}
	nIndex = m_AStarAlgo.ConvertCoordinateToIndex(nEndX, nEndY);
	pBtn = NULL;
	if (nIndex >= 0 && nIndex < (int)m_vecBtnNode.size())
	{
		pBtn = m_vecBtnNode[nIndex];
	}
	if (pBtn)
	{
		pBtn->SetWindowText(bClear ? g_strNode[NodeStatus_None] : g_strNode[NodeStatus_Final]);
	}

	return true;
}

bool CTestAStarDlg::IsValidBeginAndEndNode(int nBeginX, int nBeginY, int nEndX, int nEndY)
{
	// �ж��Ƿ�ͬһ����
	if (nBeginX == nEndX && nBeginY == nEndY)
	{
		return false;
	}
	// ���
	int nMapValue = m_AStarAlgo.GetMapValue(nBeginX, nBeginY);
	if (nMapValue < NodeStatus_Begin || nMapValue >= NodeStatus_End // �ж��Ƿ���Чֵ
		|| CAStar::IsNodeBlock(nMapValue) // �ж��Ƿ����ϰ���
		)
	{
		return false;
	}
	// �յ�
	nMapValue = m_AStarAlgo.GetMapValue(nEndX, nEndY);
	if (nMapValue < NodeStatus_Begin || nMapValue >= NodeStatus_End // �ж��Ƿ���Чֵ
		|| CAStar::IsNodeBlock(nMapValue) // �ж��Ƿ����ϰ���
		)
	{
		return false;
	}

	return true;
}

void CTestAStarDlg::OnBnClickedBtnSave()
{
	if (SaveData())
	{
		AfxMessageBox(_T("Finish saving file!"));
	}
}

bool CTestAStarDlg::SaveData(void)
{
	// TODO: Add your control notification handler code here
	if (m_AStarAlgo.GetInitialzed())
	{
		if (m_AStarAlgo.SaveDataFile())
		{
			m_bEditting = false;
			RefreshCtrlStatus_Edit(m_bEditting);

			return true;
		}
	}

	return false;
}

void CTestAStarDlg::RefreshNodeCtrl(void)
{
	if (!m_AStarAlgo.GetInitialzed())
	{
		return;
	}
	// �ƶ��Ի����λ��
	int nWidth = POS_START_X + STATIC_WIDTH + m_AStarAlgo.GetColumnCount() * (BTN_SIZE+X_SPAN) + 16;
	if (nWidth < DLG_WIDTH)
	{
		nWidth = DLG_WIDTH;
	}
	int nHeight = POS_START_Y + STATIC_HEIGHT + m_AStarAlgo.GetLineCount() * (BTN_SIZE+Y_SPAN) + 36;
	if (nHeight < DLG_HEIGHT)
	{
		nHeight = DLG_HEIGHT;
	}
	int x = 0;
	int y = 0;
	x = (::GetSystemMetrics(SM_CXMAXIMIZED) - nWidth) / 2;
	if (x < 0)
	{
		x = 0;
	}
	y = (::GetSystemMetrics(SM_CYMAXIMIZED) - nHeight) / 2;
	if (y < -2)
	{
		y = -2;
	}
	MoveWindow(x, y, nWidth, nHeight);
	// �ƶ�λ��
	CButton* pBtn = NULL;
	x = 0;
	y = 0;
	int i = 0;
	int nValue = 0; // �����е�ֵ
	int nSize = (int)m_vecBtnNode.size();
	for (; i < m_AStarAlgo.GetTotalCount() && i < nSize; i++)
	{
		if (! m_AStarAlgo.ConvertIndexToCoordinate(i, x, y))
		{
			ASSERT(0);
			continue;
		}
		nValue = m_AStarAlgo.GetMapValue(i);
		if (nValue < NodeStatus_Begin || nValue >= NodeStatus_End)
		{
			ASSERT(0);
			nValue = NodeStatus_None;
		}
		// ���ð�ť
		//pBtn = (CButton*)GetDlgItem(BTN_ID_BEGIN + i);
		pBtn = m_vecBtnNode[i];
		if (pBtn)
		{
			pBtn->ShowWindow(SW_SHOW);
			pBtn->SetWindowText(g_strNode[nValue]);
			pBtn->MoveWindow(x*(BTN_SIZE+X_SPAN)+POS_START_X+STATIC_WIDTH
				, y*(BTN_SIZE+Y_SPAN)+POS_START_Y+STATIC_HEIGHT
				, BTN_SIZE, BTN_SIZE);
			pBtn->EnableWindow(FALSE);
		}
		else
		{
			ASSERT(0);
		}
	}
	// ����İ�ť���ص�
	for (; i < nSize; i++)
	{
		pBtn = m_vecBtnNode[i];
		if (pBtn)
		{
			pBtn->ShowWindow(SW_HIDE);
		}
		else
		{
			ASSERT(0);
		}
	}

	// �к�
	CString strNap = _T("    ");
	CString strText = _T("");
	CStatic* pStatic = NULL;
	i = m_AStarAlgo.GetColumnCount();
	int j = 0;
	nSize = (int)m_vecStatic.size();
	for (0; j < i; j++)
	{
		if (j >= nSize)
		{
			ASSERT(0);
			break;
		}
		pStatic = m_vecStatic[j];
		if (pStatic)
		{
			pStatic->ShowWindow(SW_SHOW);
			pStatic->MoveWindow(POS_START_X+STATIC_WIDTH+j*(BTN_SIZE+X_SPAN), POS_START_Y, STATIC_WIDTH, STATIC_HEIGHT);
			// �ı�
			strText.Format(_T("%.2d%s"), j, strNap);
			pStatic->SetWindowText(strText);
		}
		else
		{
			ASSERT(0);
		}
	}

	// �к�
	strNap = _T("\n ");
	strText = _T("");
	i = m_AStarAlgo.GetLineCount();
	int nColumnCount = m_AStarAlgo.GetColumnCount();
	j = 0;
	for ( ; j < i; j++)
	{
		if (j+nColumnCount >= nSize)
		{
			ASSERT(0);
			break;
		}
		pStatic = m_vecStatic[j+nColumnCount];
		if (pStatic)
		{
			pStatic->ShowWindow(SW_SHOW);
			pStatic->MoveWindow(POS_START_X, POS_START_Y+STATIC_HEIGHT+j*(BTN_SIZE+Y_SPAN), STATIC_WIDTH, STATIC_HEIGHT);
			// �ı�
			strText.Format(_T("%.2d%s"), j, strNap);
			pStatic->SetWindowText(strText);
		}
		else
		{
			ASSERT(0);
		}
	}

	// ����İ�ť���ص�
	j += nColumnCount;
	for (; j < nSize; j++)
	{
		pStatic = m_vecStatic[j];
		if (pStatic)
		{
			pStatic->ShowWindow(SW_HIDE);
		}
		else
		{
			ASSERT(0);
		}
	}
}

void CTestAStarDlg::RefreshCtrlStatus(void)
{
	BOOL bEnable = m_AStarAlgo.GetInitialzed() ? TRUE : FALSE;

	m_btnEdit.EnableWindow(bEnable);
	m_btnRun.EnableWindow(bEnable);
	m_btnSave.EnableWindow(bEnable);
	m_btnReload.EnableWindow(TRUE); // ����������Զ����״̬
	GetDlgItem(IDC_BTN_SET)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT1)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT2)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT3)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT4)->EnableWindow(bEnable);

	GetDlgItem(IDC_CHECK_FOUR_DIRECTION)->EnableWindow(bEnable);

	// ���ݱ༭״̬�����Ʊ༭���ֵĿؼ�״̬
	RefreshCtrlStatus_Edit(m_bEditting);

	//
	GetDlgItem(IDC_EDIT7)->EnableWindow(m_nLoadDataFile ? FALSE : TRUE);
	GetDlgItem(IDC_EDIT8)->EnableWindow(m_nLoadDataFile ? FALSE : TRUE);
}

void CTestAStarDlg::RefreshCtrlStatus_Edit(bool bEditting)
{
	// �ı䰴ť״̬
	CButton* pBtn = NULL;
	int nSize = (int)m_vecBtnNode.size();
	for (int i = 0; i < m_AStarAlgo.GetTotalCount() && i < nSize; i++)
	{
		pBtn = m_vecBtnNode[i];
		if (pBtn)
		{
			pBtn->EnableWindow(bEditting);
		}
	}
	// ��ʾ����
	if (bEditting)
	{
		m_btnEdit.SetWindowText(_T("ATTACH"));
	}
	else
	{
		m_btnEdit.SetWindowText(_T("EDIT"));
	}
	// 
	GetDlgItem(IDC_EDIT5)->EnableWindow(bEditting);
	GetDlgItem(IDC_EDIT6)->EnableWindow(bEditting);
	GetDlgItem(IDC_EDIT_LINK)->EnableWindow(bEditting);
	m_CheckWay.EnableWindow(bEditting);
	m_btnFind.EnableWindow(bEditting);

	GetDlgItem(IDC_EDIT9)->EnableWindow(bEditting && m_nRange);
	GetDlgItem(IDC_EDIT10)->EnableWindow(bEditting && m_nRange);
	GetDlgItem(IDC_CHECK_RANGE)->EnableWindow(bEditting);

}

void CTestAStarDlg::OnBnClickedCheckWay()
{
	// TODO: Add your control notification handler code here
	if (!UpdateData())
	{
		return;
	}
	int nValue = NodeStatus_None;
	if (BST_UNCHECKED == m_CheckWay.GetCheck())
	{
		nValue = NodeStatus_Block;
	}
	// �ж��Ƿ��Ƿ�Χ����
	int nRangeX = m_nCurX;
	int nRangeY = m_nCurY;
	if (m_nRange)
	{
		nRangeX = m_nRangeX;
		nRangeY = m_nRangeY;
	}
	// ѭ������
	int nIndex = 0;
	for (int i = min(m_nCurX,nRangeX); i <= max(m_nCurX,nRangeX); i++)
	{
		for (int j = min(m_nCurY,nRangeY); j <= max(m_nCurY,nRangeY); j++)
		{
			nIndex = m_AStarAlgo.ConvertCoordinateToIndex(i, j);
			if (m_AStarAlgo.SetMapValue(nIndex, nValue))
			{
				m_vecBtnNode[nIndex]->SetWindowText(g_strNode[nValue]);
			}
		}
	}
	
}

void CTestAStarDlg::OnBnClickedButtonFind()
{
	// TODO: Add your control notification handler code here
	// ��������
	if (!UpdateData())
	{
		return;
	}
	int nIndex = m_AStarAlgo.ConvertCoordinateToIndex(m_nCurX, m_nCurY);
	CString strText;
	strText.Format(_T("%d"), nIndex);
	GetDlgItem(IDC_STATIC_INDEX)->SetWindowText(strText);

	// {Test By TanFeng 2012-06-28 ���Բ�������ķ��ϰ���ڵ�
	int nNearestIndex = m_AStarAlgo.FindNearestUnblockNode(nIndex);
	int x = 0;
	int y = 0;
	m_AStarAlgo.ConvertIndexToCoordinate(nNearestIndex, x, y);
	strText.Format(_T("The nearest unblock node is %d(%d,%d)."), nNearestIndex, x, y);
	AfxMessageBox(strText);
	// ~}Test By TanFeng
}

BOOL CTestAStarDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (VK_SHIFT == pMsg->wParam)
	{
		if(WM_KEYDOWN == pMsg->message)
		{
			m_bKeyDownShift = true;
		}
		else if(WM_KEYUP == pMsg->message)
		{
			m_bKeyDownShift = false;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CTestAStarDlg::CreateBtnNode(int nNeedCount)
{
	if (nNeedCount > ASTAR_COLUMN_MAX*ASTAR_LINE_MAX)
	{
		ASSERT(0);
		return;
	}
	int i = (int)m_vecBtnNode.size();
	if (nNeedCount <= i)
	{
		return;
	}
	for (; i < nNeedCount; i++)
	{
		CButton* pBtn = new CButton;
		if (pBtn)
		{
			pBtn->Create(_T("0"), WS_CHILD|WS_VISIBLE|BS_FLAT|BS_PUSHBUTTON, 
				CRect(0,0,BTN_SIZE,BTN_SIZE), this, BTN_ID_BEGIN + i);
			m_vecBtnNode.push_back(pBtn);
		}
		else
		{
			ASSERT(0);
			return; // error
		}
	}
}

void CTestAStarDlg::DestroyBtnNode(void)
{
	int nSize = (int)m_vecBtnNode.size();
	for (int i=0; i<nSize; i++)
	{
		delete m_vecBtnNode[i];
		m_vecBtnNode[i] = NULL;
	}
	m_vecBtnNode.clear();
}

void CTestAStarDlg::CreateStatic(int nNeedCount)
{
	if (nNeedCount > ASTAR_COLUMN_MAX + ASTAR_LINE_MAX)
	{
		ASSERT(0);
		return;
	}
	int i = (int)m_vecStatic.size();
	if (nNeedCount <= i)
	{
		return;
	}
	for (; i < nNeedCount; i++)
	{
		CStatic* pStatic = new CStatic;
		if (pStatic)
		{
			pStatic->Create(_T(""), WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(0,0,STATIC_WIDTH,STATIC_HEIGHT), this);
			m_vecStatic.push_back(pStatic);
		}
		else
		{
			ASSERT(0);
			return; // error
		}
	}
}

void CTestAStarDlg::DestroyStatic(void)
{
	int nSize = (int)m_vecStatic.size();
	for (int i=0; i<nSize; i++)
	{
		delete m_vecStatic[i];
		m_vecStatic[i] = NULL;
	}
	m_vecStatic.clear();
}

void CTestAStarDlg::OnBnClickedCheckLoad()
{
	// TODO: Add your control notification handler code here
	// ��������
	if (!UpdateData())
	{
		return;
	}
	GetDlgItem(IDC_EDIT7)->EnableWindow(m_nLoadDataFile ? FALSE : TRUE);
	GetDlgItem(IDC_EDIT8)->EnableWindow(m_nLoadDataFile ? FALSE : TRUE);
}

void CTestAStarDlg::OnBnClickedCheckRange()
{
	// TODO: Add your control notification handler code here
	// ��������
	if (!UpdateData())
	{
		return;
	}
	GetDlgItem(IDC_EDIT9)->EnableWindow(m_bEditting && m_nRange);
	GetDlgItem(IDC_EDIT10)->EnableWindow(m_bEditting && m_nRange);
}
