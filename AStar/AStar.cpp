#include "AStar.h"
#include "NodeRect.h"
#include "CustomList.cpp"

#include <math.h>
#include <fstream>
#include <map>
using namespace std;

// ��һ��
const char* g_sFirstLine = "// astar algorithm's config file";
// �ڶ��е����ݸ���
const int PARSE_MEMBER_COUNT = 6;

//
CAStar::CAStar(void)
: m_bInitialized(false), m_nFourDirection(0), m_nWidth(0), m_nHeight(0), m_nStart(0)
, m_nEnd(0), m_nSeatCount(0), m_NodeRect(NULL)
{
}

CAStar::~CAStar(void)
{
	DestroyData();

}

bool CAStar::InitData(int* pMap, int nWidth, int nLength, int nStart, int nEnd, int nFourDirection)
{
	if (GetInitialzed())
	{
		return false; // �Ѿ���ʼ�����ˣ�����ʧ��
	}
	// ��ֵ
	m_nFourDirection = nFourDirection;
	m_nWidth = nWidth;
	m_nHeight = nLength;
	m_nStart = nStart;
	m_nEnd = nEnd;
	m_nSeatCount = 0;

	// �����Ҫ����
	if (!CheckImportantMember())
	{
		return false;
	}

	// ����bEnable����
	CreateNodeRect();
	InitNodeRect();
	// ��ͼ��ֵ
	if (pMap)
	{
		for(int nIndex = 0; nIndex < GetTotalCount(); nIndex++)
		{
			m_NodeRect[nIndex].SetStatus( *(pMap + nIndex) );
		}
	}

	// ����Ϊ��ʼ��
	m_bInitialized = true;
	return true;
}

bool CAStar::LoadDataFile(const char* sFileName /* = DATA_FILE_NAME */)
{
	if (GetInitialzed())
	{
		return false;
	}
	// ���ļ�
	ifstream fs;
	fs.open(sFileName);
	if (!fs.is_open())
	{
		return false;
	}

	// �ж��ַ��������Ƿ����
	if (STRING_LEN_M <= ASTAR_COLUMN_MAX * 2)
	{
		return false;
	}
	// ��ȡ�ļ�
	char sTemp[STRING_LEN_M];
	int i = -1;			// ����û�м���
	int nLine = 0;		// ������
	int nSeatCount = 0; // �����е���λ����
	while (!fs.eof())
	{
		fs.getline(sTemp, STRING_LEN_M, LINE_ENDER[0]);
		if (0 == nLine) // ע��
		{
			// У��
			if (0 != strcmp(sTemp, g_sFirstLine))
			{
				break;
			}
		}
		else if(1 == nLine) //  ��Ҫ����
		{
			if (!ParseImportantMember(sTemp))
			{
				break;
			}
			i = 0; // ���õ�ͼ���ݵ�����������
		}
		else // ���ǵ�ͼ���ݰ�
		{
			// �������
			if (i < m_nHeight) // ��ͼ��Ϣ����
			{
				if (0 == i)
				{
					// ������ͼ
					CreateNodeRect();
					InitNodeRect();
				}
				// ���õ�ͼ��Ϣ
				if (!ParseMapInfo(i, sTemp))
				{
					break;
				}
			}
			else // ��λ��Ϣ����
			{
				// ������λ��Ϣ
				if (!ParseSeatInfo(sTemp))
				{
					break;
				}
				nSeatCount++;
			}
			i++;
		}
		nLine++;
	}
	fs.close();

	// �ж��Ƿ����ʧ�ܹ�
	if (i != m_nHeight + m_nSeatCount || nSeatCount != m_nSeatCount)
	{
		// �ж��Ƿ񴴽����ڵ�
		if (i >= 0)
		{
			DestroyNodeRect(); // ɾ����λ
		}
		return false;
	}


	// �����Ѿ���ʼ��
	m_bInitialized = true;
	return true;
}

