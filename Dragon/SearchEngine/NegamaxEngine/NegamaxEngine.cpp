
#include "stdafx.h"
#include "NegamaxEngine.h"

//////////////////////////////////////////////////////////////////////

CNegamaxEngine::CNegamaxEngine()
{

}

CNegamaxEngine::~CNegamaxEngine()
{

}
void CNegamaxEngine::SearchAGoodMove(int position[10][10],int m_UpDown)
{
	CPublicToMakeMove ptmm;
	m_nMaxDepth = m_nSearchDepth;

	memcpy(CurPosition, position, sizeof(CurPosition));
	
	NegaMax(m_nMaxDepth);
	
	MakeMove(&m_cmBestMove,ptmm,-1);
	memcpy(position, CurPosition, sizeof(CurPosition));
}
extern int count;

int CNegamaxEngine::NegaMax(int depth)
{
	int current = -2000000 ;
	int score;
	int Count,i;
	CPublicToMakeMove ptmm;
	int mtype = (m_nMaxDepth%2 == depth%2) ? (-1) : (1);

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

		MakeMove(&m_pMG->m_nMoveList[depth][i],ptmm);
		score = -NegaMax(depth - 1);
		UnMakeMove(&m_pMG->m_nMoveList[depth][i],ptmm); 
		
		if (score > current)
		{
			current = score;
			if(depth == m_nMaxDepth)
			{
				m_cmBestMove = m_pMG->m_nMoveList[depth][i];
			}
		}

	}

	return current;
}

