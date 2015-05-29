/////////////////////////////////////////////////////////////////
/// @file AStarDefine.h
///
/// @purpose A���㷨�����ļ�
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
// ���ö���
//////////////////////////////////////////////////////////////////////////
#ifndef NULL
#define NULL 0
#endif

// �ַ�������
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
// �������
const int ASTAR_LINE_MAX = 46;
// �������
const int ASTAR_COLUMN_MAX = 72;

// ÿһ�����ֵ����
const int g_nGridValue = 10;

// Ĭ�������ļ�����
const char* const DATA_FILE_NAME = "config.txt";
// �����ļ�������֮��ķָ��
//const char* const DATA_DELIMITER = ",";
#define DATA_DELIMITER ","
// ÿ�н�β��
const char* const LINE_ENDER = "\n";

// ��λ���ַ����ܳ���(nnnn,nnnn,nnnn,nnnn,....)
const int SEAT_TOTAL_LEN = 128;
// ��λ�е���������ַ�������(nnnn,)
const int SEAT_RECT_LEN = 5;
// ��λ�ķ����������
const int SEAT_RECT_MAX = SEAT_TOTAL_LEN / SEAT_RECT_LEN;

// ����Piֵ
const float ASTAR_PI = 3.14159265f;

// ��������Ľڵ�ʱ�����ҵ������
const int FIND_NEAREST_LAYER = 4;

//////////////////////////////////////////////////////////////////////////
// ö�ٶ���
//////////////////////////////////////////////////////////////////////////
// ��չ���
enum ExpandResult
{
	ExpandResult_Begin = 0,
	ExpandResult_None = ExpandResult_Begin, // ��
	ExpandResult_Final,						// ���սڵ�

	ExpandResult_End // ���ڴ���֮ǰ����
};

// �ڵ�״̬
enum NodeStatus
{
	NodeStatus_Begin = 0,
	NodeStatus_None = NodeStatus_Begin, // ��
	NodeStatus_Block,					// �ϰ���
	NodeStatus_Way,						// ·��
	NodeStatus_Start,					// ���
	NodeStatus_Final,					// �յ�
	NodeStatus_None_Seat,				// ��:����λ
	NodeStatus_Block_Seat,				// �ϰ���:����λ

	NodeStatus_End // ���ڴ���֮ǰ����
};

// ����ö��
enum DirectionIndex
{
	DirectionIndex_Begin = 0,
	DirectionIndex_Right_Top = DirectionIndex_Begin,	// ����
	DirectionIndex_Right,								// ��
	DirectionIndex_Right_Bottom,						// ����
	DirectionIndex_Top,									// ��
	DirectionIndex_Left_Top,							// ����
	DirectionIndex_Left,								// ��
	DirectionIndex_Left_Bottom,							// ����
	DirectionIndex_Bottom,								// ��

	DirectionIndex_End // ���ڴ�ǰ�����
};

// ����ֵ
const int DirectionValue_Top		= 0x01;	// ��
const int DirectionValue_Bottom		= 0x02;	// ��
const int DirectionValue_Left		= 0x04;	// ��
const int DirectionValue_Right		= 0x08;	// ��

const int g_arrDirectionValue[DirectionIndex_End] = {
	DirectionValue_Right + DirectionValue_Bottom,	// ����
	DirectionValue_Right,							// ��
	DirectionValue_Right + DirectionValue_Top,		// ����
	DirectionValue_Top,								// ��
	DirectionValue_Left + DirectionValue_Top,		// ����
	DirectionValue_Left,							// ��
	DirectionValue_Left + DirectionValue_Bottom,	// ����
	DirectionValue_Bottom,							// ��
};

#endif // __A_STAR_DEFINE_H__
