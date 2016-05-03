#pragma once
#ifndef __INCLUDE_FALPHABETAENGINE_H__
#define __INCLUDE_FALPHABETAENGINE_H__
#include "SearchEngine.h"

class CFAlphaBetaEngine : public CSearchEngine  
{
public:
	CFAlphaBetaEngine();
	virtual ~CFAlphaBetaEngine();

	virtual void SearchAGoodMove(int position[10][10],int m_UpDown);

protected:
	int FAlphaBeta(int depth, int alpha, int beta,int m_UpDown);

};

#endif // __INCLUDE_FALPHABETAENGINE_H__
