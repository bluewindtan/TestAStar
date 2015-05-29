/////////////////////////////////////////////////////////////////
/// @file AStarDefine.h
///
/// @purpose A星算法定义文件
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
#ifndef __A_STAR_DEFINE_H__
#define __A_STAR_DEFINE_H__

//////////////////////////////////////////////////////////////////////////
// 常用定义
//////////////////////////////////////////////////////////////////////////
#ifndef NULL
#define NULL 0
#endif

// 字符串长度
#ifndef STRING_LEN_S
#define STRING_LEN_S 64
#endif

#ifndef STRING_LEN_M
#define STRING_LEN_M 256
#endif

#ifndef STRING_LEN_L
#define STRING_LEN_L 1024
#endif

//////////////////////////////////////////////////////////////////////////
// 最大行数
const int ASTAR_LINE_MAX = 46;
// 最大列数
const int ASTAR_COLUMN_MAX = 72;

// 每一格的数值定义
const int g_nGridValue = 10;

// 默认数据文件名称
const char* const DATA_FILE_NAME = "config.txt";
// 数据文件中数据之间的分割符
//const char* const DATA_DELIMITER = ",";
#define DATA_DELIMITER ","
// 每行结尾符
const char* const LINE_ENDER = "\n";

// 座位的字符串总长度(nnnn,nnnn,nnnn,nnnn,....)
const int SEAT_TOTAL_LEN = 128;
// 座位中单个方格的字符串长度(nnnn,)
const int SEAT_RECT_LEN = 5;
// 座位的方格最大数量
const int SEAT_RECT_MAX = SEAT_TOTAL_LEN / SEAT_RECT_LEN;

// 定义Pi值
const float ASTAR_PI = 3.14159265f;

// 查找最靠近的节点时，查找的最大层次
const int FIND_NEAREST_LAYER = 4;

//////////////////////////////////////////////////////////////////////////
// 枚举定义
//////////////////////////////////////////////////////////////////////////
// 扩展结果
enum ExpandResult
{
	ExpandResult_Begin = 0,
	ExpandResult_None = ExpandResult_Begin, // 无
	ExpandResult_Final,						// 最终节点

	ExpandResult_End // 请在此项之前增加
};

// 节点状态
enum NodeStatus
{
	NodeStatus_Begin = 0,
	NodeStatus_None = NodeStatus_Begin, // 空
	NodeStatus_Block,					// 障碍物
	NodeStatus_Way,						// 路线
	NodeStatus_Start,					// 起点
	NodeStatus_Final,					// 终点
	NodeStatus_None_Seat,				// 空:带座位
	NodeStatus_Block_Seat,				// 障碍物:带座位

	NodeStatus_End // 请在此项之前增加
};

// 方向枚举
enum DirectionIndex
{
	DirectionIndex_Begin = 0,
	DirectionIndex_Right_Top = DirectionIndex_Begin,	// 右下
	DirectionIndex_Right,								// 右
	DirectionIndex_Right_Bottom,						// 右上
	DirectionIndex_Top,									// 上
	DirectionIndex_Left_Top,							// 左上
	DirectionIndex_Left,								// 左
	DirectionIndex_Left_Bottom,							// 左下
	DirectionIndex_Bottom,								// 下

	DirectionIndex_End // 请在此前添加项
};

// 方向值
const int DirectionValue_Top		= 0x01;	// 上
const int DirectionValue_Bottom		= 0x02;	// 下
const int DirectionValue_Left		= 0x04;	// 左
const int DirectionValue_Right		= 0x08;	// 右

const int g_arrDirectionValue[DirectionIndex_End] = {
	DirectionValue_Right + DirectionValue_Bottom,	// 右下
	DirectionValue_Right,							// 右
	DirectionValue_Right + DirectionValue_Top,		// 右上
	DirectionValue_Top,								// 上
	DirectionValue_Left + DirectionValue_Top,		// 左上
	DirectionValue_Left,							// 左
	DirectionValue_Left + DirectionValue_Bottom,	// 左下
	DirectionValue_Bottom,							// 下
};

#endif // __A_STAR_DEFINE_H__
