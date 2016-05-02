#include "stdafx.h"
#include "PublicToMakeMove.h"
CPublicToMakeMove::CPublicToMakeMove()
{
	m_bIsBecomeKing = FALSE;
}

CPublicToMakeMove::~CPublicToMakeMove()
{

}
void CPublicToMakeMove::MakeMoveQi(CHESSMOVE* move,int CurPosition[10][10],int type)
{
	for(int i=0;i<move->m_Count;i++)
	{
		switch(move->step[i])
		{
		case Left_Down:
			m_ntype[i] = m_pMG1.Move_z_x_x(move->From.y,move->From.x,CurPosition);
			move->From.y += 2;
			move->From.x -= 2;
			break;
		case Right_Down:
			m_ntype[i] = m_pMG1.Move_y_x_x(move->From.y,move->From.x,CurPosition);
			move->From.y += 2;
			move->From.x += 2;
			break;
		case Left_Up:
			m_ntype[i] = m_pMG1.Move_z_s_s(move->From.y,move->From.x,CurPosition);
			move->From.y -= 2;
			move->From.x -= 2;
			break;
		case Right_Up:
			m_ntype[i] = m_pMG1.Move_y_s_s(move->From.y,move->From.x,CurPosition);
			move->From.y -= 2;
			move->From.x += 2;
			break;
		}
	}
	
	if(0 == move->m_Count)//没有吃子，只有行棋
	{
		switch(move->step[0])
		{
		case Left_Down:
			CurPosition[move->From.y+1][move->From.x-1] = CurPosition[move->From.y][move->From.x]; 
			CurPosition[move->From.y][move->From.x] = 0;
			move->From.y += 1;
			move->From.x -= 1;
			break;
		case Right_Down:
			CurPosition[move->From.y+1][move->From.x+1] = CurPosition[move->From.y][move->From.x]; 
			CurPosition[move->From.y][move->From.x] = 0;
			move->From.y += 1;
			move->From.x += 1;
			break;
		case Left_Up:
			CurPosition[move->From.y-1][move->From.x-1] = CurPosition[move->From.y][move->From.x]; 
			CurPosition[move->From.y][move->From.x] = 0;
			move->From.y -= 1;
			move->From.x -= 1;
			break;
		case Right_Up:
			CurPosition[move->From.y-1][move->From.x+1] = CurPosition[move->From.y][move->From.x]; 
			CurPosition[move->From.y][move->From.x] = 0;
			move->From.y -= 1;
			move->From.x += 1;
			break;
		}
		if(0 != type && 9 != move->From.y)
		{
			CurPosition[move->From.y][move->From.x] = 4;
		}
	}
	else
	{
		if(0 != type && 9 != move->From.y)
		{
			CurPosition[move->From.y][move->From.x] = 4;
		}
	}
	BecomeKing(CurPosition);
}
void CPublicToMakeMove::UnMakeMoveQi(CHESSMOVE* move,int CurPosition[10][10])
{
	if(TRUE == m_bIsBecomeKing)
	{
		CurPosition[move->From.y][move->From.x] = move->m_Side;
		m_bIsBecomeKing = FALSE;
	}
	for(int i=move->m_Count-1;i>=0;i--)
		{
			switch(move->step[i])
			{
			case Left_Down:
				m_pMG1.MoveF_y_s_s(move->From.y,move->From.x,m_ntype[i],CurPosition);
				move->From.y -= 2;
				move->From.x += 2;
				break;
			case Right_Down:
				m_pMG1.MoveF_z_s_s(move->From.y,move->From.x,m_ntype[i],CurPosition);
				move->From.y -= 2;
				move->From.x -= 2;
				break;
			case Left_Up:
				m_pMG1.MoveF_y_x_x(move->From.y,move->From.x,m_ntype[i],CurPosition);
				move->From.y += 2;
				move->From.x += 2;
				break;
			case Right_Up:
				m_pMG1.MoveF_z_x_x(move->From.y,move->From.x,m_ntype[i],CurPosition);
				move->From.y += 2;
				move->From.x -= 2;
				break;
			}
		}
		if(0 == move->m_Count)//没有吃子，只有行棋
		{
			switch(move->step[0])
			{
			case Left_Down:
				CurPosition[move->From.y-1][move->From.x+1] = CurPosition[move->From.y][move->From.x]; 
				CurPosition[move->From.y][move->From.x] = 0;
				move->From.y -= 1;
				move->From.x += 1;
				break;
			case Right_Down:
				CurPosition[move->From.y-1][move->From.x-1] = CurPosition[move->From.y][move->From.x]; 
				CurPosition[move->From.y][move->From.x] = 0;
				move->From.y -= 1;
				move->From.x -= 1;
				break;
			case Left_Up:
				CurPosition[move->From.y+1][move->From.x+1] = CurPosition[move->From.y][move->From.x]; 
				CurPosition[move->From.y][move->From.x] = 0;
				move->From.y += 1;
				move->From.x += 1;
				break;
			case Right_Up:
				CurPosition[move->From.y+1][move->From.x-1] = CurPosition[move->From.y][move->From.x]; 
				CurPosition[move->From.y][move->From.x] = 0;
				move->From.y += 1;
				move->From.x -= 1;
				break;
			}
		}
}
void CPublicToMakeMove::MakeMoveWANG(CHESSMOVE* move,int CurPosition[10][10],int type)
{
	for(int i=0;i<move->m_Count;i++)
		{
			switch(move->step[i])
			{
			case Left_Down:
				m_ntype[i] = m_pMG1.Move_z_x_x_Wang(move->From.y,move->From.x,move->step_king_kongge1[i],move->step_king_kongge2[i],move->m_Side,CurPosition);
				move->From.y += (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				move->From.x -= (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				break;
			case Right_Down:
				m_ntype[i] = m_pMG1.Move_y_x_x_Wang(move->From.y,move->From.x,move->step_king_kongge1[i],move->step_king_kongge2[i],move->m_Side,CurPosition);
				move->From.y += (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				move->From.x += (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				break;
			case Left_Up:
				m_ntype[i] = m_pMG1.Move_z_s_s_Wang(move->From.y,move->From.x,move->step_king_kongge1[i],move->step_king_kongge2[i],move->m_Side,CurPosition);
				move->From.y -= (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				move->From.x -= (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				break;
			case Right_Up:
				m_ntype[i] = m_pMG1.Move_y_s_s_Wang(move->From.y,move->From.x,move->step_king_kongge1[i],move->step_king_kongge2[i],move->m_Side,CurPosition);
				move->From.y -= (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				move->From.x += (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				break;
			}
		}
		for(int i=0;i<10;i++)
			for(int j=0;j<10;j++)
				if(3 == CurPosition[i][j])
					CurPosition[i][j] = 0;
		if(0 == move->m_Count)//没有吃子，只有行棋
		{
			switch(move->step[0])
			{
			case Left_Down:
				CurPosition[move->From.y+1+move->step_king_kongge1[0]][move->From.x-1-move->step_king_kongge1[0]] = CurPosition[move->From.y][move->From.x]; 
				CurPosition[move->From.y][move->From.x] = 0;
				move->From.y += (move->step_king_kongge1[0] + 1);
				move->From.x -= (move->step_king_kongge1[0] + 1);
				break;
			case Right_Down:
				CurPosition[move->From.y+1+move->step_king_kongge1[0]][move->From.x+1+move->step_king_kongge1[0]] = CurPosition[move->From.y][move->From.x]; 
				CurPosition[move->From.y][move->From.x] = 0;
				move->From.y += (move->step_king_kongge1[0] + 1);
				move->From.x += (move->step_king_kongge1[0] + 1);
				break;
			case Left_Up:
				CurPosition[move->From.y-1-move->step_king_kongge1[0]][move->From.x-1-move->step_king_kongge1[0]] = CurPosition[move->From.y][move->From.x]; 
				CurPosition[move->From.y][move->From.x] = 0;
				move->From.y -= (move->step_king_kongge1[0] + 1);
				move->From.x -= (move->step_king_kongge1[0] + 1);
				break;
			case Right_Up:
				CurPosition[move->From.y-1-move->step_king_kongge1[0]][move->From.x+1+move->step_king_kongge1[0]] = CurPosition[move->From.y][move->From.x]; 
				CurPosition[move->From.y][move->From.x] = 0;
				move->From.y -= (move->step_king_kongge1[0] + 1);
				move->From.x += (move->step_king_kongge1[0] + 1);
				break;
			}
			if(0 != type)
			{
				CurPosition[move->From.y][move->From.x] = -4;
			}
		}
		else
		{
			if(0 != type)
			{
				CurPosition[move->From.y][move->From.x] = -4;
			}
		}
}

void CPublicToMakeMove::UnMakeMoveWANG(CHESSMOVE* move,int CurPosition[10][10])
{
	for(int i=move->m_Count-1;i>=0;i--)
		{
			switch(move->step[i])
			{
			case Left_Down:
				m_pMG1.MoveF_y_s_s_Wang(move->From.y,move->From.x,move->step_king_kongge1[i],move->step_king_kongge2[i],move->m_Side,m_ntype[i],CurPosition);
				move->From.y -= (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				move->From.x += (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				break;
			case Right_Down:
				m_pMG1.MoveF_z_s_s_Wang(move->From.y,move->From.x,move->step_king_kongge1[i],move->step_king_kongge2[i],move->m_Side,m_ntype[i],CurPosition);
				move->From.y -= (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				move->From.x -= (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				break;
			case Left_Up:
				m_pMG1.MoveF_y_x_x_Wang(move->From.y,move->From.x,move->step_king_kongge1[i],move->step_king_kongge2[i],move->m_Side,m_ntype[i],CurPosition);
				move->From.y += (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				move->From.x += (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				break;
			case Right_Up:
				m_pMG1.MoveF_z_x_x_Wang(move->From.y,move->From.x,move->step_king_kongge1[i],move->step_king_kongge2[i],move->m_Side,m_ntype[i],CurPosition);
				move->From.y += (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				move->From.x -= (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				break;
			}
		}
		if(0 == move->m_Count)//没有吃子，只有行棋
		{
			switch(move->step[0])
			{
			case Left_Down:
				CurPosition[move->From.y-1-move->step_king_kongge1[0]][move->From.x+1+move->step_king_kongge1[0]] = CurPosition[move->From.y][move->From.x]; 
				CurPosition[move->From.y][move->From.x] = 0;
				move->From.y -= (move->step_king_kongge1[0] + 1);
				move->From.x += (move->step_king_kongge1[0] + 1);
				break;
			case Right_Down:
				CurPosition[move->From.y-1-move->step_king_kongge1[0]][move->From.x-1-move->step_king_kongge1[0]] = CurPosition[move->From.y][move->From.x]; 
				CurPosition[move->From.y][move->From.x] = 0;
				move->From.y -= (move->step_king_kongge1[0] + 1);
				move->From.x -= (move->step_king_kongge1[0] + 1);
				break;
			case Left_Up:
				CurPosition[move->From.y+1+move->step_king_kongge1[0]][move->From.x+1+move->step_king_kongge1[0]] = CurPosition[move->From.y][move->From.x]; 
				CurPosition[move->From.y][move->From.x] = 0;
				move->From.y += (move->step_king_kongge1[0] + 1);
				move->From.x += (move->step_king_kongge1[0] + 1);
				break;
			case Right_Up:
				CurPosition[move->From.y+1+move->step_king_kongge1[0]][move->From.x-1-move->step_king_kongge1[0]] = CurPosition[move->From.y][move->From.x]; 
				CurPosition[move->From.y][move->From.x] = 0;
				move->From.y += (move->step_king_kongge1[0] + 1);
				move->From.x -= (move->step_king_kongge1[0] + 1);
				break;
			}
		}
}
void CPublicToMakeMove::BecomeKing(int CurPosition[10][10])
{
	for(int i=0;i<10;i++)
	{
		if(1 == CurPosition[0][i])
		{
			CurPosition[0][i] = 2;
			m_bIsBecomeKing = TRUE;
		}
		if(-1 == CurPosition[9][i])
		{
			CurPosition[9][i] = -2;
			m_bIsBecomeKing = TRUE;
		}
	}

}