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
	// ���ȴ���ָ�뿪ʼ
	CustomNode<Type>* temp = m_pHead;
	for (int i = 0; i < nIndex; i++)
	{
		temp = temp->next;
	}
	// �ҵ�
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
		// �ж��Ƿ�ͬһ������
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
	// ��λ��Ҫ�����λ��
	CustomNode<Type>* tempPre = NULL;
	CustomNode<Type>* temp = m_pHead;
	for (int i = 0; i < nIndex; i++)
	{
		tempPre = temp;
		temp = temp->next;
	}
	// �����½ڵ�
	CustomNode<Type>* pNew = new CustomNode<Type>;
	if (NULL == pNew)
	{
		return false;
	}
	// ��ֵ
	pNew->obj = obj;
	pNew->next = temp;
	// �ж��Ƿ��һ��
	if (tempPre)
	{
		tempPre->next = pNew;
	}
	else
	{
		m_pHead = pNew; // ������ǰ��
	}
	// ���Ӽ���
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
	// �ж��Ƿ��һ��
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
	// ���ɾ����ָ��
	if (temp)
	{
		obj = temp->obj;
		delete temp;
		temp = NULL;
	}
	m_nLength--;
	// ����б���ˣ���������ָ��Ϊ��
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