bool CAStar::SaveDataFile(const char* sFileName /* = DATA_FILE_NAME */)
{
	if (!GetInitialzed())
	{
		return false;
	}
	// ���ļ�
	ofstream fs;
	fs.open(sFileName);
	if (!fs.is_open())
	{
		return false;
	}
	// �������������
	fs.clear();

	// �������λ��Ϣ
	int nCur = -1;
	int nNext = -1;
	int nHead = -1;
	map< int, int > mapNodeRect;
	map< int, int >::iterator it;
	//m_nSeatCount = 0;
	for(int i = 0; i < GetTotalCount(); i++)
	{
		// �������λ��Ϣ����ͳ������
		if (m_NodeRect[i].GetLinkNode())
		{
			// �жϸ���λ�����Ƿ��Ѿ����뼯������
			it = mapNodeRect.find(i);
			if (it != mapNodeRect.end())
			{
				continue; // ������������һ��
			}
			// ˵���Ѿ��ҵ�һ��������λ��ͷ����
			//m_nSeatCount++;
			nCur = i;
			// 
			while (true)
			{
				// ���뼯��
				mapNodeRect.insert(make_pair(nCur, 0));
				// ���ͷ����
				if (-1 == nHead)
				{
					nHead = nCur;
				}
				else
				{
					// һֱ�ҵ�ͷ����Ϊֹ
					if (nCur == nHead)
					{
						break; // ����ľ���������
					}
				}
				// ����������
				if (NULL == m_NodeRect[nCur].GetLinkNode())
				{
					break;
				}
				nNext = m_NodeRect[nCur].GetLinkNode()->m_nID;
				// ���������Ч��
				if (!CheckIndex(nNext))
				{
					break;
				}
				nCur = nNext;
			}
			nHead = -1;
		}
	}

	// ��һ��: ע��
	//fs.write(sFirstLine, (streamsize)strlen(sFirstLine));
	fs << g_sFirstLine << LINE_ENDER;

	// �ڶ��У���ͼ��������������㡢�յ㡢�Ƿ��ķ���
	fs << m_nWidth << DATA_DELIMITER
		<< m_nHeight << DATA_DELIMITER
		<< m_nStart << DATA_DELIMITER
		<< m_nEnd << DATA_DELIMITER
		<< m_nFourDirection << DATA_DELIMITER
		<< m_nSeatCount << DATA_DELIMITER
		<< LINE_ENDER;

	// ������-��N�У���ͼ������Ϣ������ΪN-2
	for(int i = 0; i < m_nHeight; i++)
	{
		for(int j = 0; j < m_nWidth; j++)
		{
			fs << (CAStar::IsNodeBlock(m_NodeRect[ConvertCoordinateToIndex(j, i)].GetStatus()) ? NodeStatus_Block : NodeStatus_None) << DATA_DELIMITER;
		}
		fs << LINE_ENDER;
	}

	// ��N+1��-�ļ���β����λ��Ϣ������Ϊm_nSeatCount
	nCur = -1;
	nNext = -1;
	nHead = -1;
	map< int, int >::iterator itModiy;
	for (it = mapNodeRect.begin(); it != mapNodeRect.end(); ++it)
	{
		// �������Ƿ�д����ļ�
		if (1 == it->second)
		{
			continue; // ������һ��
		}
		// �õ���һ����д���ļ�
		nCur = it->first;
		// 
		while (true)
		{
			// д���ļ�
			fs << nCur << DATA_DELIMITER;
			mapNodeRect[nCur] = 1;
			// ����ͷ����
			if (-1 == nHead)
			{
				nHead = nCur;
			}
			else
			{
				// һֱ�ҵ�ͷ����Ϊֹ
				if (nCur == nHead)
				{
					break; // ����ľ���������
				}
			}
			// ����������λָ��
			if (NULL == m_NodeRect[nCur].GetLinkNode())
			{
				break;
			}
			// ���������Ч��
			nNext = m_NodeRect[nCur].GetLinkNode()->m_nID;
			if (!CheckIndex(nNext))
			{
				break;
			}
			nCur = nNext;
		}
		// ˵��һ��������λ��д����
		fs << LINE_ENDER;
		nCur = -1;
		nNext = -1;
		nHead = -1;
	}

	// �ر��ļ�
	fs.close();
	return true;
}

void CAStar::DestroyData(void)
{
	if (!GetInitialzed())
	{
		return;
	}
	// ���bEnable�����ڴ�
	DestroyNodeRect();

	// 
	m_nWidth = 0;
	m_nHeight = 0;
	m_nStart = 0;
	m_nEnd = 0;
	listClose.Clear();
	listOpen.Clear();

	// ���δ��ʼ��
	m_bInitialized = false;
}

int CAStar::GetMapValue(int x, int y) const
{
	return GetMapValue(ConvertCoordinateToIndex(x, y));
}

