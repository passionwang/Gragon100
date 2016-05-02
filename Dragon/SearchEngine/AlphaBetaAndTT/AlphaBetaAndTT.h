#pragma once
#ifndef __INCLUDE_ALPHABETAANDTT_H__
#define __INCLUDE_ALPHABETAANDTT_H__

#include "SearchEngine.h"
#include "TranspositionTable.h"

class CAlphaBetaAndTT : public CSearchEngine, public CTranspositionTable 
{
public:

	CAlphaBetaAndTT();

	virtual ~CAlphaBetaAndTT();

	virtual void SearchAGoodMove(int position[10][10]);

protected:
	int alphabeta(int depth, int alpha, int beta);
};

#endif // __INCLUDE_ALPHABETAANDTT_H__