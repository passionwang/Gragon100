#pragma once

#ifndef __INCLUDE_ASPIRATIONSEARCH_H__
#define __INCLUDE_ASPIRATIONSEARCH_H__


#include "FAlphaBetaEngine.h"

class CAspirationSearch : public CFAlphaBetaEngine  
{
public:
	CAspirationSearch();
	virtual ~CAspirationSearch();

	virtual void SearchAGoodMove(int position[10][10],int m_UpDown);
};

#endif // __INCLUDE_ASPIRATIONSEARCH_H__