int CAStar::GetMapValue(int nIndex) const
{
	int nValue = NodeStatus_End;

	if (m_NodeRect)
	{
		if (CheckIndex(nIndex))
		{
			nValue = m_NodeRect[nIndex].GetStatus();
		}
	}

	return nValue;
}



bool CAStar::CheckIndex(int nIndex) const
{
	return nIndex >=0 && nIndex < GetTotalCount();
}

int CAStar::GetNewIndexByDirectionValue(int nCurIndex, int nDirectionValue, int nLayer /* = 1 */)
{
	int nNewIndex = nCurIndex;
	// �����
	if (nLayer < 1)
	{
		return -1;
	}

	// ���������Ƿ��ڱ߽߱ǽ�
	int x = -1;
	int y = -1;
	if (! ConvertIndexToCoordinate(nCurIndex, x, y))
	{
		return -1;
	}

	// ��
	if (nDirectionValue & DirectionValue_Top)
	{
		// ��鲻�ܴ��ڵ�һ��
		if (0 == y)
		{
			return -1;
		}
		nNewIndex -= m_nWidth * nLayer;
	}
	// ��
	if (nDirectionValue & DirectionValue_Bottom)
	{
		// ��鲻�ܴ������һ��
		if (m_nHeight - 1 == y)
		{
			return -1;
		}
		nNewIndex += m_nWidth * nLayer;
	}
	// ��
	if (nDirectionValue & DirectionValue_Left)
	{
		// ��鲻�ܴ��ڵ�һ��
		if (0 == x)
		{
			return -1;
		}
		nNewIndex -= 1 * nLayer;
	}
	// ��
	if (nDirectionValue & DirectionValue_Right)
	{
		// ��鲻�ܴ������һ��
		if (m_nWidth - 1 == x)
		{
			return -1;
		}
		nNewIndex += 1 * nLayer;
	}

	return nNewIndex;
}

bool CAStar::ExpandNodeRectByDirectionValue(int nCurIndex, int nDirectionValue, int& nResult)
{
	// Ĭ�Ͻ��
	nResult = ExpandResult_None;
	// ��鵱ǰ����
	if(! CheckIndex(nCurIndex))
	{
		return false;
	}
	// ���ݷ���ó�������
	int nNewIndex = GetNewIndexByDirectionValue(nCurIndex, nDirectionValue);
	// ���������
	if (! CheckIndex(nNewIndex))
	{
		return false;
	}
	if(CAStar::IsNodeBlock(m_NodeRect[nNewIndex].GetStatus()))
	{
		return false;
	}
	// ����Ѿ��ҵ�����bEnable������������ˣ�ֱ�ӷ���
	if(nNewIndex == m_nEnd)
	{
		// ����bEnable�����б�
		m_NodeRect[nNewIndex].m_pNRPre = &m_NodeRect[nCurIndex];
		nResult = ExpandResult_Final;
		return true;
	}

	// ����Close�б�
	if(-1 == listClose.Search(nNewIndex))
	{
		// ����Close�б��е�ʱ��
		// ����Open�б�
		if(-1 == listOpen.Search(nNewIndex))
		{
			// ����bEnable�����б�
			m_NodeRect[nNewIndex].m_pNRPre = &m_NodeRect[nCurIndex];
			m_NodeRect[nNewIndex].m_nValueG = GetValueG(nNewIndex, nDirectionValue);
			m_NodeRect[nNewIndex].m_nValueH = GetValueH(nNewIndex);
			// ���ڣ�����뵽open�б�
			listOpen.Insert(listOpen.Length(), nNewIndex);
		}
		else
		{
			// ��open���У��ж�Gֵ�Ƿ���Ҫ����
			if (m_NodeRect[nNewIndex].m_nValueG > GetValueG(nNewIndex, nDirectionValue))
			{
				// ��������bEnable�����б�
				m_NodeRect[nNewIndex].m_pNRPre = &m_NodeRect[nCurIndex];
				m_NodeRect[nNewIndex].m_nValueG = GetValueG(nNewIndex, nDirectionValue);
			}
			else
			{
				// ����ʲôҲ����
			}
		}
	}

	return true;
}

