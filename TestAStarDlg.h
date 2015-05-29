// TestAStarDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "AStar/AStar.h"
#include <vector>
using namespace std;

// CTestAStarDlg dialog
class CTestAStarDlg : public CDialog
{
// Construction
public:
	CTestAStarDlg(CWnd* pParent = NULL);	// standard constructor
	~CTestAStarDlg();

// Dialog Data
	enum { IDD = IDD_TESTASTAR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton(UINT nID);
	afx_msg void OnBnClickedBtnRun();
	afx_msg void OnBnClickedBtnEdit();
	afx_msg void OnBnClickedBtnReload();
	afx_msg void OnBnClickedBtnSet();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedCheckWay();
	afx_msg void OnBnClickedButtonFind();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	// 刷新与节点有关的按钮、静态文本等等控件
	void RefreshNodeCtrl(void);
	// 加载数据初始化
	void LoadDataFile(void);
	// 通过参数初始化
	bool InitDataByParameters(void);
	// 重新加载数据
	void ReloadData(void);
	// 保存数据
	bool SaveData(void);
	// 设置起点和终点
	bool SetBeginAndEndNode(int nBeginX, int nBeginY, int nEndX, int nEndY, bool bClear);
	// 判断该店是否可以被设置为起点或者终点
	bool IsValidBeginAndEndNode(int nBeginX, int nBeginY, int nEndX, int nEndY);

	// 刷新操作控件的状态
	void RefreshCtrlStatus(void);

	// 刷新操作控件的状态(编辑部分)
	void RefreshCtrlStatus_Edit(bool bEditting);

	// 创建静态文本
	void CreateStatic(int nNeedCount);
	// 删除静态文本
	void DestroyStatic(void);
	// 创建节点按钮
	void CreateBtnNode(int nNeedCount);
	// 删除节点按钮
	void DestroyBtnNode(void);

private:
	CAStar	m_AStarAlgo;		// A星算法类
	int		m_nBeginX;
	int		m_nEndX;
	int		m_nBeginY;
	int		m_nEndY;
	int		m_nFourDirection;
	bool	m_bEditting;			// 是否编辑状态中
	bool	m_bKeyDownShift;		// 是否按下了Shift键
	int		m_nCurX;
	int		m_nCurY;
	int		m_nColumn;
	int		m_nLine;
	int		m_nLoadDataFile;
	int		m_nRange;
	int		m_nRangeX;
	int		m_nRangeY;
	CButton m_btnRun;
	CButton m_btnEdit;
	CButton m_btnReload;
	CButton m_btnSave;
	CButton m_CheckWay;
	CButton m_btnFind;

	std::vector< CButton* > m_vecBtnNode;	// 节点按钮的集合
	std::vector< CStatic* > m_vecStatic;	// 显示行号和列号的静态文本集合
public:
	afx_msg void OnBnClickedCheckLoad();
	afx_msg void OnBnClickedCheckRange();
};
