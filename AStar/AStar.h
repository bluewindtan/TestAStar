/////////////////////////////////////////////////////////////////
/// @file AStar.h
///
/// @purpose A星算法类
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
	// 初始化：通过传入的参数
	bool InitData(int* pMap, int nWidth, int nLength, int nStart, int nEnd, int nFourDirection);
	// 初始化：加载数据文件
	bool LoadDataFile(const char* sFileName = DATA_FILE_NAME);
	// 保存数据文件
	bool SaveDataFile(const char* sFileName = DATA_FILE_NAME);
	// 销毁数据
	void DestroyData(void);

	// 寻路
	bool FindPath(void);

public:
	// 是否初始化
	bool GetInitialzed(void) const {return m_bInitialized;}

	// 设置座位信息
	bool SetSeatInfo(int nIndex, const char* sInfo);
	// 返回座位信息(注：需要删除返回的字符串指针)
	const char* GetSeatInfo(int nIndex) const;

	// 设置地图值
	bool SetMapValue(int nIndex, int nStatus);
	bool SetMapValue(int x, int y, int nStatus);
	// 得到地图值
	int GetMapValue(int x, int y) const;
	int GetMapValue(int nIndex) const;

	// 属性的读取
	int GetFourDirection(void) const {return m_nFourDirection;}
	void SetFourDirection(int nFourDirection) {m_nFourDirection = nFourDirection;}

	// 设置起点
	void SetStartIndex(int nIndex) {m_nStart = nIndex;}
	void SetStartIndex(int x, int y);
	bool GetStartIndex(int& x, int& y) const;
	// 设置终点
	void SetEndIndex(int nIndex) {m_nEnd = nIndex;}
	void SetEndIndex(int x, int y);
	bool GetEndIndex(int& x, int& y) const;

	// 判断是否有效索引
	bool CheckIndex(int nIndex) const;
	// 判断是否有效列索引
	bool CheckColumn(int nColumnIndex) const;
	// 判断是否有效行索引
	bool CheckLine(int nLineIndex) const;

	// 坐标与索引的互相转换
	int ConvertCoordinateToIndex(int x, int y) const;
	bool ConvertIndexToCoordinate(int nIndex, int& x, int& y) const;

	// 得到行数
	int GetLineCount(void) const {return m_nHeight;}
	// 得到列数
	int GetColumnCount(void) const {return m_nWidth;}
	// 得到地图bEnable矩形的总数量
	int GetTotalCount(void) const {return m_nWidth * m_nHeight;}

	// 查找最终结果：前一格
	int GetPreWay(int nIndex);
	// 确定当前格在前一格的哪个方向值(返回值：0 同一格；-1 无效；其他 具体的方向值；)
	int GetDirectionValueWith2Node(int nPre, int nCur);

	// 从各个方向查找最靠近的非障碍的格子
	int FindNearestUnblockNode(int nIndex, int nDirection = DirectionIndex_End, int nLayer = 1);

public:
	// 判断是否为障碍物
	static bool IsNodeBlock(int nValue);

	// 判断是否为有效行数
	static bool IsValidLine(int nLine) {return nLine > 0 && nLine <= ASTAR_LINE_MAX;}
	// 判断是否为有效列数
	static bool IsValidColumn(int nColumn) {return nColumn > 0 && nColumn <= ASTAR_COLUMN_MAX;}

	// 判断是否有效方向值
	static bool IsValidDirectionIndex(int nDirectionIndex) {return nDirectionIndex >= DirectionIndex_Begin && nDirectionIndex < DirectionIndex_End;}
	// 根据方向值得到方向的索引
	static int GetDirectionIndexByValue(int nDirectionValue);
	// 根据方向得到角度
	static int GetDegreeByDirectionIndex(int nDirectionIndex);
	// 根据角度得到欧拉角值
	static float GetEulerAnglesByDegree(int nDegree);

private:
	// 寻找下一个
	bool FindNext(void);
	// 获取结果,让最短路径的bEnable值修改为一条路径
	void GetResult(void);

	// 根据当前索引和扩展方向得出新索引
	int GetNewIndexByDirectionValue(int nCurIndex, int nDirectionValue, int nLayer = 1);
	// 从某个方向扩展bEnable矩形项，同时插入到Open列表中(若nResult返回1，表示找到最终点了)
	bool ExpandNodeRectByDirectionValue(int nCurIndex, int nDirectionValue, int& nResult);
	// 从各个方向扩展bEnable矩形项
	bool ExpandNodeRectAllDirection(int nCurIndex, int& nResult);

	// 获取H值
	int GetValueH(int i);
	// 获取G值
	int GetValueG(int i, int nDirectionValue);

	// 创建bEnable矩形，并且初始化之
	void CreateNodeRect(void);
	void InitNodeRect(void);
	void DestroyNodeRect(void);

	// 解析重要参数
	bool ParseImportantMember(char* sInfo);
	// 解析地图信息
	bool ParseMapInfo(int nLine, char* sInfo);
	// 解析座位信息
	bool ParseSeatInfo(char* sInfo);

	// 检查重要参数
	bool CheckImportantMember(void);

private:
	bool		m_bInitialized;		// 是否初始化过
	int			m_nFourDirection;	// bEnable矩形的扩展方向：1(非0)为四方向，0为八方向
	int			m_nWidth;			// 地图的列数
	int			m_nHeight;			// 地图的行数
	int			m_nStart;			// 起点索引
	int			m_nEnd;				// 终点索引
	int			m_nSeatCount;		// 座位数量
	CNodeRect*	m_NodeRect;			// bEnable矩形指针，用于最终路径bEnable链表

	CustomList<int> listOpen;		// OPEN列表
	CustomList<int> listClose;		// CLOSE列表
};
#endif // __A_STAR_CLASS_H__
