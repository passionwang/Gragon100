#include "stdafx.h"
#include "TranspositionTable.h"


INT64 rand64(void)
{

    return rand() ^ ((LONGLONG)rand() << 15) ^ ((LONGLONG)rand() << 30) ^

        ((LONGLONG)rand() << 45) ^ ((LONGLONG)rand() << 60);

}
int rand32(void)
{

    return rand() ^ ((LONG)rand() << 15) ^ ((LONG)rand() << 30);

}

CTranspositionTable::CTranspositionTable()
{
	InitializeHashKey();
}

CTranspositionTable::~CTranspositionTable()
{
	delete m_pTT[0];
	delete m_pTT[1];
}

void CTranspositionTable::InitializeHashKey()
{
	int i,j,k;
	
	srand( (unsigned)time( NULL ));
	
	for (i = 0; i < 4; i++)
		for (j = 0; j < 10; j++)
			for (k = 0; k < 10; k++)
			{
				m_nHashKey32[i][j][k] = rand32();
				m_ulHashKey64[i][j][k] = rand64();
			}

	m_pTT[0] = new HashItem[1024*1024];
	m_pTT[1] = new HashItem[1024*1024];
}
void CTranspositionTable::CalculateInitHashKey(int CurPosition[10][10])
{
	int j,k,nChessType;
	m_HashKey32 = 0;
	m_HashKey64 = 0;
	for (j = 0; j < 10; j++)
		for (k = 0; k < 10; k++)
		{
			nChessType = CurPosition[j][k];
			if (nChessType != 0)
			{
				m_HashKey32 = m_HashKey32 ^ m_nHashKey32[nChessType][j][k]; 
				m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[nChessType][j][k]; 
			}
		}
}