bool CAStar::ExpandNodeRectAllDirection(int nCurIndex, int& nResult)
{
	// ѭ������������չ
	for (int i = DirectionIndex_Begin; i < DirectionIndex_End; i++)
	{
		// ������ķ�������Ҫ�ж��Ƿ��ĸ�������
		if (GetFourDirection())
		{
			if (0 == (i % 2))
			{
				continue; // б�ķ��򲻹���
			}
		}
		// ��չ�÷���
		if (ExpandNodeRectByDirectionValue(nCurIndex, g_arrDirectionValue[i], nResult))
		{
			// ��չ�ɹ������ҽ��Ϊ���յ㣬�����������չ
			if (ExpandResult_Final == nResult)
			{
				return true; // �˳�
			}
		}
	}


	return true;
}

int CAStar::GetValueH(int i)
{
	if (0 == m_nWidth)
	{
		return -1;
	}
	return (abs(m_nEnd / m_nWidth - i / m_nWidth) + abs(m_nEnd % m_nWidth - i % m_nWidth)) * g_nGridValue;
}

int CAStar::GetValueG(int i, int nDirectionValue)
{
	int nValue = g_nGridValue;
	if (m_NodeRect[i].m_pNRPre)
	{
		nValue += m_NodeRect[i].m_pNRPre->m_nValueG;
	}
	// �Ƿ�б�ķ���
	bool bOblique = false; // Ĭ�ϲ���б�ķ���
	for (int j = DirectionIndex_Begin; j < DirectionIndex_End; j++)
	{
		// ������ķ�������Ҫ�ж��Ƿ��ĸ�������
		if (g_arrDirectionValue[j] == nDirectionValue)
		{
			if (0 == (j % 2))
			{
				bOblique = true; // �����ǰ�ĸ���Ϊб�ķ���
				break;
			}
		}
	}
	// �����б�ķ���
	if (bOblique)
	{
		nValue += (int)(g_nGridValue * 0.414f); // �Խ���
	}

	return nValue;
}

bool CAStar::FindPath(void)
{
	if (!GetInitialzed())
	{
		return false;
	}
	// ���ϴν���������
	listClose.Clear();
	listOpen.Clear();
	// ��λ��Ϣ
	InitNodeRect();

	// ��������յ�
	if (m_nStart == m_nEnd
		|| !CheckIndex(m_nStart)
		|| !CheckIndex(m_nEnd)
		)
	{
		return false;
	}
	// Ѱ·��ʼ
	if (!FindNext())
	{
		return false;
	}

	// ȡ�ý��
	GetResult();

	return true;
}


bool CAStar::FindNext(void)
{
	int nTemp = 0;
	int nResult = 0;
	// ���close�б�Ϊ��
	if(listClose.IsEmpty())
	{
		// ����ʼ����뵽Close�б���
		listClose.Insert(listClose.Length(), m_nStart);

		// ��Ҫ������ʼ��ĸ����������bEnable
		if (ExpandNodeRectAllDirection(m_nStart, nResult))
		{
			if (ExpandResult_Final == nResult)
			{
				return true; // �˳�
			}
		}
	}

	// �ҳ�Open�б�����С����
	int nFindIndex = -1;		// �ҵ�������
	int nMin = 0;				// ��Сֵ
	int nNodeRectIndex = -1;	// bEnable���ε�����
	int nIndexTemp = -1;
	int nValueF = -1;
	for(int i = 0; i < listOpen.Length(); i++)
	{
		// �õ�bEnable���ε�����
		listOpen.Find(i, nIndexTemp);
		// ����Fֵ
		nValueF = m_NodeRect[nIndexTemp].m_nValueH +  m_NodeRect[nIndexTemp].m_nValueG;
		// Ĭ�ϵ�һ��Ϊ��Сֵ���ȽϺ�������Сֵ
		if(0 == i || nValueF <= nMin)
		{
			nMin = nValueF;
			nFindIndex = i;
			nNodeRectIndex = nIndexTemp;
		}
	}
	if(-1 == nFindIndex)
	{
		return false; // ��Ͷ��·��Ѱ·ʧ����
	}
	// �Ѹ���Сֵ��Open�б���ɾ��
	listOpen.Delete(nFindIndex, nTemp);
	// �Ѹ���Сֵ���뵽Close�б���
	listClose.Insert(listClose.Length(), nNodeRectIndex);

	// ����Ӹ�������ʼ�����������������bEnable
	if (ExpandNodeRectAllDirection(nNodeRectIndex, nResult))
	{
		if (ExpandResult_Final == nResult)
		{
			return true; // �˳�
		}
	}

	// �����ݹ�
	return FindNext();
}

