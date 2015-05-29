/////////////////////////////////////////////////////////////////
/// @file CustomList.h
///
/// @purpose 自定义链表类(索引从0开始)
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
#ifndef __A_STAR_CUSTOM_LIST_H__
#define __A_STAR_CUSTOM_LIST_H__
#include "AStarDefine.h"

template <class Type> 
class CustomList;

template <class Type> 
class CustomNode
{
	friend CustomList<Type>;

	Type		obj;
	CustomNode*	next;
};

template <class Type>
class CustomList
{
public:
	CustomList();
	~CustomList();

	//判断链表是否为空
	bool IsEmpty() const {return (NULL == m_pHead) ? true : false;}

	//返回链表中的元素总数
	int Length() const {return m_nLength;}

	// 检查索引的有效性
	bool CheckIndex(int nIndex) const {return (nIndex >= 0 && nIndex < m_nLength);}

	// 寻找链表中的第i个元素，并将其赋值于obj，返回:是否找到
	bool Find(int nIndex, Type & obj) const;

	// 寻找obj，如果发现obj，则返回obj的索引，否则返回-1
	int Search(const Type & obj) const;

	// 插入obj，并使其成为第i个元素，返回：是否成功
	bool Insert(int nIndex, const Type & obj);

	// 将第i个元素的值修改为obj
	bool Modify(int nIndex, const Type & obj);

	// 把第i个元素取至obj，然后从链表中删除第i个元素
	bool Delete(int nIndex, Type & obj);

	// 清除数据
	void Clear(void);

private:
	CustomNode<Type>*	m_pHead;	// 链表中的首指针
	int					m_nLength;	// 总共长度
};

#endif // __A_STAR_CUSTOM_LIST_H__
