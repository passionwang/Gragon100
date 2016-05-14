
#include "stdafx.h"
#include "FAlphaBetaEngine.h"

CFAlphaBetaEngine::CFAlphaBetaEngine()
{
}

CFAlphaBetaEngine::~CFAlphaBetaEngine()
{
}

void CFAlphaBetaEngine::SearchAGoodMove(int position[10][10])
{
	CPublicToMakeMove ptmm;
	memcpy(CurPosition, position, sizeof(CurPosition));
	m_nMaxDepth = m_nSearchDepth;
	FAlphaBeta(m_nMaxDepth, -2000000, 2000000);
	MakeMove(&m_cmBestMove,ptmm,WHITE);
	memcpy(position, CurPosition, sizeof(CurPosition));
}


int CFAlphaBetaEngine::FAlphaBeta(int depth, int alpha, int beta)
{
	int current = -2000000 ;
	int score;
	int Count,i;
	int mtype = (m_nMaxDepth%2 == depth%2) ? (-1) : (1);
	CPublicToMakeMove ptmm;
	i = IsGameOver(CurPosition, depth,mtype);	
	if (i != 0)
		return i;

	if (depth <= 0)	//Ҷ�ӽڵ�ȡ��ֵ
	{
		return m_pEval->Eveluate(CurPosition,mtype,(m_nMaxDepth-depth)%2);
	}
	Count = m_pMG->CreatePossibleMove(CurPosition, depth, mtype);

	if(1 == Count && depth == m_nMaxDepth)
	{
		m_cmBestMove = m_pMG->m_nMoveList[depth][0];
		return 0;
	}

	for (i=0;i<Count;i++) 
	{
		MakeMove(&m_pMG->m_nMoveList[depth][i],ptmm);
		score = -FAlphaBeta(depth - 1, -beta, -alpha);
		UnMakeMove(&m_pMG->m_nMoveList[depth][i],ptmm); 
				
		if (score > current)
		{
			current = score;
			if(depth == m_nMaxDepth)
				m_cmBestMove = m_pMG->m_nMoveList[depth][i];
			if (score > alpha)
				alpha = score;
			if (score >= beta) //beta��֦
				break;
		}
	}
	return current;
}

