#pragma once 
#ifndef __INCLUDE_ALPHABETAENGINE_H__
#define __INCLUDE_ALPHABETAENGINE_H__

#include "SearchEngine.h"
class CAlphaBetaEngine : public CSearchEngine  
{
public:
	CAlphaBetaEngine();
	virtual ~CAlphaBetaEngine();
	virtual void SearchAGoodMove(int position[10][10]);
protected:
	int alphabeta(int depth, int alpha, int beta);

};

#endif //__INCLUDE_ALPHABETAENGINE_H__