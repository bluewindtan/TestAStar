#include "NodeRect.h"
#include "AStarDefine.h"
#include <sstream>
using namespace std;

CNodeRect::CNodeRect()
: m_nID(0), m_nStatus(0), m_nMapX(0), m_nMapY(0), m_nValueG(0)
, m_nValueH(0), m_pNRPre(NULL), m_pNRLink(NULL)
{
}

CNodeRect::~CNodeRect()
{

}

void CNodeRect::ClearData(void)
{
	m_nID = 0;
	m_nStatus = 0;
	m_nMapX = 0;
	m_nMapY = 0;
	m_nValueG = 0;
	m_nValueH = 0;
	m_pNRPre = NULL;
	m_pNRLink = NULL;
}

const char* CNodeRect::GetSeatInfo(void) const
{
	if (NULL == m_pNRLink)
	{
		return NULL;
	}
	ostringstream ostrs;
	int nHead = m_nID;
	const CNodeRect* pNode = this;
	int nCount = 0;
	while (true)
	{
		ostrs << pNode->m_nID << DATA_DELIMITER;
		nCount++;
		//
		pNode = pNode->m_pNRLink;
		if (NULL == pNode)
		{
			break;
		}
		if (nHead == pNode->m_nID)
		{
			break; // 找到完整的座位信息了
		}
	}
	// 座位的方格最大数量
	if (nCount > SEAT_RECT_MAX)
	{
		return NULL; // error
	}
	// 最后别忘了加上一个
	ostrs << nHead << DATA_DELIMITER;

	// 需要创建字符串
	char* strSeat = new char[SEAT_TOTAL_LEN];
	strcpy_s(strSeat, SEAT_TOTAL_LEN, ostrs.str().c_str());

	return strSeat;
}

void CNodeRect::ClearSeat(void)
{
	CNodeRect* pNode = m_pNRLink;
	CNodeRect* pTemp = NULL;
	while (pNode)
	{
		pTemp = pNode->m_pNRLink;
		if (NULL == pTemp)
		{
			break;
		}
		pNode->m_pNRLink = NULL;
		pNode = pTemp;
	}
}

bool CNodeRect::CheckSeat(const char* sInfo)
{
	if (NULL == sInfo)
	{
		return false;
	}
	char sIndex[SEAT_TOTAL_LEN];
	sprintf_s(sIndex, SEAT_TOTAL_LEN, "%s%d%s", DATA_DELIMITER, m_nID, DATA_DELIMITER);
	char sTemp[SEAT_TOTAL_LEN];
	sprintf_s(sTemp, SEAT_TOTAL_LEN, "%s%s", DATA_DELIMITER, sInfo);
	// 查找一番
	char* pTemp = strstr(sTemp, sIndex);
	int nCount = 0;
	while (pTemp)
	{
		nCount++;
		//
		pTemp += strlen(sIndex) - strlen(DATA_DELIMITER);
		pTemp = strstr(pTemp, sIndex);
	}
	// 判断出现次数
	if (2 != nCount)
	{
		return false;
	}

	return true;
}

void CNodeRect::RefreshStatus(void)
{
	// 刷新状态
	if (NodeStatus_Block == m_nStatus)
	{
		m_nStatus = m_pNRLink ? NodeStatus_Block_Seat : NodeStatus_Block;
	}
	else if (NodeStatus_None == m_nStatus)
	{
		m_nStatus = m_pNRLink ? NodeStatus_None_Seat : NodeStatus_None;
	}
}

void CNodeRect::SetStatus(int nStatus)
{
	m_nStatus = nStatus;
	RefreshStatus();
}

void CNodeRect::SetLinkNode(CNodeRect* pLink)
{
	m_pNRLink = pLink;
	RefreshStatus();
}
