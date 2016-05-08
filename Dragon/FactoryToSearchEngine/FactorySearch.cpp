#include "stdafx.h"
#include "FactorySearch.h"

CFactoryToSearch::CFactoryToSearch()
{

}
CFactoryToSearch::~CFactoryToSearch()
{

}
CSearchEngine* CFactoryToSearch::CreateSearchEngine(SearhType iType)
{
	switch(iType)
	{
	case Alphabeta_HH:
		return new CAlphabeta_HH;
		break;
	case AlphaBetaAndTT:
		return new CAlphaBetaAndTT;
		break;
	case AlphaBetaEngine:
		return new CAlphaBetaEngine;
		break;
	case AspirationSearch:
		return new CAspirationSearch;
		break;
	case FAlphaBetaEngine:
		return new CFAlphaBetaEngine;
		break;
	case IDAlphabeta:
		return new CIDAlphabeta;
		break;
	case NegamaxEngine:
		return new CNegamaxEngine;
		break;
	case NegaScout_TT_HH:
		return new CNegaScout_TT_HH;
		break;
	case PVS_Engine:
		return new CPVS_Engine;
		break;
	}
	return NULL;
}