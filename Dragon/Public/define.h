#pragma once


#ifndef __INCLUDE_DEFINE_H__
#define __INCLUDE_DEFINE_H__

#include<iostream>
using namespace std;

#define iMenu   32778

#define WHITE   (-1)
#define BLACK   (1)

#define WRITE_GO   -1
#define BLACK_GO   1

#define Left_Down   (1)
#define Right_Down  (2)
#define Left_Up     (3)
#define Right_Up    (4)

#define WANG     (2)
#define Qi       (1)
struct CHESSPOS
{
	unsigned char x;
	unsigned char y;
};
struct Node_YouSha
{
public:
	BYTE shendu;
	BYTE tongdao;
	BYTE kongge1;
	BYTE kongge2;
	Node_YouSha *pNext;
};
struct CHESSMOVE
{
public:
	CHESSMOVE() //初始化
	{
		m_Count = 0;
		for(BYTE i=0;i<20;i++)
		{
			step[i] = 0;
			step_king_kongge1[i] = 0;
			step_king_kongge2[i] = 0;
		}
	}
public:
	int m_Side;     //黑棋还是白棋
	int m_Type;    //王棋还是普通棋
	CHESSPOS From;    //起点坐标
	int m_Count;    //杀棋数量 (走棋步数)
	BYTE step[20];     //每一步的方向（最多20步）
	BYTE step_king_kongge1[20];//每一步的两个棋中空格数
	BYTE step_king_kongge2[20];//每一步的落点空格数
	BYTE Score;        //记录分数
	bool operator==(const CHESSMOVE& move)
	{   //判断起始坐标
		if(From.x==move.From.x && From.y==move.From.y && m_Count==move.m_Count)
		{   //循环每次走法
			for(BYTE i=0;i < m_Count+1;i++)
			{   //判断是否完全相同
				if(step[i]!=move.step[i] || step_king_kongge1[i]!=move.step_king_kongge1[i] || step_king_kongge2[i]!=move.step_king_kongge2[i])
				{
					return false;
				}
			}
			return true;
		}
		else
		{
			return false;
		}
	}
};

class CUndoNode
{
public:
	CUndoNode()
	{

	}
	CUndoNode(int Position[10][10],int bGo)
	{
		m_bGo = bGo;
		for(int i=0;i<10;i++)
		{
			for(int j=0;j<10;j++)
			{
				m_NowPosition[i][j] = Position[i][j];
			}
		}
	}
	~CUndoNode()
	{

	}
public:
	int m_NowPosition[10][10];
	int m_bGo;
};

#endif //__INCLUDE_DEFINE_H__