#pragma once
#ifndef __INCLUDE_IDALPHABETA_H__
#define __INCLUDE_IDALPHABETA_H__
#include "SearchEngine.h"

class CIDAlphabeta : public CSearchEngine  
{
public:
	CIDAlphabeta();
	virtual ~CIDAlphabeta();
	virtual void SearchAGoodMove(int position[10][10]);

protected:
	int alphabeta(int depth, int alpha, int beta);
	int m_nTimeCount;
};

#endif // __INCLUDE_IDALPHABETA_H__
