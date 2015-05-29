#include "CustomList.h"

template <class Type> 
CustomList<Type>::CustomList()
: m_nLength(0), m_pHead(NULL)
{
}

template <class Type> 
CustomList<Type>::~CustomList()
{
	if (m_pHead)
	{
		delete m_pHead;
		m_pHead = NULL;
	}
}

template <class Type> 
bool CustomList<Type>::Find(int nIndex, Type & obj) const
{
	if(! CheckIndex(nIndex))
	{
		return false;
	}
	// 首先从首指针开始
	CustomNode<Type>* temp = m_pHead;
	for (int i = 0; i < nIndex; i++)
	{
		temp = temp->next;
	}
	// 找到
	if (temp)
	{
		obj = temp->obj;
	}

	return (NULL == temp);
}

template <class Type> 
int CustomList<Type>::Search(const Type & obj) const
{
	CustomNode<Type>* temp = m_pHead;
	for (int i = 0; i < m_nLength; i++)
	{
		if (NULL == temp)
		{
			break;
		}
		// 判断是否同一个对象
		if(temp->obj == obj)
		{
			return i;
		}
		//
		temp = temp->next;
	}

	return -1;
}

template <class Type> 
bool CustomList<Type>::Insert(int nIndex, const Type & obj)
{
	//if (! CheckIndex(nIndex))
	//{
	//	return false;
	//}
	// 定位需要插入的位置
	CustomNode<Type>* tempPre = NULL;
	CustomNode<Type>* temp = m_pHead;
	for (int i = 0; i < nIndex; i++)
	{
		tempPre = temp;
		temp = temp->next;
	}
	// 创建新节点
	CustomNode<Type>* pNew = new CustomNode<Type>;
	if (NULL == pNew)
	{
		return false;
	}
	// 赋值
	pNew->obj = obj;
	pNew->next = temp;
	// 判断是否第一个
	if (tempPre)
	{
		tempPre->next = pNew;
	}
	else
	{
		m_pHead = pNew; // 插在最前面
	}
	// 增加计数
	m_nLength++;

	return true;
}

template <class Type> 
bool CustomList<Type>::Modify(int nIndex, const Type &obj)
{
	if (! CheckIndex(nIndex))
	{
		return false;
	}
	CustomNode<Type>* temp = m_pHead;
	for (int i = 0; i < nIndex; i++)
	{
		temp = temp->next;
	}
	if (temp)
	{
		temp->obj = obj;
	}
	return true;
}

template <class Type> 
bool CustomList<Type>::Delete(int nIndex, Type & obj)
{
	if (! CheckIndex(nIndex))
	{
		return false;
	}
	CustomNode<Type>* tempPre = NULL;
	CustomNode<Type>* temp = m_pHead;
	for (int i = 0; i < nIndex; i++)
	{
		tempPre = temp;
		temp = temp->next;
	}
	// 判断是否第一个
	if (tempPre)
	{
		if (temp)
		{
			tempPre->next = temp->next;
		}
		else
		{
			tempPre->next = NULL;
		}
	}
	else
	{
		if (temp)
		{
			m_pHead = temp->next;
		}
	}
	// 最后删除该指针
	if (temp)
	{
		obj = temp->obj;
		delete temp;
		temp = NULL;
	}
	m_nLength--;
	// 如果列表空了，则设置首指针为空
	if (0 == m_nLength)
	{
		m_pHead = NULL;
	}
	return true;
}

template <class Type> 
void CustomList<Type>::Clear(void)
{
	Type obj;
	while (!IsEmpty())
	{
		Delete(0, obj);
	}
}
