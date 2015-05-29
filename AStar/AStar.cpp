#include "AStar.h"
#include "NodeRect.h"
#include "CustomList.cpp"

#include <math.h>
#include <fstream>
#include <map>
using namespace std;

// 第一行
const char* g_sFirstLine = "// astar algorithm's config file";
// 第二行的数据个数
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
		return false; // 已经初始化过了，返回失败
	}
	// 赋值
	m_nFourDirection = nFourDirection;
	m_nWidth = nWidth;
	m_nHeight = nLength;
	m_nStart = nStart;
	m_nEnd = nEnd;
	m_nSeatCount = 0;

	// 检查重要参数
	if (!CheckImportantMember())
	{
		return false;
	}

	// 创建bEnable矩形
	CreateNodeRect();
	InitNodeRect();
	// 地图赋值
	if (pMap)
	{
		for(int nIndex = 0; nIndex < GetTotalCount(); nIndex++)
		{
			m_NodeRect[nIndex].SetStatus( *(pMap + nIndex) );
		}
	}

	// 设置为初始化
	m_bInitialized = true;
	return true;
}

bool CAStar::LoadDataFile(const char* sFileName /* = DATA_FILE_NAME */)
{
	if (GetInitialzed())
	{
		return false;
	}
	// 打开文件
	ifstream fs;
	fs.open(sFileName);
	if (!fs.is_open())
	{
		return false;
	}

	// 判断字符串长度是否合理
	if (STRING_LEN_M <= ASTAR_COLUMN_MAX * 2)
	{
		return false;
	}
	// 读取文件
	char sTemp[STRING_LEN_M];
	int i = -1;			// 索引没有激活
	int nLine = 0;		// 总行数
	int nSeatCount = 0; // 数据中的座位数量
	while (!fs.eof())
	{
		fs.getline(sTemp, STRING_LEN_M, LINE_ENDER[0]);
		if (0 == nLine) // 注释
		{
			// 校验
			if (0 != strcmp(sTemp, g_sFirstLine))
			{
				break;
			}
		}
		else if(1 == nLine) //  重要参数
		{
			if (!ParseImportantMember(sTemp))
			{
				break;
			}
			i = 0; // 设置地图数据的索引激活了
		}
		else // 都是地图数据啊
		{
			// 检查行数
			if (i < m_nHeight) // 地图信息部分
			{
				if (0 == i)
				{
					// 创建地图
					CreateNodeRect();
					InitNodeRect();
				}
				// 设置地图信息
				if (!ParseMapInfo(i, sTemp))
				{
					break;
				}
			}
			else // 座位信息部分
			{
				// 设置座位信息
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

	// 判断是否加载失败过
	if (i != m_nHeight + m_nSeatCount || nSeatCount != m_nSeatCount)
	{
		// 判断是否创建过节点
		if (i >= 0)
		{
			DestroyNodeRect(); // 删除座位
		}
		return false;
	}


	// 设置已经初始化
	m_bInitialized = true;
	return true;
}

bool CAStar::SaveDataFile(const char* sFileName /* = DATA_FILE_NAME */)
{
	if (!GetInitialzed())
	{
		return false;
	}
	// 打开文件
	ofstream fs;
	fs.open(sFileName);
	if (!fs.is_open())
	{
		return false;
	}
	// 先清空所有数据
	fs.clear();

	// 计算出座位信息
	int nCur = -1;
	int nNext = -1;
	int nHead = -1;
	map< int, int > mapNodeRect;
	map< int, int >::iterator it;
	//m_nSeatCount = 0;
	for(int i = 0; i < GetTotalCount(); i++)
	{
		// 如果有座位信息，则统计起来
		if (m_NodeRect[i].GetLinkNode())
		{
			// 判断该座位格子是否已经加入集合中了
			it = mapNodeRect.find(i);
			if (it != mapNodeRect.end())
			{
				continue; // 不处理，继续下一个
			}
			// 说明已经找到一个整体座位的头索引
			//m_nSeatCount++;
			nCur = i;
			// 
			while (true)
			{
				// 加入集合
				mapNodeRect.insert(make_pair(nCur, 0));
				// 检查头索引
				if (-1 == nHead)
				{
					nHead = nCur;
				}
				else
				{
					// 一直找到头索引为止
					if (nCur == nHead)
					{
						break; // 后面的就舍弃掉了
					}
				}
				// 检查关联格子
				if (NULL == m_NodeRect[nCur].GetLinkNode())
				{
					break;
				}
				nNext = m_NodeRect[nCur].GetLinkNode()->m_nID;
				// 检查索引有效性
				if (!CheckIndex(nNext))
				{
					break;
				}
				nCur = nNext;
			}
			nHead = -1;
		}
	}

	// 第一行: 注释
	//fs.write(sFirstLine, (streamsize)strlen(sFirstLine));
	fs << g_sFirstLine << LINE_ENDER;

	// 第二行：地图列数、行数、起点、终点、是否四方向
	fs << m_nWidth << DATA_DELIMITER
		<< m_nHeight << DATA_DELIMITER
		<< m_nStart << DATA_DELIMITER
		<< m_nEnd << DATA_DELIMITER
		<< m_nFourDirection << DATA_DELIMITER
		<< m_nSeatCount << DATA_DELIMITER
		<< LINE_ENDER;

	// 第三行-第N行：地图格子信息，数量为N-2
	for(int i = 0; i < m_nHeight; i++)
	{
		for(int j = 0; j < m_nWidth; j++)
		{
			fs << (CAStar::IsNodeBlock(m_NodeRect[ConvertCoordinateToIndex(j, i)].GetStatus()) ? NodeStatus_Block : NodeStatus_None) << DATA_DELIMITER;
		}
		fs << LINE_ENDER;
	}

	// 第N+1行-文件结尾：座位信息，数量为m_nSeatCount
	nCur = -1;
	nNext = -1;
	nHead = -1;
	map< int, int >::iterator itModiy;
	for (it = mapNodeRect.begin(); it != mapNodeRect.end(); ++it)
	{
		// 检查该项是否写入过文件
		if (1 == it->second)
		{
			continue; // 继续下一个
		}
		// 得到第一个，写入文件
		nCur = it->first;
		// 
		while (true)
		{
			// 写入文件
			fs << nCur << DATA_DELIMITER;
			mapNodeRect[nCur] = 1;
			// 设置头索引
			if (-1 == nHead)
			{
				nHead = nCur;
			}
			else
			{
				// 一直找到头索引为止
				if (nCur == nHead)
				{
					break; // 后面的就舍弃掉了
				}
			}
			// 检查关联的座位指针
			if (NULL == m_NodeRect[nCur].GetLinkNode())
			{
				break;
			}
			// 检查索引有效性
			nNext = m_NodeRect[nCur].GetLinkNode()->m_nID;
			if (!CheckIndex(nNext))
			{
				break;
			}
			nCur = nNext;
		}
		// 说明一个整体座位，写完了
		fs << LINE_ENDER;
		nCur = -1;
		nNext = -1;
		nHead = -1;
	}

	// 关闭文件
	fs.close();
	return true;
}

void CAStar::DestroyData(void)
{
	if (!GetInitialzed())
	{
		return;
	}
	// 清除bEnable矩形内存
	DestroyNodeRect();

	// 
	m_nWidth = 0;
	m_nHeight = 0;
	m_nStart = 0;
	m_nEnd = 0;
	listClose.Clear();
	listOpen.Clear();

	// 标记未初始化
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
	// 检查层次
	if (nLayer < 1)
	{
		return -1;
	}

	// 检查该索引是否处于边边角角
	int x = -1;
	int y = -1;
	if (! ConvertIndexToCoordinate(nCurIndex, x, y))
	{
		return -1;
	}

	// 上
	if (nDirectionValue & DirectionValue_Top)
	{
		// 检查不能处于第一行
		if (0 == y)
		{
			return -1;
		}
		nNewIndex -= m_nWidth * nLayer;
	}
	// 下
	if (nDirectionValue & DirectionValue_Bottom)
	{
		// 检查不能处于最后一行
		if (m_nHeight - 1 == y)
		{
			return -1;
		}
		nNewIndex += m_nWidth * nLayer;
	}
	// 左
	if (nDirectionValue & DirectionValue_Left)
	{
		// 检查不能处于第一列
		if (0 == x)
		{
			return -1;
		}
		nNewIndex -= 1 * nLayer;
	}
	// 右
	if (nDirectionValue & DirectionValue_Right)
	{
		// 检查不能处于最后一列
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
	// 默认结果
	nResult = ExpandResult_None;
	// 检查当前索引
	if(! CheckIndex(nCurIndex))
	{
		return false;
	}
	// 根据方向得出新索引
	int nNewIndex = GetNewIndexByDirectionValue(nCurIndex, nDirectionValue);
	// 检查新索引
	if (! CheckIndex(nNewIndex))
	{
		return false;
	}
	if(CAStar::IsNodeBlock(m_NodeRect[nNewIndex].GetStatus()))
	{
		return false;
	}
	// 如果已经找到最终bEnable，则无需继续了，直接返回
	if(nNewIndex == m_nEnd)
	{
		// 设置bEnable矩形列表
		m_NodeRect[nNewIndex].m_pNRPre = &m_NodeRect[nCurIndex];
		nResult = ExpandResult_Final;
		return true;
	}

	// 查找Close列表
	if(-1 == listClose.Search(nNewIndex))
	{
		// 不在Close列表中的时候
		// 查找Open列表
		if(-1 == listOpen.Search(nNewIndex))
		{
			// 设置bEnable矩形列表
			m_NodeRect[nNewIndex].m_pNRPre = &m_NodeRect[nCurIndex];
			m_NodeRect[nNewIndex].m_nValueG = GetValueG(nNewIndex, nDirectionValue);
			m_NodeRect[nNewIndex].m_nValueH = GetValueH(nNewIndex);
			// 不在，则加入到open列表
			listOpen.Insert(listOpen.Length(), nNewIndex);
		}
		else
		{
			// 在open表中，判断G值是否需要更新
			if (m_NodeRect[nNewIndex].m_nValueG > GetValueG(nNewIndex, nDirectionValue))
			{
				// 重新设置bEnable矩形列表
				m_NodeRect[nNewIndex].m_pNRPre = &m_NodeRect[nCurIndex];
				m_NodeRect[nNewIndex].m_nValueG = GetValueG(nNewIndex, nDirectionValue);
			}
			else
			{
				// 否则什么也不做
			}
		}
	}

	return true;
}

bool CAStar::ExpandNodeRectAllDirection(int nCurIndex, int& nResult)
{
	// 循环各个方向扩展
	for (int i = DirectionIndex_Begin; i < DirectionIndex_End; i++)
	{
		// 如果是四方向，则需要判断是否四个正方向
		if (GetFourDirection())
		{
			if (0 == (i % 2))
			{
				continue; // 斜的方向不管了
			}
		}
		// 扩展该方向
		if (ExpandNodeRectByDirectionValue(nCurIndex, g_arrDirectionValue[i], nResult))
		{
			// 扩展成功，并且结果为最终点，则无需继续扩展
			if (ExpandResult_Final == nResult)
			{
				return true; // 退出
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
	// 是否斜的方向
	bool bOblique = false; // 默认不是斜的方向
	for (int j = DirectionIndex_Begin; j < DirectionIndex_End; j++)
	{
		// 如果是四方向，则需要判断是否四个正方向
		if (g_arrDirectionValue[j] == nDirectionValue)
		{
			if (0 == (j % 2))
			{
				bOblique = true; // 数组的前四个数为斜的方向
				break;
			}
		}
	}
	// 如果是斜的方向
	if (bOblique)
	{
		nValue += (int)(g_nGridValue * 0.414f); // 对角线
	}

	return nValue;
}

bool CAStar::FindPath(void)
{
	if (!GetInitialzed())
	{
		return false;
	}
	// 把上次结果给清除掉
	listClose.Clear();
	listOpen.Clear();
	// 座位信息
	InitNodeRect();

	// 检查起点和终点
	if (m_nStart == m_nEnd
		|| !CheckIndex(m_nStart)
		|| !CheckIndex(m_nEnd)
		)
	{
		return false;
	}
	// 寻路开始
	if (!FindNext())
	{
		return false;
	}

	// 取得结果
	GetResult();

	return true;
}


bool CAStar::FindNext(void)
{
	int nTemp = 0;
	int nResult = 0;
	// 如果close列表为空
	if(listClose.IsEmpty())
	{
		// 把起始点加入到Close列表中
		listClose.Insert(listClose.Length(), m_nStart);

		// 需要创建开始点的各个方向的子bEnable
		if (ExpandNodeRectAllDirection(m_nStart, nResult))
		{
			if (ExpandResult_Final == nResult)
			{
				return true; // 退出
			}
		}
	}

	// 找出Open列表中最小的项
	int nFindIndex = -1;		// 找到的索引
	int nMin = 0;				// 最小值
	int nNodeRectIndex = -1;	// bEnable矩形的索引
	int nIndexTemp = -1;
	int nValueF = -1;
	for(int i = 0; i < listOpen.Length(); i++)
	{
		// 得到bEnable矩形的索引
		listOpen.Find(i, nIndexTemp);
		// 计算F值
		nValueF = m_NodeRect[nIndexTemp].m_nValueH +  m_NodeRect[nIndexTemp].m_nValueG;
		// 默认第一个为最小值，比较后设置最小值
		if(0 == i || nValueF <= nMin)
		{
			nMin = nValueF;
			nFindIndex = i;
			nNodeRectIndex = nIndexTemp;
		}
	}
	if(-1 == nFindIndex)
	{
		return false; // 走投无路，寻路失败了
	}
	// 把该最小值从Open列表中删除
	listOpen.Delete(nFindIndex, nTemp);
	// 把该最小值插入到Close列表中
	listClose.Insert(listClose.Length(), nNodeRectIndex);

	// 下面从该索引开始，创建各个方向的子bEnable
	if (ExpandNodeRectAllDirection(nNodeRectIndex, nResult))
	{
		if (ExpandResult_Final == nResult)
		{
			return true; // 退出
		}
	}

	// 继续递归
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
	// 先清除原来的座位
	if (m_NodeRect[nIndex].GetLinkNode())
	{
		m_NodeRect[nIndex].ClearSeat();
		m_nSeatCount--; // 计数减少
	}

	// 检查一番
	bool bReturn = true;
	if (m_NodeRect[nIndex].CheckSeat(sInfo))
	{
		// 解析座位信息
		char sTemp[SEAT_TOTAL_LEN];
		strcpy_s(sTemp, SEAT_TOTAL_LEN, sInfo);
		bReturn = ParseSeatInfo(sTemp);
	}

	// 如果有新的座位了
	if (m_NodeRect[nIndex].GetLinkNode())
	{
		m_nSeatCount++; // 计数增加
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
	// 检查字符串
	if (strlen(sInfo) >= SEAT_TOTAL_LEN)
	{
		return false;
	}
	// 
	char* pTemp = NULL;
	char* next_token = NULL;
	// 解析字符串
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
		// 记录头索引
		if (-1 == nPre)
		{
			nHead = nCur;
		}
		else
		{
			if (nPre == nCur)
			{
				break; // 同一个索引连续出现，结束处理
			}
			// 将座位联系起来
			m_NodeRect[nPre].SetLinkNode( &m_NodeRect[nCur] );
			nCount++;
			// 回到头索引的时候，就算完成了，后面的数据可以舍弃了
			if (nCur == nHead)
			{
				break;
			}
		}
		//
		nPre = nCur;
		pTemp = strtok_s(NULL, DATA_DELIMITER, &next_token);
	}
	// 检查数据数量
	if (nCount > SEAT_RECT_MAX)
	{
		return false;
	}
	// 检查数据的完整性
	if (nCur != nHead)
	{
		return false; // 说明没有形成一个整体座位
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
	// 解析字符串
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
	// 判断数据数量是否正确
	if (PARSE_MEMBER_COUNT != j)
	{
		return false;
	}

	// 检查重要参数
	if (!CheckImportantMember())
	{
		return false;
	}

	return true;
}

bool CAStar::CheckImportantMember(void)
{
	// 判断列数
	if (!CAStar::IsValidColumn(m_nWidth))
	{
		return false;
	}
	// 判断行数
	if (!CAStar::IsValidLine(m_nHeight))
	{
		return false;
	}
	// 判断起点和终点
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
	// 解析字符串
	pTemp = strtok_s(sInfo, DATA_DELIMITER, &next_token);
	if (NULL == pTemp)
	{
		return false;
	}
	int nColumn = 0; // 每行重新设定列坐标
	while (pTemp)
	{
		if (nColumn < m_nWidth)
		{
			m_NodeRect[ConvertCoordinateToIndex(nColumn, nLine)].SetStatus( atoi(pTemp) );
		}
		nColumn++;
		pTemp = strtok_s(NULL, DATA_DELIMITER, &next_token);
	}
	// 检查每一行的列数
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
	// 确定X坐标
	if (nCurX < nPreX)
	{
		nDirectionValue |= DirectionValue_Left;
	}
	else if (nCurX > nPreX)
	{
		nDirectionValue |= DirectionValue_Right;
	}
	// 确定Y坐标
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
	// 先把角度补成正的角度
	while (nDegree < 0)
	{
		nDegree += 360;
	}
	// 把多余的角度去除
	nDegree = nDegree % 360;
	// 区分开来
	if (nDegree > 180)
	{
		fAngles = (float)(nDegree - 360);
	}
	else
	{
		fAngles = (float)nDegree;
	}
	// 计算
	fAngles /= 180.0f;
	fAngles *= ASTAR_PI;

	return fAngles;
}

int CAStar::FindNearestUnblockNode(int nIndex, int nDirection /* = DirectionIndex_End */, int nLayer /* = 1 */)
{
	int nValidIndex = -1;

	// 如果方向值有效，则只查找一个
	if (CAStar::IsValidDirectionIndex(nDirection))
	{
		int nTempIndex = GetNewIndexByDirectionValue(nIndex, g_arrDirectionValue[nDirection], nLayer);
		// 检查索引有效性
		if (CheckIndex(nTempIndex))
		{
			// 检查该索引是否障碍物
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
			// 循环各个方向扩展
			for (int i = DirectionIndex_Begin; i < DirectionIndex_End; i++)
			{
				// 如果是四方向，则需要判断是否四个正方向
				if (GetFourDirection())
				{
					if (0 == (i % 2))
					{
						continue; // 斜的方向不管了
					}
				}
				// 得出这个方向的索引值
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
