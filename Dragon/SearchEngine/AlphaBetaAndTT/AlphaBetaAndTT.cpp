#include "stdafx.h"
#include "AlphaBetaAndTT.h"

CAlphaBetaAndTT::CAlphaBetaAndTT()
{
}

CAlphaBetaAndTT::~CAlphaBetaAndTT()
{
}


int G_nCountTT;
void CAlphaBetaAndTT::SearchAGoodMove(int position[10][10],int m_UpDown)
{
	CPublicToMakeMove ptmm;
	memcpy(CurPosition, position, sizeof(CurPosition));
	CalculateInitHashKey(CurPosition);
	m_nMaxDepth = m_nSearchDepth;
	alphabeta(m_nMaxDepth, -2000000, 2000000,m_UpDown);
	MakeMove(&m_cmBestMove,ptmm,WHITE * m_UpDown);
	memcpy(position, CurPosition, sizeof(CurPosition));
}

int CAlphaBetaAndTT::alphabeta(int depth, int alpha, int beta,int m_UpDown)
{
	int score;
	int Count,i;
	int side;
	int mtype = (m_nMaxDepth%2 == depth%2) ? (-1) : (1);
	CPublicToMakeMove ptmm;
	i = IsGameOver(CurPosition, depth,mtype * m_UpDown);	
	if (i != 0)
		return i;

	side = (m_nMaxDepth-depth)%2;

	score = LookUpHashTable(alpha, beta, depth, side); 
	if (score != 6666666) 
	{
		G_nCountTT++;
		return score;
	}
	if (depth <= 0)	//叶子节点取估值
	{
		if(1 == m_UpDown)
			score = m_pEval->Eveluate(CurPosition,mtype * m_UpDown,(m_nMaxDepth-depth)%2);
		else
			score = m_pEval->Eveluate(CurPosition,mtype * m_UpDown,(m_nMaxDepth-depth+1)%2);
		EnterHashTable(exact, score, depth, side );
		return score;
	}



	Count = m_pMG->CreatePossibleMove(CurPosition, depth,mtype);
	if(1 == Count && depth == m_nMaxDepth)
	{
		m_cmBestMove = m_pMG->m_nMoveList[depth][0];
		return 0;
	}

    int eval_is_exact = 0;

	for (i=0;i<Count;i++) 
	{
		Hash_MakeMove(&m_pMG->m_nMoveList[depth][i], CurPosition);
		MakeMove(&m_pMG->m_nMoveList[depth][i],ptmm);
		
		score = -alphabeta(depth - 1, -beta, -alpha,m_UpDown);

		Hash_UnMakeMove(&m_pMG->m_nMoveList[depth][i], CurPosition); 
		UnMakeMove(&m_pMG->m_nMoveList[depth][i],ptmm); 
        if (score >= beta) 
		{
			EnterHashTable(lower_bound, score, depth,side);
            return score;
        }

		if (score > alpha)
		{
			alpha = score;
			eval_is_exact = 1;
			if(depth == m_nMaxDepth)
				m_cmBestMove = m_pMG->m_nMoveList[depth][i];
		}

	}

    if (eval_is_exact) 
		EnterHashTable(exact, alpha, depth,side);
    else 
		EnterHashTable(upper_bound, alpha, depth,side);
	return alpha;
}