void CAStar::GetResult(void)
{
	CNodeRect *p = &m_NodeRect[m_nEnd];
	while(p)
	{
		p->SetStatus(NodeStatus_Way);
		if (p->m_nID == m_nStart)
		{
			break;
		}
		p = p->m_pNRPre;
	}
}

void CAStar::CreateNodeRect(void)
{
	m_NodeRect = new CNodeRect[m_nWidth*m_nHeight];
}

void CAStar::InitNodeRect(void)
{
	if (NULL == m_NodeRect)
	{
		return; // error
	}
	int x = 0;
	int y = 0;
	for(int i = 0; i < GetTotalCount(); i++)
	{
		//m_NodeRect[i].ClearData();
		m_NodeRect[i].m_nID = i;
		if (!ConvertIndexToCoordinate(i, x, y))
		{
			continue; // error
		}
		m_NodeRect[i].m_nMapX = x;
		m_NodeRect[i].m_nMapY = y;
		m_NodeRect[i].m_nValueG = 0;
		m_NodeRect[i].m_nValueH = 0;
		m_NodeRect[i].m_pNRPre = NULL;
		if (NodeStatus_Way == m_NodeRect[i].GetStatus())
		{
			m_NodeRect[i].SetStatus(NodeStatus_None);
		}
	}
}

void CAStar::DestroyNodeRect(void)
{
	if (m_NodeRect)
	{
		delete[] m_NodeRect;
		m_NodeRect = NULL;
	}
}

int CAStar::ConvertCoordinateToIndex(int x, int y) const
{
	if (!CheckColumn(x) || !CheckLine(y))
	{
		return -1;
	}

	return y * m_nWidth + x;
}

bool CAStar::ConvertIndexToCoordinate(int nIndex, int& x, int& y) const
{
	x = -1;
	y = -1;
	if (0 == m_nWidth)
	{
		return false;
	}
	y = nIndex / m_nWidth;
	if (!CheckLine(y))
	{
		return false;
	}
	x = nIndex % m_nWidth;
	if (!CheckColumn(x))
	{
		return false;
	}

	return true;
}

void CAStar::SetStartIndex(int x, int y)
{
	m_nStart = ConvertCoordinateToIndex(x, y);
}

bool CAStar::GetStartIndex(int& x, int& y) const
{
	return ConvertIndexToCoordinate(m_nStart, x, y);
}

void CAStar::SetEndIndex(int x, int y)
{
	m_nEnd = ConvertCoordinateToIndex(x, y);
}

bool CAStar::GetEndIndex(int& x, int& y) const
{
	return ConvertIndexToCoordinate(m_nEnd, x, y);
}

bool CAStar::SetMapValue(int nIndex, int nStatus)
{
	if (NULL == m_NodeRect)
	{
		return false;
	}
	if (nStatus < NodeStatus_Begin || nStatus >= NodeStatus_End)
	{
		return false;
	}
	m_NodeRect[nIndex].SetStatus(nStatus);
	return true;
}

bool CAStar::SetMapValue(int x, int y, int nStatus)
{
	return SetMapValue(ConvertCoordinateToIndex(x, y), nStatus);
}

bool CAStar::SetSeatInfo(int nIndex, const char* sInfo)
{
	if (NULL == m_NodeRect)
	{
		return false;
	}
	if (NULL == sInfo)
	{
		return false;
	}
	// �����ԭ������λ
	if (m_NodeRect[nIndex].GetLinkNode())
	{
		m_NodeRect[nIndex].ClearSeat();
		m_nSeatCount--; // ��������
	}

	// ���һ��
	bool bReturn = true;
	if (m_NodeRect[nIndex].CheckSeat(sInfo))
	{
		// ������λ��Ϣ
		char sTemp[SEAT_TOTAL_LEN];
		strcpy_s(sTemp, SEAT_TOTAL_LEN, sInfo);
		bReturn = ParseSeatInfo(sTemp);
	}

	// ������µ���λ��
	if (m_NodeRect[nIndex].GetLinkNode())
	{
		m_nSeatCount++; // ��������
	}

	return bReturn;
}

