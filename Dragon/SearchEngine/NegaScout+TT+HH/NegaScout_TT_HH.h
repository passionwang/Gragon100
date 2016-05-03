#pragma once 
#ifndef __INCLUDE_NEGASCOUT_TT_HH_H__

#define __INCLUDE_NEGASCOUT_TT_HH_H__

#include "SearchEngine.h"
#include "TranspositionTable.h"
#include "HistoryHeuristic.h"

class CNegaScout_TT_HH : 
	public CTranspositionTable, 
	public CHistoryHeuristic, 
	public CSearchEngine  
{
public:
	CNegaScout_TT_HH();
	virtual ~CNegaScout_TT_HH();

	virtual void SearchAGoodMove(int position[10][10],int m_UpDown);

protected:
	int NegaScout(int depth, int alpha, int beta,int m_UpDown);
};

#endif // __INCLUDE_NEGASCOUT_TT_HH_H__
