#include "stdafx.h"
#include "AspirationSearch.h"

CAspirationSearch::CAspirationSearch()
{
}

CAspirationSearch::~CAspirationSearch()
{
}

void CAspirationSearch::SearchAGoodMove(int position[10][10],int m_UpDown)
{
	int x,y;
	CPublicToMakeMove ptmm;
	memcpy(CurPosition, position, sizeof(CurPosition));
	m_nMaxDepth = m_nSearchDepth-1;
	x= FAlphaBeta(m_nMaxDepth, -2000000, 2000000);
	m_nMaxDepth = m_nSearchDepth;
	y = FAlphaBeta(m_nMaxDepth, x-500, x+500);
	if (y < x-500)
		FAlphaBeta(m_nMaxDepth, -2000000, y);
	if (y > x+500)
		FAlphaBeta(m_nMaxDepth, y, 2000000);
	MakeMove(&m_cmBestMove,ptmm,-1);
	memcpy(position, CurPosition, sizeof(CurPosition));
//	return 0;
}