bool CAStar::ParseSeatInfo(char* sInfo)
{
	if (NULL == m_NodeRect)
	{
		return false;
	}
	if (NULL == sInfo)
	{
		return false;
	}
	// ����ַ���
	if (strlen(sInfo) >= SEAT_TOTAL_LEN)
	{
		return false;
	}
	// 
	char* pTemp = NULL;
	char* next_token = NULL;
	// �����ַ���
	pTemp = strtok_s(sInfo, DATA_DELIMITER, &next_token);
	if (NULL == pTemp)
	{
		return false;
	}
	int nPre = -1;
	int nCur = -1;
	int nHead = -1;
	int nCount = 0;
	while (pTemp)
	{
		nCur = atoi(pTemp);
		if (!CheckIndex(nCur))
		{
			break;
		}
		// ��¼ͷ����
		if (-1 == nPre)
		{
			nHead = nCur;
		}
		else
		{
			if (nPre == nCur)
			{
				break; // ͬһ�������������֣���������
			}
			// ����λ��ϵ����
			m_NodeRect[nPre].SetLinkNode( &m_NodeRect[nCur] );
			nCount++;
			// �ص�ͷ������ʱ�򣬾�������ˣ���������ݿ���������
			if (nCur == nHead)
			{
				break;
			}
		}
		//
		nPre = nCur;
		pTemp = strtok_s(NULL, DATA_DELIMITER, &next_token);
	}
	// �����������
	if (nCount > SEAT_RECT_MAX)
	{
		return false;
	}
	// ������ݵ�������
	if (nCur != nHead)
	{
		return false; // ˵��û���γ�һ��������λ
	}

	return true;
}

const char* CAStar::GetSeatInfo(int nIndex) const
{
	if (!CheckIndex(nIndex))
	{
		return NULL;
	}
	return m_NodeRect[nIndex].GetSeatInfo();
}

bool CAStar::ParseImportantMember(char* sInfo)
{
	if (NULL == sInfo)
	{
		return false;
	}
	// 
	char* pTemp = NULL;
	char* next_token = NULL;
	// �����ַ���
	pTemp = strtok_s(sInfo, DATA_DELIMITER, &next_token);
	if (NULL == pTemp)
	{
		return false;
	}
	int j = 0;
	int* arrValue[] = {&m_nWidth, &m_nHeight, &m_nStart, &m_nEnd, &m_nFourDirection, &m_nSeatCount};
	while (pTemp)
	{
		if (j < PARSE_MEMBER_COUNT)
		{
			*(arrValue[j]) = atoi(pTemp);
		}
		pTemp = strtok_s(NULL, DATA_DELIMITER, &next_token);
		j++;
	}
	// �ж����������Ƿ���ȷ
	if (PARSE_MEMBER_COUNT != j)
	{
		return false;
	}

	// �����Ҫ����
	if (!CheckImportantMember())
	{
		return false;
	}

	return true;
}

bool CAStar::CheckImportantMember(void)
{
	// �ж�����
	if (!CAStar::IsValidColumn(m_nWidth))
	{
		return false;
	}
	// �ж�����
	if (!CAStar::IsValidLine(m_nHeight))
	{
		return false;
	}
	// �ж������յ�
	if (!CheckIndex(m_nStart) || !CheckIndex(m_nEnd) || m_nStart == m_nEnd)
	{
		return false;
	}

	return true;
}

bool CAStar::ParseMapInfo(int nLine, char* sInfo)
{
	if (NULL == m_NodeRect)
	{
		return false;
	}
	if (NULL == sInfo)
	{
		return false;
	}
	// 
	char* pTemp = NULL;
	char* next_token = NULL;
	// �����ַ���
	pTemp = strtok_s(sInfo, DATA_DELIMITER, &next_token);
	if (NULL == pTemp)
	{
		return false;
	}
	int nColumn = 0; // ÿ�������趨������
	while (pTemp)
	{
		if (nColumn < m_nWidth)
		{
			m_NodeRect[ConvertCoordinateToIndex(nColumn, nLine)].SetStatus( atoi(pTemp) );
		}
		nColumn++;
		pTemp = strtok_s(NULL, DATA_DELIMITER, &next_token);
	}
	// ���ÿһ�е�����
	if (nColumn != m_nWidth)
	{
		return false;
	}

	return true;
}

bool CAStar::IsNodeBlock(int nValue)
{
	return NodeStatus_Block == nValue || NodeStatus_Block_Seat == nValue;
}

