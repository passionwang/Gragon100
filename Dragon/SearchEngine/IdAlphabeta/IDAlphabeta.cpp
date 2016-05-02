#include "stdafx.h"
#include "IDAlphabeta.h"
CIDAlphabeta::CIDAlphabeta()
{

}

CIDAlphabeta::~CIDAlphabeta()
{

}

void CIDAlphabeta::SearchAGoodMove(int position[10][10])
{
	CHESSMOVE backupmove;
	CPublicToMakeMove ptmm;
	memcpy(CurPosition, position, sizeof(CurPosition));
	m_nTimeCount = GetTickCount();
	for (m_nMaxDepth = 1; m_nMaxDepth <= m_nSearchDepth; m_nMaxDepth++)
	{
		if (alphabeta(m_nMaxDepth, -2000000, 2000000) != 6666666)
			backupmove = m_cmBestMove;
	}

	MakeMove(&backupmove,ptmm,-1);
	memcpy(position, CurPosition, sizeof(CurPosition));
}
int CIDAlphabeta::alphabeta(int depth, int alpha, int beta)
{
	int score;
	int Count,i;
	int mtype = (m_nMaxDepth%2 == depth%2) ? (-1) : (1);
	CPublicToMakeMove ptmm;
	i = IsGameOver(CurPosition, depth,mtype);	
	if (i != 0)
		return i;

	if (depth <= 0)	//叶子节点取估值
		return m_pEval->Eveluate(CurPosition,mtype,(m_nMaxDepth-depth)%2);
	
	Count = m_pMG->CreatePossibleMove(CurPosition, depth, mtype);
	
	if(1 == Count && depth == m_nMaxDepth)
	{
		m_cmBestMove = m_pMG->m_nMoveList[depth][0];
		return 0;
	}

	if (depth == m_nMaxDepth && m_nMaxDepth > 1)
	{
		for (i=1; i < Count;i++) 
		{
			if (m_pMG->m_nMoveList[depth][i] == m_cmBestMove)	
			{
				m_pMG->m_nMoveList[depth][i] = m_pMG->m_nMoveList[depth][0];
				m_pMG->m_nMoveList[depth][0] = m_cmBestMove;
			}				
		}	
	}

	for (i=0;i<Count;i++) 
	{
		if (depth == m_nMaxDepth)
		{
			if (GetTickCount() - m_nTimeCount >= 5000)
				return 6666666;
		}
		MakeMove(&m_pMG->m_nMoveList[depth][i],ptmm);
		score = -alphabeta(depth - 1, -beta, -alpha);
		UnMakeMove(&m_pMG->m_nMoveList[depth][i],ptmm); 

		if (score > alpha)
		{
			alpha = score;
			if(depth == m_nMaxDepth)
				m_cmBestMove = m_pMG->m_nMoveList[depth][i];
		}
        if (alpha >= beta) 
              break;

				
	}
	return alpha;
}

