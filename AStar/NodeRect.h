/////////////////////////////////////////////////////////////////
/// @file NodeRect.h
///
/// @purpose 节点矩形类(路径中的每一个格子)
///
/// @author TanFeng
///
/// @date 05/03/2012
/// -------------------------------------------------------------
/// ChangeLog:
///
/// -------------------------------------------------------------
/// Copyright (C) 2012 - All Rights Reserved
/////////////////////////////////////////////////////////////////
#ifndef __A_STAR_NODE_RECT_H__
#define __A_STAR_NODE_RECT_H__

class CNodeRect
{
public:
	CNodeRect();
	~CNodeRect();

	// 清空数据
	void ClearData(void);

	// 返回座位信息(注：需要删除返回的字符串指针)
	const char* GetSeatInfo(void) const;

	// 清除座位信息
	void ClearSeat(void);

	// 检查以本节点为首的座位信息是否正确
	bool CheckSeat(const char* sInfo);

public:
	// 属性读取:节点状态
	void SetStatus(int nStatus);
	int GetStatus(void) const {return m_nStatus;}
	
	// 属性读取：设置关联的座位信息
	void SetLinkNode(CNodeRect* pLink);
	CNodeRect* GetLinkNode(void) {return m_pNRLink;}

protected:
	// 刷新节点状态
	void RefreshStatus(void);

public:
	int m_nID;				// ID，即总索引
	int m_nMapX;			// 地图中的x坐标
	int m_nMapY;			// 地图中的y坐标
	int m_nValueH;			// H值
	int m_nValueG;			// G值
	CNodeRect* m_pNRPre;	// 父节点指针

private:
	int m_nStatus;			// 节点状态
	CNodeRect* m_pNRLink;	// 关联的节点指针，表示座位相关
};
#endif // __A_STAR_NODE_RECT_H__
