/////////////////////////////////////////////////////////////////
/// @file NodeRect.h
///
/// @purpose �ڵ������(·���е�ÿһ������)
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

	// �������
	void ClearData(void);

	// ������λ��Ϣ(ע����Ҫɾ�����ص��ַ���ָ��)
	const char* GetSeatInfo(void) const;

	// �����λ��Ϣ
	void ClearSeat(void);

	// ����Ա��ڵ�Ϊ�׵���λ��Ϣ�Ƿ���ȷ
	bool CheckSeat(const char* sInfo);

public:
	// ���Զ�ȡ:�ڵ�״̬
	void SetStatus(int nStatus);
	int GetStatus(void) const {return m_nStatus;}
	
	// ���Զ�ȡ�����ù�������λ��Ϣ
	void SetLinkNode(CNodeRect* pLink);
	CNodeRect* GetLinkNode(void) {return m_pNRLink;}

protected:
	// ˢ�½ڵ�״̬
	void RefreshStatus(void);

public:
	int m_nID;				// ID����������
	int m_nMapX;			// ��ͼ�е�x����
	int m_nMapY;			// ��ͼ�е�y����
	int m_nValueH;			// Hֵ
	int m_nValueG;			// Gֵ
	CNodeRect* m_pNRPre;	// ���ڵ�ָ��

private:
	int m_nStatus;			// �ڵ�״̬
	CNodeRect* m_pNRLink;	// �����Ľڵ�ָ�룬��ʾ��λ���
};
#endif // __A_STAR_NODE_RECT_H__
