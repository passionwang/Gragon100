#include "stdafx.h"
#include "Alphabeta_HH.h"

CAlphabeta_HH::CAlphabeta_HH()
{

}

CAlphabeta_HH::~CAlphabeta_HH()
{

}

void CAlphabeta_HH::SearchAGoodMove(int position[10][10],int m_UpDown)
{
	CPublicToMakeMove ptmm;
	memcpy(CurPosition, position, sizeof(CurPosition));
	m_nMaxDepth = m_nSearchDepth;
	ResetHistoryTable();
	alphabeta(m_nMaxDepth, -2000000, 2000000);
	MakeMove(&m_cmBestMove,ptmm,-1);
	memcpy(position, CurPosition, sizeof(CurPosition));
}
int G_nCountHH;
int CAlphabeta_HH::alphabeta(int depth, int alpha, int beta)
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
	for (i=0;i<Count;i++) 
	{
		m_pMG->m_nMoveList[depth][i].Score = 
			           GetHistoryScore(&m_pMG->m_nMoveList[depth][i]);
	}
	MergeSort(m_pMG->m_nMoveList[depth], Count, 0);
	int bestmove = -1;
	for (i=0;i<Count;i++) 
	{

		MakeMove(&m_pMG->m_nMoveList[depth][i],ptmm);
		score = -alphabeta(depth - 1, -beta, -alpha);
		UnMakeMove(&m_pMG->m_nMoveList[depth][i],ptmm); 

		if (score > alpha)
		{
			alpha = score;
			if(depth == m_nMaxDepth)
				m_cmBestMove = m_pMG->m_nMoveList[depth][i];
			bestmove = i;
		}
        if (alpha >= beta) 
		{
			bestmove = i;
			break;
		}
				
	}
	if (bestmove != -1)
	EnterHistoryScore(&m_pMG->m_nMoveList[depth][bestmove], depth);
	return alpha;
}

