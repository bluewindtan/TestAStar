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
	// ˢ����ڵ��йصİ�ť����̬�ı��ȵȿؼ�
	void RefreshNodeCtrl(void);
	// �������ݳ�ʼ��
	void LoadDataFile(void);
	// ͨ��������ʼ��
	bool InitDataByParameters(void);
	// ���¼�������
	void ReloadData(void);
	// ��������
	bool SaveData(void);
	// ���������յ�
	bool SetBeginAndEndNode(int nBeginX, int nBeginY, int nEndX, int nEndY, bool bClear);
	// �жϸõ��Ƿ���Ա�����Ϊ�������յ�
	bool IsValidBeginAndEndNode(int nBeginX, int nBeginY, int nEndX, int nEndY);

	// ˢ�²����ؼ���״̬
	void RefreshCtrlStatus(void);

	// ˢ�²����ؼ���״̬(�༭����)
	void RefreshCtrlStatus_Edit(bool bEditting);

	// ������̬�ı�
	void CreateStatic(int nNeedCount);
	// ɾ����̬�ı�
	void DestroyStatic(void);
	// �����ڵ㰴ť
	void CreateBtnNode(int nNeedCount);
	// ɾ���ڵ㰴ť
	void DestroyBtnNode(void);

private:
	CAStar	m_AStarAlgo;		// A���㷨��
	int		m_nBeginX;
	int		m_nEndX;
	int		m_nBeginY;
	int		m_nEndY;
	int		m_nFourDirection;
	bool	m_bEditting;			// �Ƿ�༭״̬��
	bool	m_bKeyDownShift;		// �Ƿ�����Shift��
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

	std::vector< CButton* > m_vecBtnNode;	// �ڵ㰴ť�ļ���
	std::vector< CStatic* > m_vecStatic;	// ��ʾ�кź��кŵľ�̬�ı�����
public:
	afx_msg void OnBnClickedCheckLoad();
	afx_msg void OnBnClickedCheckRange();
};
