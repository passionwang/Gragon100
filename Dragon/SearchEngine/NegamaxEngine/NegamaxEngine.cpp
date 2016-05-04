
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
	NegaMax(m_nMaxDepth,m_UpDown);
	MakeMove(&m_cmBestMove,ptmm,WHITE * m_UpDown);
	memcpy(position, CurPosition, sizeof(CurPosition));
}
extern int count;
int CNegamaxEngine::NegaMax(int depth,int m_UpDown)
{
	int current = -2000000 ;
	int score;
	int Count,i;
	CPublicToMakeMove ptmm;
	int mtype = (m_nMaxDepth%2 == depth%2) ? (-1) : (1);

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

	for (i=0;i<Count;i++) 
	{
		MakeMove(&m_pMG->m_nMoveList[depth][i],ptmm);
		score = -NegaMax(depth - 1,m_UpDown);
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

