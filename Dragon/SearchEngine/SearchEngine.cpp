#include "stdafx.h"
#include "SearchEngine.h"


CSearchEngine::CSearchEngine()
{
}

CSearchEngine::~CSearchEngine()
{
	delete m_pMG;
	delete m_pEval;
}

void CSearchEngine::MakeMove(CHESSMOVE* move,CPublicToMakeMove& ptmm,int type)
{
	//CurPosition改变数组
	if(Qi == move->m_Type)
	{   //吃子
		ptmm.MakeMoveQi(move,CurPosition,type);
	}
	else
	{
		ptmm.MakeMoveWANG(move,CurPosition,type);
	}
}

void CSearchEngine::UnMakeMove(CHESSMOVE* move,CPublicToMakeMove& ptmm)
{
	//CurPosition改变数组
	if(Qi == move->m_Type)
	{
		ptmm.UnMakeMoveQi(move,CurPosition);
	}
	else
	{
		ptmm.UnMakeMoveWANG(move,CurPosition);
	}
}


int CSearchEngine::IsGameOver(int position[10][10], BYTE nDepth,int mtype)
{
	int i;
	i = (m_nMaxDepth - nDepth + 1) % 2;
	
	if (0 == m_pMG1.CreatePossibleMove(position,nDepth,-1) && -1 == mtype)
		if (i)
			return -1999990 - nDepth;
		else
			return +1999990 + nDepth;
	if (0 == m_pMG1.CreatePossibleMove(position,nDepth,1) && 1 == mtype)
		if (i)
			return +1999990 + nDepth;
		else
			return -1999990 - nDepth;
	return 0;
}
