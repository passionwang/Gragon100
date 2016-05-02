#pragma once
#ifndef __INCLUDE_HISTORYHEURISTIC_H__
#define __INCLUDE_HISTORYHEURISTIC_H__
#include <windows.h>
#include "define.h"
class CHistoryHeuristic  
{
public:
	CHistoryHeuristic();
	virtual ~CHistoryHeuristic();
	
	void ResetHistoryTable();
	int GetHistoryScore(CHESSMOVE *move);
	void EnterHistoryScore(CHESSMOVE *move,int depth);
	void MergeSort(CHESSMOVE *source, int n, BOOL direction);
protected:
	void Merge(CHESSMOVE *source, CHESSMOVE *target, int l,int m, int r);
	void MergePass(CHESSMOVE *source, CHESSMOVE *target, const  int s, const  int n, const BOOL direction);
	void Merge_A(CHESSMOVE *source, CHESSMOVE *target, int l,int m, int r);
	int m_HistoryTable[100][100];
	CHESSMOVE m_TargetBuff[100]; 

};

#endif // __INCLUDE_HISTORYHEURISTIC_H__
