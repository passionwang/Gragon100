#pragma once

#ifndef __INCLUDE_FACTORYSEARCH_H__
#define __INCLUDE_FACTORYSEARCH_H__

#include "SearchEngine.h"
#include "Alphabeta_HH.h"
#include "AlphaBetaAndTT.h"
#include "AlphaBetaEngine.h"
#include "AspirationSearch.h"
#include "FAlphaBetaEngine.h"
#include "IDAlphabeta.h"
#include "NegamaxEngine.h"
#include "NegaScout_TT_HH.h"
#include "PVS_Engine.h"
enum SearhType
{
	Alphabeta_HH,
	AlphaBetaAndTT,
	AlphaBetaEngine,
	AspirationSearch,
	FAlphaBetaEngine,
	IDAlphabeta,
	NegamaxEngine,
	NegaScout_TT_HH,
	PVS_Engine
};
class CFactoryToSearch
{
public:
	CFactoryToSearch();
	~CFactoryToSearch();
public:
	CSearchEngine* CreateSearchEngine(SearhType iType);
};


#endif //__INCLUDE_FACTORYSEARCH_H__