void CTranspositionTable::Hash_MakeMove(CHESSMOVE* move,int CurPosition[10][10])
{
	int iPos = move->From.y;
	int jPos = move->From.x; 
	if(0 == move->m_Count)//不能吃子
	{
		if(move->m_Type == 1||move->m_Type == -1)//普通棋子
		{   //起点减掉
			m_HashKey32 = m_HashKey32 ^ m_nHashKey32[move->m_Side * move->m_Type][iPos][jPos]; 
			m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[move->m_Side * move->m_Type][iPos][jPos]; 
			//终点添加
			switch (move->step[0])
			{
			case Left_Down:
				m_HashKey32 = m_HashKey32 ^ m_nHashKey32[move->m_Side * move->m_Type][iPos+1][jPos-1]; 
				m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[move->m_Side * move->m_Type][iPos+1][jPos-1]; 
				break;
			case Right_Down:
				m_HashKey32 = m_HashKey32 ^ m_nHashKey32[move->m_Side * move->m_Type][iPos+1][jPos+1]; 
				m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[move->m_Side * move->m_Type][iPos+1][jPos+1]; 
				break;
			case Left_Up:
				m_HashKey32 = m_HashKey32 ^ m_nHashKey32[move->m_Side * move->m_Type][iPos-1][jPos-1]; 
				m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[move->m_Side * move->m_Type][iPos-1][jPos-1]; 
				break;
			case Right_Up:
				m_HashKey32 = m_HashKey32 ^ m_nHashKey32[move->m_Side * move->m_Type][iPos-1][jPos+1]; 
				m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[move->m_Side * move->m_Type][iPos-1][jPos+1]; 
				break;
			}
		}
		else//王
		{   //起点减掉
			m_HashKey32 = m_HashKey32 ^ m_nHashKey32[move->m_Side * move->m_Type][iPos][jPos]; 
			m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[move->m_Side * move->m_Type][iPos][jPos]; 
			//终点添加
			switch (move->step[0])
			{
			case Left_Down:
				m_HashKey32 = m_HashKey32 ^ m_nHashKey32[move->m_Side * move->m_Type][iPos+1+move->step_king_kongge1[0]][jPos-1-move->step_king_kongge1[0]]; 
				m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[move->m_Side * move->m_Type][iPos+1+move->step_king_kongge1[0]][jPos-1-move->step_king_kongge1[0]]; 
				break;
			case Right_Down:
				m_HashKey32 = m_HashKey32 ^ m_nHashKey32[move->m_Side * move->m_Type][iPos+1+move->step_king_kongge1[0]][jPos+1+move->step_king_kongge1[0]]; 
				m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[move->m_Side * move->m_Type][iPos+1+move->step_king_kongge1[0]][jPos+1+move->step_king_kongge1[0]]; 
				break;
			case Left_Up:
				m_HashKey32 = m_HashKey32 ^ m_nHashKey32[move->m_Side * move->m_Type][iPos-1-move->step_king_kongge1[0]][jPos-1-move->step_king_kongge1[0]]; 
				m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[move->m_Side * move->m_Type][iPos-1-move->step_king_kongge1[0]][jPos-1-move->step_king_kongge1[0]]; 
				break;
			case Right_Up:
				m_HashKey32 = m_HashKey32 ^ m_nHashKey32[move->m_Side * move->m_Type][iPos-1-move->step_king_kongge1[0]][jPos+1+move->step_king_kongge1[0]]; 
				m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[move->m_Side * move->m_Type][iPos-1-move->step_king_kongge1[0]][jPos+1+move->step_king_kongge1[0]]; 
				break;
			}
		}
	}
	else                  //能够吃子
	{
		if(move->m_Type == 1||move->m_Type == -1)//普通棋子
		{   //起点减掉
			m_HashKey32 = m_HashKey32 ^ m_nHashKey32[move->m_Side * move->m_Type][iPos][jPos]; 
			m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[move->m_Side * move->m_Type][iPos][jPos]; 
			for(int i=0;i<move->m_Count;i++)
			{
				//过程减掉
				switch (move->step[i])
				{
				case Left_Down:
					m_ntype[i] =  CurPosition[iPos+1][jPos-1]; 
					m_HashKey32 = m_HashKey32 ^ m_nHashKey32[m_ntype[i]][iPos+1][jPos-1]; 
					m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[m_ntype[i]][iPos+1][jPos-1]; 
					iPos += 2;
					jPos -= 2;
					break;
				case Right_Down:
					m_ntype[i] =  CurPosition[iPos+1][jPos+1]; 
					m_HashKey32 = m_HashKey32 ^ m_nHashKey32[m_ntype[i]][iPos+1][jPos+1]; 
					m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[m_ntype[i]][iPos+1][jPos+1]; 
					iPos += 2;
					jPos += 2;
					break;
				case Left_Up:
					m_ntype[i] =  CurPosition[iPos-1][jPos-1]; 
					m_HashKey32 = m_HashKey32 ^ m_nHashKey32[m_ntype[i]][iPos-1][jPos-1]; 
					m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[m_ntype[i]][iPos-1][jPos-1]; 
					iPos -= 2;
					jPos -= 2;
					break;
				case Right_Up:
					m_ntype[i] =  CurPosition[iPos-1][jPos+1]; 
					m_HashKey32 = m_HashKey32 ^ m_nHashKey32[m_ntype[i]][iPos-1][jPos+1]; 
					m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[m_ntype[i]][iPos-1][jPos+1]; 
					iPos -= 2;
					jPos += 2;
					break;
				}
			}
			//终点添加
			m_HashKey32 = m_HashKey32 ^ m_nHashKey32[move->m_Side * move->m_Type][iPos][jPos]; 
			m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[move->m_Side * move->m_Type][iPos][jPos]; 
		}
		else//王
		{
			 //起点减掉
			m_HashKey32 = m_HashKey32 ^ m_nHashKey32[move->m_Side * move->m_Type][iPos][jPos]; 
			m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[move->m_Side * move->m_Type][iPos][jPos]; 
			for(int i=0;i<move->m_Count;i++)
			{
				//过程减掉
				switch (move->step[i])
				{
				case Left_Down:
					m_ntype[i] =  CurPosition[iPos+1+move->step_king_kongge1[i]][jPos-1-move->step_king_kongge1[i]]; 
					m_HashKey32 = m_HashKey32 ^ m_nHashKey32[m_ntype[i]][iPos+1+move->step_king_kongge1[i]][jPos-1-move->step_king_kongge1[i]]; 
					m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[m_ntype[i]][iPos+1+move->step_king_kongge1[i]][jPos-1-move->step_king_kongge1[i]]; 
					iPos += (1+move->step_king_kongge1[i]+move->step_king_kongge2[i]);
					jPos -= (1+move->step_king_kongge1[i]+move->step_king_kongge2[i]);
					break;
				case Right_Down:
					m_ntype[i] =  CurPosition[iPos+1+move->step_king_kongge1[i]][jPos+1+move->step_king_kongge1[i]]; 
					m_HashKey32 = m_HashKey32 ^ m_nHashKey32[m_ntype[i]][iPos+1+move->step_king_kongge1[i]][jPos+1+move->step_king_kongge1[i]]; 
					m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[m_ntype[i]][iPos+1+move->step_king_kongge1[i]][jPos+1+move->step_king_kongge1[i]]; 
					iPos += (1+move->step_king_kongge1[i]+move->step_king_kongge2[i]);
					jPos += (1+move->step_king_kongge1[i]+move->step_king_kongge2[i]);
					break;
				case Left_Up:
					m_ntype[i] =  CurPosition[iPos-1-move->step_king_kongge1[i]][jPos-1-move->step_king_kongge1[i]];  
					m_HashKey32 = m_HashKey32 ^ m_nHashKey32[m_ntype[i]][iPos-1-move->step_king_kongge1[i]][jPos-1-move->step_king_kongge1[i]]; 
					m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[m_ntype[i]][iPos-1-move->step_king_kongge1[i]][jPos-1-move->step_king_kongge1[i]]; 
					iPos -= (1+move->step_king_kongge1[i]+move->step_king_kongge2[i]);
					jPos -= (1+move->step_king_kongge1[i]+move->step_king_kongge2[i]);
					break;
				case Right_Up:
					m_ntype[i] =  CurPosition[iPos-1-move->step_king_kongge1[i]][jPos+1+move->step_king_kongge1[i]];  
					m_HashKey32 = m_HashKey32 ^ m_nHashKey32[m_ntype[i]][iPos-1-move->step_king_kongge1[i]][jPos+1+move->step_king_kongge1[i]]; 
					m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[m_ntype[i]][iPos-1-move->step_king_kongge1[i]][jPos+1+move->step_king_kongge1[i]]; 
					iPos -= (1+move->step_king_kongge1[i]+move->step_king_kongge2[i]);
					jPos += (1+move->step_king_kongge1[i]+move->step_king_kongge2[i]);
					break;
				}
			}
			//终点添加
			m_HashKey32 = m_HashKey32 ^ m_nHashKey32[move->m_Side * move->m_Type][iPos][jPos]; 
			m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[move->m_Side * move->m_Type][iPos][jPos]; 
		}
	}
}

