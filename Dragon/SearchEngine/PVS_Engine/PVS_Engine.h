#pragma once
#ifndef __INCLUDE_PVSENGINE_H__
#define __INCLUDE_PVSENGINE_H__

#include "SearchEngine.h"

class CPVS_Engine : public CSearchEngine
{
public:
	CPVS_Engine();
	virtual ~CPVS_Engine();
	virtual void SearchAGoodMove(int position[10][10],int m_UpDown);

protected:
	int PrincipalVariation(int depth, int alpha, int beta,int m_UpDown);


};

#endif // __INCLUDE_PVSENGINE_H__
