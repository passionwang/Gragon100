#pragma once
#ifndef __INCLUDE_SEARCHENGINE_H__
#define __INCLUDE_SEARCHENGINE_H__

#include"MoveGenerator.h"
#include"Eveluation.h"
#include"PublicToMakeMove.h"

class CSearchEngine : public CPublicToMakeMove 
{
public:
	CSearchEngine();
	virtual ~CSearchEngine();
	virtual void SearchAGoodMove(int position[10][10]) = 0;
	virtual void SetSearchDepth(BYTE nDepth){m_nSearchDepth = nDepth;}
	virtual void SetEveluator(CEveluation *pEval){m_pEval = pEval;}
	virtual void SetMoveGenerator(CMoveGenerator *pMG){m_pMG = pMG;}
protected:
	virtual void MakeMove(CHESSMOVE* move,CPublicToMakeMove& ptmm,int type = 0);
	virtual void UnMakeMove(CHESSMOVE* move,CPublicToMakeMove& ptmm);
	virtual int IsGameOver(int position[10][10], BYTE nDepth,int mtype);
	int CurPosition[10][10];
	CHESSMOVE m_cmBestMove;
	CMoveGenerator *m_pMG;
	CEveluation *m_pEval;
	BYTE m_nSearchDepth;
	BYTE m_nMaxDepth;
};



#endif //__INCLUDE_SEARCHENGINE_H__