void CTranspositionTable::Hash_UnMakeMove(CHESSMOVE* move,int CurPosition[10][10])
{
	int iPos = move->From.y;
	int jPos = move->From.x; 
	if(0 == move->m_Count)//不能吃子
	{
		if(move->m_Type == 1||move->m_Type == -1)//普通棋子
		{   //起点减掉
			m_HashKey32 = m_HashKey32 ^ m_nHashKey32[move->m_Side * move->m_Type][iPos][jPos]; 
			m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[move->m_Side * move->m_Type][iPos][jPos]; 
			//终点添加
			switch (move->step[0])
			{
			case Right_Up: 
				m_HashKey32 = m_HashKey32 ^ m_nHashKey32[move->m_Side * move->m_Type][iPos+1][jPos-1]; 
				m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[move->m_Side * move->m_Type][iPos+1][jPos-1]; 
				break;
			case Left_Up:
				m_HashKey32 = m_HashKey32 ^ m_nHashKey32[move->m_Side * move->m_Type][iPos+1][jPos+1]; 
				m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[move->m_Side * move->m_Type][iPos+1][jPos+1]; 
				break;
			case Right_Down:
				m_HashKey32 = m_HashKey32 ^ m_nHashKey32[move->m_Side * move->m_Type][iPos-1][jPos-1]; 
				m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[move->m_Side * move->m_Type][iPos-1][jPos-1]; 
				break;
			case Left_Down:
				m_HashKey32 = m_HashKey32 ^ m_nHashKey32[move->m_Side * move->m_Type][iPos-1][jPos+1]; 
				m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[move->m_Side * move->m_Type][iPos-1][jPos+1]; 
				break;
			}
		}
		else//王
		{   //起点减掉
			m_HashKey32 = m_HashKey32 ^ m_nHashKey32[move->m_Side * move->m_Type][iPos][jPos]; 
			m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[move->m_Side * move->m_Type][iPos][jPos]; 
			//终点添加
			switch (move->step[0])
			{
			case Right_Up:
				m_HashKey32 = m_HashKey32 ^ m_nHashKey32[move->m_Side * move->m_Type][iPos+1+move->step_king_kongge1[0]][jPos-1-move->step_king_kongge1[0]]; 
				m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[move->m_Side * move->m_Type][iPos+1+move->step_king_kongge1[0]][jPos-1-move->step_king_kongge1[0]]; 
				break;
			case Left_Up:
				m_HashKey32 = m_HashKey32 ^ m_nHashKey32[move->m_Side * move->m_Type][iPos+1+move->step_king_kongge1[0]][jPos+1+move->step_king_kongge1[0]]; 
				m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[move->m_Side * move->m_Type][iPos+1+move->step_king_kongge1[0]][jPos+1+move->step_king_kongge1[0]]; 
				break;
			case Right_Down:
				m_HashKey32 = m_HashKey32 ^ m_nHashKey32[move->m_Side * move->m_Type][iPos-1-move->step_king_kongge1[0]][jPos-1-move->step_king_kongge1[0]]; 
				m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[move->m_Side * move->m_Type][iPos-1-move->step_king_kongge1[0]][jPos-1-move->step_king_kongge1[0]]; 
				break;
			case Left_Down:
				m_HashKey32 = m_HashKey32 ^ m_nHashKey32[move->m_Side * move->m_Type][iPos-1-move->step_king_kongge1[0]][jPos+1+move->step_king_kongge1[0]]; 
				m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[move->m_Side * move->m_Type][iPos-1-move->step_king_kongge1[0]][jPos+1+move->step_king_kongge1[0]]; 
				break;
			}
		}
	}
	else                  //能够吃子
	{
		if(move->m_Type == 1||move->m_Type == -1)//普通棋子
		{   //起点减掉
			m_HashKey32 = m_HashKey32 ^ m_nHashKey32[move->m_Side * move->m_Type][iPos][jPos]; 
			m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[move->m_Side * move->m_Type][iPos][jPos]; 
			for(int i=move->m_Count-1;i>=0;i--)
			{
				//过程减掉
				switch (move->step[i])
				{
				case Right_Up:
					m_HashKey32 = m_HashKey32 ^ m_nHashKey32[m_ntype[i]][iPos+1][jPos-1]; 
					m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[m_ntype[i]][iPos+1][jPos-1]; 
					iPos += 2;
					jPos -= 2;
					break;
				case Left_Up:
					m_HashKey32 = m_HashKey32 ^ m_nHashKey32[m_ntype[i]][iPos+1][jPos+1]; 
					m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[m_ntype[i]][iPos+1][jPos+1]; 
					iPos += 2;
					jPos += 2;
					break;
				case Right_Down:
					m_HashKey32 = m_HashKey32 ^ m_nHashKey32[m_ntype[i]][iPos-1][jPos-1]; 
					m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[m_ntype[i]][iPos-1][jPos-1]; 
					iPos -= 2;
					jPos -= 2;
					break;
				case Left_Down:
					m_HashKey32 = m_HashKey32 ^ m_nHashKey32[m_ntype[i]][iPos-1][jPos+1]; 
					m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[m_ntype[i]][iPos-1][jPos+1]; 
					iPos -= 2;
					jPos += 2;
					break;
				}
			}
			//终点添加
			m_HashKey32 = m_HashKey32 ^ m_nHashKey32[move->m_Side * move->m_Type][iPos][jPos]; 
			m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[move->m_Side * move->m_Type][iPos][jPos]; 
		}
		else//王
		{
			 //起点减掉
			m_HashKey32 = m_HashKey32 ^ m_nHashKey32[move->m_Side * move->m_Type][iPos][jPos]; 
			m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[move->m_Side * move->m_Type][iPos][jPos]; 
			for(int i=move->m_Count-1;i>=0;i--)
			{
				//过程减掉
				switch (move->step[i])
				{
				case Right_Up:
					m_HashKey32 = m_HashKey32 ^ m_nHashKey32[m_ntype[i]][iPos+move->step_king_kongge2[i]][jPos-move->step_king_kongge2[i]]; 
					m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[m_ntype[i]][iPos+move->step_king_kongge2[i]][jPos-move->step_king_kongge2[i]]; 
					iPos += (1+move->step_king_kongge1[i]+move->step_king_kongge2[i]);
					jPos -= (1+move->step_king_kongge1[i]+move->step_king_kongge2[i]);
					break;
				case Left_Up:
					m_HashKey32 = m_HashKey32 ^ m_nHashKey32[m_ntype[i]][iPos+move->step_king_kongge2[i]][jPos+move->step_king_kongge2[i]]; 
					m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[m_ntype[i]][iPos+move->step_king_kongge2[i]][jPos+move->step_king_kongge2[i]]; 
					iPos += (1+move->step_king_kongge1[i]+move->step_king_kongge2[i]);
					jPos += (1+move->step_king_kongge1[i]+move->step_king_kongge2[i]);
					break;
				case Right_Down:
					m_HashKey32 = m_HashKey32 ^ m_nHashKey32[m_ntype[i]][iPos-move->step_king_kongge2[i]][jPos-move->step_king_kongge2[i]]; 
					m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[m_ntype[i]][iPos-move->step_king_kongge2[i]][jPos-move->step_king_kongge2[i]]; 
					iPos -= (1+move->step_king_kongge1[i]+move->step_king_kongge2[i]);
					jPos -= (1+move->step_king_kongge1[i]+move->step_king_kongge2[i]);
					break;
				case Left_Down:
					m_HashKey32 = m_HashKey32 ^ m_nHashKey32[m_ntype[i]][iPos-move->step_king_kongge2[i]][jPos+move->step_king_kongge2[i]]; 
					m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[m_ntype[i]][iPos-move->step_king_kongge2[i]][jPos+move->step_king_kongge2[i]]; 
					iPos -= (1+move->step_king_kongge1[i]+move->step_king_kongge2[i]);
					jPos += (1+move->step_king_kongge1[i]+move->step_king_kongge2[i]);
					break;
				}
			}
			//终点添加
			m_HashKey32 = m_HashKey32 ^ m_nHashKey32[move->m_Side * move->m_Type][iPos][jPos]; 
			m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[move->m_Side * move->m_Type][iPos][jPos]; 
		}
	}
}

int CTranspositionTable::LookUpHashTable(int alpha, int beta, int depth,int TableNo)
{
	int x;
	HashItem * pht;

	x = m_HashKey32 & 0xFFFFF;
	pht = &m_pTT[TableNo][x];

    if (pht->depth >= depth && pht->checksum == m_HashKey64)
	{
		switch (pht->entry_type) 
		{
		case exact: 
			return pht->eval;
		case lower_bound:
			if (pht->eval >= beta)
				return (pht->eval);
			else 
				break;
		case upper_bound:
			if (pht->eval <= alpha)
				return (pht->eval);
			else 
				break;
        }
	}

	return 6666666;
}
void CTranspositionTable::EnterHashTable(ENTRY_TYPE entry_type, int eval, short depth,int TableNo)
{
	int x;
	HashItem * pht;

	x = m_HashKey32 & 0xFFFFF;//二十位哈希地址
	pht = &m_pTT[TableNo][x];

	pht->checksum = m_HashKey64;
	pht->entry_type = entry_type;
	pht->eval = eval;
	pht->depth = depth;

}
