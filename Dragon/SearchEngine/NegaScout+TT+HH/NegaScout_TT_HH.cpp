
#include "stdafx.h"
#include "NegaScout_TT_HH.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNegaScout_TT_HH::CNegaScout_TT_HH()
{
	
}

CNegaScout_TT_HH::~CNegaScout_TT_HH()
{
	
}

void CNegaScout_TT_HH::SearchAGoodMove(int position[10][10])
{
	CPublicToMakeMove ptmm;
	memcpy(CurPosition, position, sizeof(CurPosition));
	m_nMaxDepth = m_nSearchDepth;
	CalculateInitHashKey(CurPosition);
	ResetHistoryTable();
//	m_nMaxDepth = 1;
//	NegaScout(m_nMaxDepth, -20000, 20000);
//	m_nMaxDepth = m_nSearchDepth;
//	for (m_nMaxDepth = 1; m_nMaxDepth <= m_nSearchDepth; m_nMaxDepth++)
	NegaScout(m_nMaxDepth, -2000000, 2000000);
	MakeMove(&m_cmBestMove,ptmm,-1);
	memcpy(position,CurPosition,sizeof(CurPosition));
}
int G_nCountTTHH;
int CNegaScout_TT_HH::NegaScout(int depth, int alpha, int beta)
{
	bool bIsSure = false;
	int Count,i;
	int a,b,t;
	int side;
	int score;
	int mtype = (m_nMaxDepth%2 == depth%2) ? (-1) : (1);
	CPublicToMakeMove ptmm;
	i = IsGameOver(CurPosition, depth,mtype);	
	if (i != 0)
		return i;
	
	side = (m_nMaxDepth-depth)%2;
	
	score = LookUpHashTable(alpha, beta, depth, side); 
	if (score != 6666666) 
	{
		G_nCountTTHH++;
		return score;
	}
	
	if (depth <= 0)	//叶子节点取估值
	{
		score = m_pEval->Eveluate(CurPosition,mtype,(m_nMaxDepth-depth)%2);
		EnterHashTable(exact, score, depth, side );
		return score;
	}
	
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
	
	int bestmove=-1;
	
    a = alpha;
    b = beta;
    int eval_is_exact = 0;
    for ( i = 0; i < Count; i++ ) 
	{
		Hash_MakeMove(&m_pMG->m_nMoveList[depth][i], CurPosition);
		MakeMove(&m_pMG->m_nMoveList[depth][i],ptmm);
		
		t = -NegaScout(depth-1 , -b, -a );
		
		if (t > a && t < beta && i > 0) 
		{
			a = -NegaScout (depth-1, -beta, -t );     /* re-search */
			eval_is_exact = 1;
			if(depth == m_nMaxDepth)
			{
				bIsSure = true;
				Hash_UnMakeMove(&m_pMG->m_nMoveList[depth][i],CurPosition); 
				UnMakeMove(&m_pMG->m_nMoveList[depth][i],ptmm); 
				m_cmBestMove = m_pMG->m_nMoveList[depth][i];
			}
			bestmove = i;
		}
		if(bIsSure == false)
		{
			Hash_UnMakeMove(&m_pMG->m_nMoveList[depth][i],CurPosition); 
			UnMakeMove(&m_pMG->m_nMoveList[depth][i],ptmm); 
		}
		else
		{
			bIsSure = false;
		}
		if (a < t)
		{
			eval_is_exact = 1;
			a=t;
			if(depth == m_nMaxDepth)
				m_cmBestMove = m_pMG->m_nMoveList[depth][i];
		}
		if ( a >= beta ) 
		{
			EnterHashTable(lower_bound, a, depth,side);
			EnterHistoryScore(&m_pMG->m_nMoveList[depth][i], depth);
			return a;
		}
		b = a + 1;                      /* set new null window */
	}
	if (bestmove != -1)
	EnterHistoryScore(&m_pMG->m_nMoveList[depth][bestmove], depth);
	if (eval_is_exact) 
		EnterHashTable(exact, a, depth,side);
	else 
		EnterHashTable(upper_bound, a, depth,side);
	return a;
}