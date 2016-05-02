#pragma once 
#ifndef __INCLUDE__TRANSPOSITIONTABLE_H__
#define __INCLUDE_TRANSPOSITIONTABLE_H__
#include <windows.h>
#include "define.h"
enum ENTRY_TYPE{ exact , lower_bound, upper_bound}; 

typedef struct HASHITEM{
    INT64 checksum;	// or long long might be even better
	ENTRY_TYPE  entry_type;
    short depth;
    int eval;
}HashItem;

class CTranspositionTable  
{
public:
	CTranspositionTable();
	virtual ~CTranspositionTable();

	void CalculateInitHashKey(int CurPosition[10][10]);
	void Hash_MakeMove(CHESSMOVE* move,int CurPosition[10][10]);
	void Hash_UnMakeMove(CHESSMOVE* move,int CurPosition[10][10]);

	int LookUpHashTable(int alpha, int beta, int depth,int TableNo);
	void EnterHashTable(ENTRY_TYPE entry_type, int eval, short depth,int TableNo);

	int m_nHashKey32[4][10][10];
	INT64 m_ulHashKey64[4][10][10];
	HashItem *m_pTT[10];
	void InitializeHashKey();
	UINT m_HashKey32;
	INT64 m_HashKey64;
	int m_ntype[20];
};

#endif // __INCLUDE_TRANSPOSITIONTABLE_H__