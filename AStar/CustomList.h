/////////////////////////////////////////////////////////////////
/// @file CustomList.h
///
/// @purpose �Զ���������(������0��ʼ)
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

	//�ж������Ƿ�Ϊ��
	bool IsEmpty() const {return (NULL == m_pHead) ? true : false;}

	//���������е�Ԫ������
	int Length() const {return m_nLength;}

	// �����������Ч��
	bool CheckIndex(int nIndex) const {return (nIndex >= 0 && nIndex < m_nLength);}

	// Ѱ�������еĵ�i��Ԫ�أ������丳ֵ��obj������:�Ƿ��ҵ�
	bool Find(int nIndex, Type & obj) const;

	// Ѱ��obj���������obj���򷵻�obj�����������򷵻�-1
	int Search(const Type & obj) const;

	// ����obj����ʹ���Ϊ��i��Ԫ�أ����أ��Ƿ�ɹ�
	bool Insert(int nIndex, const Type & obj);

	// ����i��Ԫ�ص�ֵ�޸�Ϊobj
	bool Modify(int nIndex, const Type & obj);

	// �ѵ�i��Ԫ��ȡ��obj��Ȼ���������ɾ����i��Ԫ��
	bool Delete(int nIndex, Type & obj);

	// �������
	void Clear(void);

private:
	CustomNode<Type>*	m_pHead;	// �����е���ָ��
	int					m_nLength;	// �ܹ�����
};

#endif // __A_STAR_CUSTOM_LIST_H__