int CAStar::GetPreWay(int nIndex)
{
	int nPre = -1;

	if (!CheckIndex(nIndex))
	{
		return nPre;
	}

	CNodeRect* pNRPre = m_NodeRect[nIndex].m_pNRPre;
	if (pNRPre)
	{
		nPre = pNRPre->m_nID;
	}

	return nPre;
}

int CAStar::GetDirectionValueWith2Node(int nPre, int nCur)
{
	if (nPre == nCur)
	{
		return 0;
	}
	if (!CheckIndex(nPre) || !CheckIndex(nCur))
	{
		return -1;
	}
	int nPreX = 0;
	int nPreY = 0;
	if (!ConvertIndexToCoordinate(nPre, nPreX, nPreY))
	{
		return -1;
	}
	int nCurX = 0;
	int nCurY = 0;
	if (!ConvertIndexToCoordinate(nCur, nCurX, nCurY))
	{
		return -1;
	}
	int nDirectionValue = 0;
	// ȷ��X����
	if (nCurX < nPreX)
	{
		nDirectionValue |= DirectionValue_Left;
	}
	else if (nCurX > nPreX)
	{
		nDirectionValue |= DirectionValue_Right;
	}
	// ȷ��Y����
	if (nCurY < nPreY)
	{
		nDirectionValue |= DirectionValue_Top;
	}
	else if (nCurY > nPreY)
	{
		nDirectionValue |= DirectionValue_Bottom;
	}

	return nDirectionValue;
}

bool CAStar::CheckColumn(int nColumnIndex) const
{
	if (nColumnIndex < 0 || nColumnIndex >= m_nWidth)
	{
		return false;
	}

	return true;
}

bool CAStar::CheckLine(int nLineIndex) const
{
	if (nLineIndex < 0 || nLineIndex >= m_nHeight)
	{
		return false;
	}

	return true;
}

int CAStar::GetDirectionIndexByValue(int nDirectionValue)
{
	int nDirctionIndex = DirectionIndex_End;

	for (int i=DirectionIndex_Begin; i<DirectionIndex_End; i++)
	{
		if (nDirectionValue == g_arrDirectionValue[i])
		{
			nDirctionIndex = i;
			break;
		}
	}

	return nDirctionIndex;
}

int CAStar::GetDegreeByDirectionIndex(int nDirectionIndex)
{
	if (!IsValidDirectionIndex(nDirectionIndex))
	{
		return 0;
	}
	return (nDirectionIndex+1) * 45;
}

float CAStar::GetEulerAnglesByDegree(int nDegree)
{
	float fAngles = 0.0f;
	// �ȰѽǶȲ������ĽǶ�
	while (nDegree < 0)
	{
		nDegree += 360;
	}
	// �Ѷ���ĽǶ�ȥ��
	nDegree = nDegree % 360;
	// ���ֿ���
	if (nDegree > 180)
	{
		fAngles = (float)(nDegree - 360);
	}
	else
	{
		fAngles = (float)nDegree;
	}
	// ����
	fAngles /= 180.0f;
	fAngles *= ASTAR_PI;

	return fAngles;
}

int CAStar::FindNearestUnblockNode(int nIndex, int nDirection /* = DirectionIndex_End */, int nLayer /* = 1 */)
{
	int nValidIndex = -1;

	// �������ֵ��Ч����ֻ����һ��
	if (CAStar::IsValidDirectionIndex(nDirection))
	{
		int nTempIndex = GetNewIndexByDirectionValue(nIndex, g_arrDirectionValue[nDirection], nLayer);
		// ���������Ч��
		if (CheckIndex(nTempIndex))
		{
			// ���������Ƿ��ϰ���
			if (!CAStar::IsNodeBlock(GetMapValue(nTempIndex)))
			{
				nValidIndex = nTempIndex;
			}
		}
	}
	else
	{
		for (int j = 1; j <= FIND_NEAREST_LAYER; j++)
		{
			// ѭ������������չ
			for (int i = DirectionIndex_Begin; i < DirectionIndex_End; i++)
			{
				// ������ķ�������Ҫ�ж��Ƿ��ĸ�������
				if (GetFourDirection())
				{
					if (0 == (i % 2))
					{
						continue; // б�ķ��򲻹���
					}
				}
				// �ó�������������ֵ
				nValidIndex = FindNearestUnblockNode(nIndex, i, j);
				if (-1 <= nValidIndex)
				{
					break;
				}
			}
		}
	}

	return nValidIndex;
}
