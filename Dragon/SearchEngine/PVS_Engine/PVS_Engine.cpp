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
	   PrincipalVariation(m_nMaxDepth, -2000000, 2000000,m_UpDown);
	}
	
	MakeMove(&m_cmBestMove,ptmm,WHITE * m_UpDown);
	memcpy(position, CurPosition, sizeof(CurPosition));
}

int CPVS_Engine::PrincipalVariation(int depth, int alpha, int beta,int m_UpDown)
{
	int score;
	int Count,i;
	int best;
	int mtype = (m_nMaxDepth%2 == depth%2) ? (-1) : (1);
	CPublicToMakeMove ptmm;
	i = IsGameOver(CurPosition, depth,mtype * m_UpDown);	
	if (i != 0)
		return i;

	if (depth <= 0)	//叶子节点取估值
	{
		if(1 == m_UpDown)
			return m_pEval->Eveluate(CurPosition,mtype * m_UpDown,(m_nMaxDepth-depth)%2);
		else
			return m_pEval->Eveluate(CurPosition,mtype * m_UpDown,(m_nMaxDepth-depth+1)%2);
	}
	
	Count = m_pMG->CreatePossibleMove(CurPosition, depth,  mtype * m_UpDown);
	
	if(1 == Count && depth == m_nMaxDepth)
	{
		m_cmBestMove = m_pMG->m_nMoveList[depth][0];
		return 0;
	}

	MakeMove(&m_pMG->m_nMoveList[depth][0],ptmm);
	best = -PrincipalVariation( depth-1, -beta, -alpha,m_UpDown);
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
			score = -PrincipalVariation(depth-1, -alpha-1, -alpha,m_UpDown);
			if (score > alpha && score < beta) 
			{
				best = -PrincipalVariation(depth-1, -beta, -score,m_UpDown);
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