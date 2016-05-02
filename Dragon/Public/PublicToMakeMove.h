#pragma once


#ifndef __INCLUDE_PUBLICTOMAKEMOVE_H__
#define __INCLUDE_PUBLICTOMAKEMOVE_H__

#include"define.h"
#include"MoveGenerator.h"

class CPublicToMakeMove
{
public:
	CPublicToMakeMove();
	~CPublicToMakeMove();
public:
	void MakeMoveQi(CHESSMOVE* move,int CurPosition[10][10],int type = 0);
	void MakeMoveWANG(CHESSMOVE* move,int CurPosition[10][10],int type = 0);
	void UnMakeMoveQi(CHESSMOVE* move,int CurPosition[10][10]);
	void UnMakeMoveWANG(CHESSMOVE* move,int CurPosition[10][10]);
	void BecomeKing(int CurPosition[10][10]);
public:
	CMoveGenerator m_pMG1;
	int m_ntype[20];
	BOOL m_bIsBecomeKing;
};


#endif //__INCLUDE_PUBLICTOMAKEMOVE_H__