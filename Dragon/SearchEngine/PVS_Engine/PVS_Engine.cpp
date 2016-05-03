#include "stdafx.h"
#include "PVS_Engine.h"

CPVS_Engine::CPVS_Engine()
{

}

CPVS_Engine::~CPVS_Engine()
{

}
void CPVS_Engine::SearchAGoodMove(int position[10][10],int m_UpDown)
{
	CPublicToMakeMove ptmm;
	memcpy(CurPosition, position, sizeof(CurPosition));
	m_nMaxDepth = m_nSearchDepth;

//	for (m_nMaxDepth = 1; m_nMaxDepth <= m_nSearchDepth; m_nMaxDepth++)
	{
	   PrincipalVariation(m_nMaxDepth, -2000000, 2000000);
	}
	
	MakeMove(&m_cmBestMove,ptmm,-1);
	memcpy(position, CurPosition, sizeof(CurPosition));
}

int CPVS_Engine::PrincipalVariation(int depth, int alpha, int beta)
{
	int score;
	int Count,i;
	int best;
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

	MakeMove(&m_pMG->m_nMoveList[depth][0],ptmm);
	best = -PrincipalVariation( depth-1, -beta, -alpha);
	UnMakeMove(&m_pMG->m_nMoveList[depth][0],ptmm); 
	if(depth == m_nMaxDepth)
		m_cmBestMove = m_pMG->m_nMoveList[depth][0];

	for (i=1;i<Count;i++) 
	{
		
		if(best < beta) 
		{
			if (best > alpha) 
				alpha = best;
			MakeMove(&m_pMG->m_nMoveList[depth][i],ptmm);
			score = -PrincipalVariation(depth-1, -alpha-1, -alpha);
			if (score > alpha && score < beta) 
			{
				best = -PrincipalVariation(depth-1, -beta, -score);
				if(depth == m_nMaxDepth)
				{
					UnMakeMove(&m_pMG->m_nMoveList[depth][i],ptmm); 
					m_cmBestMove = m_pMG->m_nMoveList[depth][i];
					continue;
				}
			}
			else if (score > best)
			{
				best = score;
				if(depth == m_nMaxDepth)
				{
					UnMakeMove(&m_pMG->m_nMoveList[depth][i],ptmm); 
					m_cmBestMove = m_pMG->m_nMoveList[depth][i];
					continue;
				}
			}
			UnMakeMove(&m_pMG->m_nMoveList[depth][i],ptmm); 
		}
	}

	return best;
}