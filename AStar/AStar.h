/////////////////////////////////////////////////////////////////
/// @file AStar.h
///
/// @purpose A���㷨��
///
/// @author TanFeng
///
/// @date 04/25/2012
/// -------------------------------------------------------------
/// ChangeLog:
///
/// -------------------------------------------------------------
/// Copyright (C) 2012 - All Rights Reserved
/////////////////////////////////////////////////////////////////
#ifndef __A_STAR_CLASS_H__
#define __A_STAR_CLASS_H__
#include "CustomList.h"

class CNodeRect;
// 
class CAStar
{
public:
	CAStar(void);
	~CAStar(void);

public:
	// ��ʼ����ͨ������Ĳ���
	bool InitData(int* pMap, int nWidth, int nLength, int nStart, int nEnd, int nFourDirection);
	// ��ʼ�������������ļ�
	bool LoadDataFile(const char* sFileName = DATA_FILE_NAME);
	// ���������ļ�
	bool SaveDataFile(const char* sFileName = DATA_FILE_NAME);
	// ��������
	void DestroyData(void);

	// Ѱ·
	bool FindPath(void);

public:
	// �Ƿ��ʼ��
	bool GetInitialzed(void) const {return m_bInitialized;}

	// ������λ��Ϣ
	bool SetSeatInfo(int nIndex, const char* sInfo);
	// ������λ��Ϣ(ע����Ҫɾ�����ص��ַ���ָ��)
	const char* GetSeatInfo(int nIndex) const;

	// ���õ�ͼֵ
	bool SetMapValue(int nIndex, int nStatus);
	bool SetMapValue(int x, int y, int nStatus);
	// �õ���ͼֵ
	int GetMapValue(int x, int y) const;
	int GetMapValue(int nIndex) const;

	// ���ԵĶ�ȡ
	int GetFourDirection(void) const {return m_nFourDirection;}
	void SetFourDirection(int nFourDirection) {m_nFourDirection = nFourDirection;}

	// �������
	void SetStartIndex(int nIndex) {m_nStart = nIndex;}
	void SetStartIndex(int x, int y);
	bool GetStartIndex(int& x, int& y) const;
	// �����յ�
	void SetEndIndex(int nIndex) {m_nEnd = nIndex;}
	void SetEndIndex(int x, int y);
	bool GetEndIndex(int& x, int& y) const;

	// �ж��Ƿ���Ч����
	bool CheckIndex(int nIndex) const;
	// �ж��Ƿ���Ч������
	bool CheckColumn(int nColumnIndex) const;
	// �ж��Ƿ���Ч������
	bool CheckLine(int nLineIndex) const;

	// �����������Ļ���ת��
	int ConvertCoordinateToIndex(int x, int y) const;
	bool ConvertIndexToCoordinate(int nIndex, int& x, int& y) const;

	// �õ�����
	int GetLineCount(void) const {return m_nHeight;}
	// �õ�����
	int GetColumnCount(void) const {return m_nWidth;}
	// �õ���ͼbEnable���ε�������
	int GetTotalCount(void) const {return m_nWidth * m_nHeight;}

	// �������ս����ǰһ��
	int GetPreWay(int nIndex);
	// ȷ����ǰ����ǰһ����ĸ�����ֵ(����ֵ��0 ͬһ��-1 ��Ч������ ����ķ���ֵ��)
	int GetDirectionValueWith2Node(int nPre, int nCur);

	// �Ӹ��������������ķ��ϰ��ĸ���
	int FindNearestUnblockNode(int nIndex, int nDirection = DirectionIndex_End, int nLayer = 1);

public:
	// �ж��Ƿ�Ϊ�ϰ���
	static bool IsNodeBlock(int nValue);

	// �ж��Ƿ�Ϊ��Ч����
	static bool IsValidLine(int nLine) {return nLine > 0 && nLine <= ASTAR_LINE_MAX;}
	// �ж��Ƿ�Ϊ��Ч����
	static bool IsValidColumn(int nColumn) {return nColumn > 0 && nColumn <= ASTAR_COLUMN_MAX;}

	// �ж��Ƿ���Ч����ֵ
	static bool IsValidDirectionIndex(int nDirectionIndex) {return nDirectionIndex >= DirectionIndex_Begin && nDirectionIndex < DirectionIndex_End;}
	// ���ݷ���ֵ�õ����������
	static int GetDirectionIndexByValue(int nDirectionValue);
	// ���ݷ���õ��Ƕ�
	static int GetDegreeByDirectionIndex(int nDirectionIndex);
	// ���ݽǶȵõ�ŷ����ֵ
	static float GetEulerAnglesByDegree(int nDegree);

private:
	// Ѱ����һ��
	bool FindNext(void);
	// ��ȡ���,�����·����bEnableֵ�޸�Ϊһ��·��
	void GetResult(void);

	// ���ݵ�ǰ��������չ����ó�������
	int GetNewIndexByDirectionValue(int nCurIndex, int nDirectionValue, int nLayer = 1);
	// ��ĳ��������չbEnable�����ͬʱ���뵽Open�б���(��nResult����1����ʾ�ҵ����յ���)
	bool ExpandNodeRectByDirectionValue(int nCurIndex, int nDirectionValue, int& nResult);
	// �Ӹ���������չbEnable������
	bool ExpandNodeRectAllDirection(int nCurIndex, int& nResult);

	// ��ȡHֵ
	int GetValueH(int i);
	// ��ȡGֵ
	int GetValueG(int i, int nDirectionValue);

	// ����bEnable���Σ����ҳ�ʼ��֮
	void CreateNodeRect(void);
	void InitNodeRect(void);
	void DestroyNodeRect(void);

	// ������Ҫ����
	bool ParseImportantMember(char* sInfo);
	// ������ͼ��Ϣ
	bool ParseMapInfo(int nLine, char* sInfo);
	// ������λ��Ϣ
	bool ParseSeatInfo(char* sInfo);

	// �����Ҫ����
	bool CheckImportantMember(void);

private:
	bool		m_bInitialized;		// �Ƿ��ʼ����
	int			m_nFourDirection;	// bEnable���ε���չ����1(��0)Ϊ�ķ���0Ϊ�˷���
	int			m_nWidth;			// ��ͼ������
	int			m_nHeight;			// ��ͼ������
	int			m_nStart;			// �������
	int			m_nEnd;				// �յ�����
	int			m_nSeatCount;		// ��λ����
	CNodeRect*	m_NodeRect;			// bEnable����ָ�룬��������·��bEnable����

	CustomList<int> listOpen;		// OPEN�б�
	CustomList<int> listClose;		// CLOSE�б�
};
#endif // __A_STAR_CLASS_H__
