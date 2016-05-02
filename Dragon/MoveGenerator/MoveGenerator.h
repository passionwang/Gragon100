#pragma once
#ifndef __INCLUDE_CMOVEGENERATOR_H__
#define __INCLUDE_CMOVEGENERATOR_H__
#include <iostream>
#include"define.h"
#include "PublicToCountSpace.h"
using namespace std;


class CMoveGenerator : public CPublicToCountSpace
{
public:
///////////////////////////////////////////////////////////////////////////////////////
	CMoveGenerator();
	virtual ~CMoveGenerator();
	void ClearData();
	//检查下一个走法是否合法
	//bool IsValidMove(short position[10][10],CHESSMOVE move);
	//产生给定棋盘上的所有合法的走法
	int CreatePossibleMove(int position[10][10],int nPly,int nSide);  
	//插入一个走法
	void AddMove_ShaQi(int i,int j,int nSide,int nPly,int& nMethod);
	void AddMove_BuJv(int nCount,int nSide,int nType,int mFangXiang,int nPly,int& nMethod,int nKongge = 0);
public:
////////////////////////////////////////////////////////////////////////////////////////
	void CommonCode(int i,int j,int value,int shu,int fangxiang,int TURN,int Board[10][10],int nCount = -1);
	void CommonFind(int depth,int nCount,int lianI,int chu1[600],int chu2[600],int MaxPos[600],int kong1[600],int kong2[600]);
	void NodeCount(int nCount);
	//判断杀伤力
	bool PangDuan_ShaShangli();
	//链表
	void addLike(Node_YouSha **pHead,Node_YouSha **pEnd,int sd,int td,int kg1=0,int kg2=0);
	void ClearLike(Node_YouSha **ppHead);
//////////////////////////////////////////////////////////////////////////////////////////////
	//杀棋模拟
	int Move_z_x_x(int i,int j,int Board[10][10]);
	void MoveF_z_x_x(int i,int j,int n,int Board[10][10]);
	int Move_z_s_s(int i,int j,int Board[10][10]);
	void MoveF_z_s_s(int i,int j,int n,int Board[10][10]);
	int Move_y_x_x(int i,int j,int Board[10][10]);
	void MoveF_y_x_x(int i,int j,int n,int Board[10][10]);
	int Move_y_s_s(int i,int j,int Board[10][10]);
	void MoveF_y_s_s(int i,int j,int n,int Board[10][10]);
	int Move_z_x_x_Wang(int i,int j,int kongge1,int m,int TURN,int Board[10][10]) ;
	void MoveF_z_x_x_Wang(int i,int j,int kongge1,int m,int TURN,int ntype,int Board[10][10]) ;
	int Move_z_s_s_Wang(int i,int j,int kongge1,int m,int TURN,int Board[10][10]);
	void MoveF_z_s_s_Wang(int i,int j,int kongge1,int m,int TURN,int ntype,int Board[10][10]) ;
	int Move_y_x_x_Wang(int i,int j,int kongge1,int m,int TURN,int Board[10][10]) ;
	void MoveF_y_x_x_Wang(int i,int j,int kongge1,int m,int TURN,int ntype,int Board[10][10]) ;
	int Move_y_s_s_Wang(int i,int j,int kongge1,int m,int TURN,int Board[10][10]) ;
	void MoveF_y_s_s_Wang(int i,int j,int kongge1,int m,int TURN,int ntype,int Board[10][10]);
	
	
/////////////////////////////////////////////////////////////////////////////////////////////
	//存放走法队列
	CHESSMOVE m_nMoveList[10][100];
protected: 
};

#endif //__INCLUDE_CMOVEGENERATOR_H__