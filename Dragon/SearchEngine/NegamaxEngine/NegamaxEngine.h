#pragma once

#ifndef __INCLUDE_NEGAMAXENGINE_H__
#define __INCLUDE_NEGAMAXENGINE_H__

#include "SearchEngine.h"

class CNegamaxEngine : public CSearchEngine  
{
public:
	CNegamaxEngine();
	virtual ~CNegamaxEngine();

	virtual void SearchAGoodMove(int position[10][10],int m_UpDown);

protected:
	int NegaMax(int depth,int m_UpDown);

};

#endif // __INCLUDE_NEGAMAXENGINE_H__
