#include "stdafx.h"
#include"MoveGenerator.h"
//记录m_nMoveList中走法的数量
	int m_nMoveCount;
	CHESSPOS m_qi[20];
	int m_type[20];
	int m_count[20];//记录每步的行棋深度
	int m_step[20][10][10]; //左下下==1 右下下==2  左上上==3   右上上==4
	int m_step_king_kongge1[20][10][10];
	int m_step_king_kongge2[20][10][10];
	int m_lianJ[20];  //同一颗棋子的走法 个数
	int m_MaxLen;
	int m_MaxPos[20];
	Node_YouSha* m_pHead;
	Node_YouSha* m_pEnd;
//构造
CMoveGenerator::CMoveGenerator()//初始化
{
	m_pHead = NULL;
	m_pEnd = NULL;
	m_MaxLen = 0;
	//记录m_nMoveList中走法的数量
	m_nMoveCount = 0;
	for(int i=0;i < 20;i++)
	{
		m_MaxPos[i] = -1;
		m_lianJ[i] = 0;
		m_type[i] = 0;
	    m_count[i] = 0;//记录每步的行棋深度
		for(int j=0;j < 20;j++)
		{
			for(int k=0;k < 20;k++)
			{
				m_step[i][j][k] = 0; //左下下==1 右下下==2  左上上==3   右上上==4
				m_step_king_kongge1[i][j][k] = 0;
				m_step_king_kongge2[i][j][k] = 0;
			}
		}
	}
}
//产生给定棋盘上的所有合法的走法
int CMoveGenerator::CreatePossibleMove(int position[10][10],int nPly,int nSide)
{
	int nCount = -1;
	int nMethod = 0;
	ClearData();
	for(int i=0;i < 10;i++)
	{
		for(int j=0;j < 10;j++)
		{
			//1.找到自己棋子-记录信息
			if(nSide == position[i][j] || WANG*nSide == position[i][j])//2.普通棋子
			{
				nCount++;
				//递归寻找-存到链表里，找到该棋子所有走法
				CommonCode(i,j,position[i][j],0,0,nSide,position,nCount);
				//将这颗棋子的走法记录下来，保留这个棋子杀棋最多的走法
				NodeCount(nCount);
			}
		}
	}
	for(int i=0;i<10;i++)
			for(int j=0;j<10;j++)
				if(3 == position[i][j])
					position[i][j] = 0;
	if(true == PangDuan_ShaShangli())//可以杀棋
	{   //几颗棋子可以杀
		for(int i=0;i<m_MaxLen;i++)
		{   //每颗棋子不同杀法
			for(int j=0;j<m_lianJ[m_MaxPos[i]];j++)
			{   //装进去
				AddMove_ShaQi(i,j,nSide,nPly,nMethod);
			}
		}
	}
	else                       //不能杀棋
	{
		nCount = -1;
		int kongge1 = 0;//王用
		for(int i=0;i<10;i++)
		{
			for(int j=0;j<10;j++)
			{
				if(nSide == position[i][j])
				{
					nCount++;
					m_qi[nCount].x = j;
					m_qi[nCount].y = i;
					if(nSide == WHITE)
					{
						if(i<9 && j>0 && 0==position[i+1][j-1])//左下
						{
							AddMove_BuJv(nCount,nSide,Qi,Left_Down,nPly,nMethod);
						}
						if(i<9 && j<9 && 0==position[i+1][j+1])//右下
						{
							AddMove_BuJv(nCount,nSide,Qi,Right_Down,nPly,nMethod);
						}
					}
					if(nSide == BLACK)
					{
						if(i>0 && j>0 && 0==position[i-1][j-1])//左上
						{
							AddMove_BuJv(nCount,nSide,Qi,Left_Up,nPly,nMethod);
						}
						if(i>0 && j<9 && 0==position[i-1][j+1])//右上
						{
							AddMove_BuJv(nCount,nSide,Qi,Right_Up,nPly,nMethod);
						}
					}
				}
				else if(WANG*nSide == position[i][j])
				{
					nCount++;
					m_qi[nCount].x = j;
					m_qi[nCount].y = i;

					kongge1 = ZuoXia(i,j,position);//左下
					for(int k=0;k<kongge1;k++)
					{
						AddMove_BuJv(nCount,nSide,WANG,Left_Down,nPly,nMethod,k);
					}
					kongge1 = YouXia(i,j,position);//右下
					for(int k=0;k<kongge1;k++)
					{
						AddMove_BuJv(nCount,nSide,WANG,Right_Down,nPly,nMethod,k);
					}
					kongge1 = ZuoShang(i,j,position);//左上
					for(int k=0;k<kongge1;k++)
					{
						AddMove_BuJv(nCount,nSide,WANG,Left_Up,nPly,nMethod,k);
					}
					kongge1 = YouShang(i,j,position);//右上
					for(int k=0;k<kongge1;k++)
					{
						AddMove_BuJv(nCount,nSide,WANG,Right_Up,nPly,nMethod,k);
					}
				}
			}
		}
	}
	return nMethod;
}
//类内函数可能用到的函数方法
void CMoveGenerator::CommonCode(/*位置*/int i,int j,/*价值*/int value,/*层次*/int shu,/*上一次杀棋方向*/int fangxiang,/*哪一边*/int TURN,int Board[10][10],/*第几颗棋子*/int nCount)
{
	int kongge1=0;//记录空格数，王用
	int kongge2=0;
	int m=0;      //王的循环中用到
	int ntype;  //记录被吃的棋子值
	if(TURN == value)
	{
		if(-1 != nCount)
			m_type[nCount] = Qi;//记录普通棋子
		if(j>=2 && i<=7 && Right_Up!=fangxiang)//检测可以左下下 边界
		{   //检测左下方是对方的，左下下是无子
			if((Board[i+1][j-1]==(-1*TURN) || Board[i+1][j-1]==(-1*WANG*TURN)) && 0==Board[i+2][j-2])
			{
				if(-1 != nCount)
				{   //第一次进来记录棋子坐标，以后进入不在记录
					m_qi[nCount].x = j;
					m_qi[nCount].y = i;
				}
				ntype = Board[i+1][j-1]; //记录被吃
				addLike(&m_pHead,&m_pEnd,shu+1,Left_Down);
				Move_z_x_x(i,j,Board);
				CommonCode(i+2,j-2,1*TURN,shu+1,Left_Down,TURN,Board);
				MoveF_y_s_s (i+2,j-2,ntype,Board);
			}
		}
		if(j<=7 && i<=7 && Left_Up!=fangxiang)//检测可以右下下
		{
			if((Board[i+1][j+1]==(-1*TURN) || Board[i+1][j+1]==(-1*WANG*TURN))&&0==Board[i+2][j+2])//检测右上方是对方的，右上上是无子
			{
				if(-1 != nCount)
				{
					m_qi[nCount].x = j;
					m_qi[nCount].y = i;
				}
				ntype = Board[i+1][j+1];
				addLike(&m_pHead,&m_pEnd,shu+1,Right_Down);
				Move_y_x_x(i,j,Board);
				CommonCode(i+2,j+2,1*TURN,shu+1,Right_Down,TURN,Board);
				MoveF_z_s_s (i+2,j+2,ntype,Board);	
			}
		}
		if(j>=2 && i>=2 && Right_Down!=fangxiang)//检测可以左上上
		{
			if((Board[i-1][j-1]==(-1*TURN) || Board[i-1][j-1]==(-1*WANG*TURN))&&0==Board[i-2][j-2])//检测左上方是对方的，左上上是无子
			{
				if(-1 != nCount)
				{
					m_qi[nCount].x = j;
					m_qi[nCount].y = i;
				}
				ntype = Board[i-1][j-1];
				addLike(&m_pHead,&m_pEnd,shu+1,Left_Up);
				Move_z_s_s(i,j,Board);
				CommonCode(i-2,j-2,1*TURN,shu+1,Left_Up,TURN,Board);
				MoveF_y_x_x (i-2,j-2,ntype,Board);
			}
		}
		if( j<=7 && i>=2 && Left_Down!=fangxiang)//检测可以右上上
		{
			if((Board[i-1][j+1]==(-1*TURN) || Board[i-1][j+1]==(-1*WANG*TURN))&&0==Board[i-2][j+2])//检测右上方是对方的，右上上是无子
			{
				if(-1 != nCount)
				{
					m_qi[nCount].x = j;
					m_qi[nCount].y = i;
				}
				ntype = Board[i-1][j+1];
				addLike(&m_pHead,&m_pEnd,shu+1,Right_Up);
				Move_y_s_s(i,j,Board);
				CommonCode(i-2,j+2,1*TURN,shu+1,Right_Up,TURN,Board);
				MoveF_z_x_x (i-2,j+2,ntype,Board);
			}
		}
	}
	if(2*TURN==value)
	{
		if(-1 != nCount)
			m_type[nCount] = WANG;//记录王棋
		if(j>=2 && i<=7 && Right_Up!=fangxiang)//检测可以左下下
		{
			kongge1=ZuoXia (i,j,Board);
			kongge2=ZuoXia (i+kongge1+1,j-kongge1-1,Board);
			//检测左上方是对方的，左上上是无子
			if(kongge2>0&& ((-1*Qi*TURN)==Board[i+kongge1+1][j-kongge1-1] || (-1*WANG*TURN)==Board[i+kongge1+1][j-kongge1-1]))
			{
				if(-1 != nCount)
				{
					m_qi[nCount].x = j;
					m_qi[nCount].y = i;
				}
				ntype = Board[i+kongge1+1][j-kongge1-1];
				for(m=1;m<=kongge2;m++)
				{
					addLike(&m_pHead,&m_pEnd,shu+1,Left_Down,kongge1,m);
					Move_z_x_x_Wang(i,j,kongge1,m,TURN,Board);
					CommonCode(i+kongge1+m+1,j-kongge1-m-1,2*TURN,shu+1,Left_Down,TURN,Board);
					MoveF_y_s_s_Wang(i+kongge1+m+1,j-kongge1-m-1,kongge1,m,TURN,ntype,Board);
				}
			}
		}
		if(j<=7 && i<=7 && Left_Up!=fangxiang)//检测可以右下下
		{
			kongge1=YouXia (i,j,Board);
			kongge2=YouXia (i+kongge1+1,j+kongge1+1,Board);
			if(kongge2>0&&((-1*Qi*TURN)==Board[i+kongge1+1][j+kongge1+1] || (-1*WANG*TURN)==Board[i+kongge1+1][j+kongge1+1]))//检测左上方是对方的，左上上是无子
			{
				if(-1 != nCount)
				{
					m_qi[nCount].x = j;
					m_qi[nCount].y = i;
				}
				ntype = Board[i+kongge1+1][j+kongge1+1];
				for(m=1;m<=kongge2;m++)
				{
					addLike(&m_pHead,&m_pEnd,shu+1,Right_Down,kongge1,m);
					Move_y_x_x_Wang(i,j,kongge1,m,TURN,Board);
					CommonCode(i+kongge1+m+1,j+kongge1+m+1,2*TURN,shu+1,Right_Down,TURN,Board);
					MoveF_z_s_s_Wang(i+kongge1+m+1,j+kongge1+m+1,kongge1,m,TURN,ntype,Board);
				}
			}
		}
		if(j>=2 && i>=2 && Right_Down!=fangxiang)//检测可以左上上
		{
			kongge1=ZuoShang (i,j,Board);
			kongge2=ZuoShang (i-kongge1-1,j-kongge1-1,Board);
			if(kongge2>0&&((-1*Qi*TURN)==Board[i-kongge1-1][j-kongge1-1] ||(-1*WANG*TURN)==Board[i-kongge1-1][j-kongge1-1]))//检测左上方是对方的，左上上是无子
			{
				if(-1 != nCount)
				{
					m_qi[nCount].x = j;
					m_qi[nCount].y = i;
				}
				ntype = Board[i-kongge1-1][j-kongge1-1];
				for(m=1;m<=kongge2;m++)
				{
					addLike(&m_pHead,&m_pEnd,shu+1,Left_Up,kongge1,m);
					Move_z_s_s_Wang(i,j,kongge1,m,TURN,Board);
					CommonCode(i-kongge1-m-1,j-kongge1-m-1,2*TURN,shu+1,Left_Up,TURN,Board);
					MoveF_y_x_x_Wang(i-kongge1-m-1,j-kongge1-m-1,kongge1,m,TURN,ntype,Board);
				}
			}
		}
		if(j<=7 && i>=2 && Left_Down!=fangxiang)//检测可以右上上
		{
			kongge1=YouShang (i,j,Board);
			kongge2=YouShang (i-kongge1-1,j+kongge1+1,Board);
			if(kongge2>0&&((-1*Qi*TURN)==Board[i-kongge1-1][j+kongge1+1] ||(-1*WANG*TURN)==Board[i-kongge1-1][j+kongge1+1]))//检测左上方是对方的，左上上是无子
			{
				if(-1 != nCount)
				{
					m_qi[nCount].x = j;
					m_qi[nCount].y = i;
				}
				ntype = Board[i-kongge1-1][j+kongge1+1];
				for(m=1;m<=kongge2;m++)
				{
					addLike(&m_pHead,&m_pEnd,shu+1,Right_Up,kongge1,m);
					Move_y_s_s_Wang(i,j,kongge1,m,TURN,Board);
					CommonCode(i-kongge1-m-1,j+kongge1+m+1,2*TURN,shu+1,Right_Up,TURN,Board);
					MoveF_z_x_x_Wang(i-kongge1-m-1,j+kongge1+m+1,kongge1,m,TURN,ntype,Board);
				}
			}
		}
	}
}
//链表计数.，暂时考虑可以杀20个
void CMoveGenerator::NodeCount(int nCount)//value是表示当前是普通还是王
{
	int l,lianMax=0,lianI=0;
	int MaxPos[600]={0};
	int chu1[600]={0};//深度
	int chu2[600]={0};//深度对应行棋方向
	int kong1[600]={0};//深度对应两棋子之间空格数，，，王用
	int kong2[600]={0};//深度对应对方棋子后空格数，，，王用
	Node_YouSha *pp=NULL;
	//链表循环，将值付给一个数组
	if(m_pHead != NULL)
	{
		//把数组请0 
		lianMax=0;
		for(l=0;l<600;l++)
		{
			chu1[l]=0;
			chu2[l]=0;
			kong1[l]=0;
			kong2[l]=0;
			MaxPos[l]=0;
		}
		l=0;
		pp = m_pHead;
		while(pp != NULL)
		{
			chu1[l]=pp->shendu ;
			chu2[l]=pp->tongdao ;
			kong1[l]=pp->kongge1 ;
			kong2[l]=pp->kongge2 ;
			l++;
			pp = pp->pNext ;
		}
		//释放链表
		ClearLike(&m_pHead);
		m_pHead=NULL;
		m_pEnd=NULL;
		lianMax = chu1[0];
		for(lianI=0;lianI<l;lianI++)
		{//数组数据处理，找到最大的值
			if(lianMax < chu1[lianI])
			{//找到最大，记录最大深度
				lianMax = chu1[lianI];
			}
		}
		m_lianJ[nCount] = 0;//走法初始化为0
		for(lianI=0;lianI<l;lianI++)
		{//遍历编标所有节点
			if(lianMax == chu1[lianI])
			{//找到深度相符的位置，记录位置，走法+1
				MaxPos[m_lianJ[nCount]]=lianI;
				m_lianJ[nCount]++;
			}
		}
		//把值放到最后的数组中
		for(lianI=0;lianI<m_lianJ[nCount];lianI++)
		{
			switch(lianMax)
			{
			case 1:
				CommonFind(1,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 2:
				CommonFind(2,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 3:
				CommonFind(3,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 4:
				CommonFind(4,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 5:
				CommonFind(5,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 6:
				CommonFind(6,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 7:
				CommonFind(7,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 8:
				CommonFind(8,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 9:
				CommonFind(9,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 10:
				CommonFind(10,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 11:
				CommonFind(11,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 12:
				CommonFind(12,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 13:
				CommonFind(13,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 14:
				CommonFind(14,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 15:
				CommonFind(15,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 16:
				CommonFind(16,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 17:
				CommonFind(17,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 18:
				CommonFind(18,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 19:
				CommonFind(19,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 20:
				CommonFind(20,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			}
		}	
	}
}
void CMoveGenerator::CommonFind(int depth,int nCount,int lianI,int chu1[600],int chu2[600],int MaxPos[600],int kong1[600],int kong2[600])
{
	m_count[nCount] = depth;//2 记录深度
	m_step[nCount][lianI][depth-1]=chu2[MaxPos[lianI]];//记录方向
	m_step_king_kongge1[nCount][lianI][depth-1]= kong1[MaxPos[lianI]];//记录空格
	m_step_king_kongge2[nCount][lianI][depth-1]= kong2[MaxPos[lianI]];
	for(int m=0;m<depth-1;m++)
	{
		for(int k=MaxPos[lianI];k>=0;k--)
		{
			if(depth-1-m == chu1[k-1])
			{
				m_step[nCount][lianI][depth-2-m]=chu2[k-1];
				m_step_king_kongge1[nCount][lianI][depth-2-m]= kong1[k-1];
				m_step_king_kongge2[nCount][lianI][depth-2-m]= kong2[k-1];
				MaxPos[lianI] = k-1;//寻找位置已经前移，重新赋值
				break;
			}
			MaxPos[lianI] = k-1;////寻找位置已经前移，重新赋值
		}
	}
}
//判断杀伤力,杀得最多的棋子数
bool CMoveGenerator::PangDuan_ShaShangli()
{
	short max = 0;
	m_MaxLen = 0;
	max = m_count[0];
	//找到最大杀伤力max
	for(int i=1;i<20;i++)
	{
		if(m_count[i] > max)
		{
			max = m_count[i];
		}
	}
	if(0 == max)//不能杀棋
	{
		return false;
	}//保存信息，记录哪个棋子可以杀棋最多
	for(int i=0;i<20;i++)
	{
		if(m_count[i] == max)
		{
			m_MaxPos[m_MaxLen] = i;
			m_MaxLen++;
			//break;
		}
	}
	return true;
}
//插入一个走法 布局 杀棋
void CMoveGenerator::AddMove_BuJv(int nCount,int nSide,int nType,int mFangXiang,int nPly,int& nMethod,int nKongge)
{
	CHESSMOVE move;
	move.From = m_qi[nCount];
	move.m_Count = 0;
	move.m_Side = nSide;
	move.m_Type = nType;
	move.step[0] = mFangXiang;
	move.step_king_kongge1[0] = nKongge;
	m_nMoveList[nPly][nMethod] = move;
	nMethod++;
}
void CMoveGenerator::AddMove_ShaQi(int i,int j,int nSide,int nPly,int& nMethod)
{
	CHESSMOVE move;
	move.From = m_qi[m_MaxPos[i]];
	move.m_Count = m_count[m_MaxPos[i]];
	move.m_Side = nSide;
	move.m_Type = m_type[m_MaxPos[i]];
	for(int k=0;k<m_count[m_MaxPos[i]];k++)
	{
		move.step[k] = m_step[m_MaxPos[i]][j][k]; 
		move.step_king_kongge1[k] = m_step_king_kongge1[m_MaxPos[i]][j][k]; 
		move.step_king_kongge2[k] = m_step_king_kongge2[m_MaxPos[i]][j][k]; 
	}
	m_nMoveList[nPly][nMethod] = move;
	nMethod++;
}
//模拟杀棋
int CMoveGenerator::Move_z_x_x(int i,int j,int Board[10][10]) 
{
	int ntype = Board[i+1][j-1];
	Board[i+2][j-2]=Board[i][j];
	Board[i][j]=0;
	Board[i+1][j-1]=0;
	return ntype;
}
void CMoveGenerator::MoveF_z_x_x(int i,int j,int n,int Board[10][10]) 
{
	Board[i+2][j-2]=Board[i][j];
	Board[i][j]=0;
	Board[i+1][j-1]=n;
}
int CMoveGenerator::Move_z_s_s(int i,int j,int Board[10][10])
{
	int ntype = Board[i-1][j-1];
	Board[i-2][j-2]=Board[i][j];
	Board[i][j]=0;
	Board[i-1][j-1]=0;
	return ntype;
}
void CMoveGenerator::MoveF_z_s_s(int i,int j,int n,int Board[10][10]) 
{
	Board[i-2][j-2]=Board[i][j];
	Board[i][j]=0;
	Board[i-1][j-1]=n;
}
int CMoveGenerator::Move_y_x_x(int i,int j,int Board[10][10]) 
{
	int ntype = Board[i+1][j+1];
	Board[i+2][j+2]=Board[i][j];
	Board[i][j]=0;
	Board[i+1][j+1]=0;
	return ntype;
}
void CMoveGenerator::MoveF_y_x_x(int i,int j,int n,int Board[10][10]) 
{
	Board[i+2][j+2]=Board[i][j];
	Board[i][j]=0;
	Board[i+1][j+1]=n;
}
int CMoveGenerator::Move_y_s_s(int i,int j,int Board[10][10]) 
{
	int ntype = Board[i-1][j+1];
	Board[i-2][j+2]=Board[i][j];
	Board[i][j]=0;
	Board[i-1][j+1]=0;
	return ntype;
}
void CMoveGenerator::MoveF_y_s_s(int i,int j,int n,int Board[10][10])
{
	Board[i-2][j+2]=Board[i][j];
	Board[i][j]=0;
	Board[i-1][j+1]=n;
}
int CMoveGenerator::Move_z_x_x_Wang(int i,int j,int kongge1,int m,int TURN,int Board[10][10]) 
{
	int ntype = Board[i+kongge1+1][j-kongge1-1];
	Board[i][j]=0;
	Board[i+kongge1+1][j-kongge1-1]=3;
	Board[i+kongge1+m+1][j-kongge1-m-1]=2*TURN;
	return ntype;
}
void CMoveGenerator::MoveF_z_x_x_Wang(int i,int j,int kongge1,int m,int TURN,int ntype,int Board[10][10]) 
{
	Board[i][j]=0;
	Board[i+m][j-m]=ntype;
	Board[i+kongge1+m+1][j-kongge1-m-1]=2*TURN;
}
int CMoveGenerator::Move_z_s_s_Wang(int i,int j,int kongge1,int m,int TURN,int Board[10][10])
{
	int ntype = Board[i-kongge1-1][j-kongge1-1];
	Board[i][j]=0;
	Board[i-kongge1-1][j-kongge1-1]=3;
	Board[i-kongge1-m-1][j-kongge1-m-1]=2*TURN;
	return ntype;
}
void CMoveGenerator::MoveF_z_s_s_Wang(int i,int j,int kongge1,int m,int TURN,int ntype,int Board[10][10]) 
{
	Board[i][j]=0;
	Board[i-m][j-m]=ntype;
	Board[i-kongge1-m-1][j-kongge1-m-1]=2*TURN;
}
int CMoveGenerator::Move_y_x_x_Wang(int i,int j,int kongge1,int m,int TURN,int Board[10][10]) 
{
	int ntype = Board[i+kongge1+1][j+kongge1+1];
	Board[i][j]=0;
	Board[i+kongge1+1][j+kongge1+1]=3;
	Board[i+kongge1+m+1][j+kongge1+m+1]=2*TURN;
	return ntype;
}
void CMoveGenerator::MoveF_y_x_x_Wang(int i,int j,int kongge1,int m,int TURN,int ntype,int Board[10][10]) 
{
	Board[i][j]=0;
	Board[i+m][j+m]=ntype;
	Board[i+kongge1+m+1][j+kongge1+m+1]=2*TURN;
}
int CMoveGenerator::Move_y_s_s_Wang(int i,int j,int kongge1,int m,int TURN,int Board[10][10]) 
{
	int ntype = Board[i-kongge1-1][j+kongge1+1];
	Board[i][j]=0;
	Board[i-kongge1-1][j+kongge1+1]=3;
	Board[i-kongge1-m-1][j+kongge1+m+1]=2*TURN;
	return ntype;
}
void CMoveGenerator::MoveF_y_s_s_Wang(int i,int j,int kongge1,int m,int TURN,int ntype,int Board[10][10])
{
	Board[i][j]=0;
	Board[i-m][j+m]=ntype;
	Board[i-kongge1-m-1][j+kongge1+m+1]=2*TURN;
}
//链表处理函数
void CMoveGenerator::addLike(Node_YouSha **pHead,Node_YouSha **pEnd,int sd,int td,int kg1,int kg2)
{
	Node_YouSha *pNew=(Node_YouSha *)malloc(sizeof(Node_YouSha));
	pNew->shendu=sd;
	pNew->tongdao=td;
	pNew->kongge1 =kg1;
	pNew->kongge2 =kg2;
	pNew->pNext=NULL;
	if(NULL==*pHead)
	{
		*pHead=pNew;
	}else
	{
		(*pEnd)->pNext=pNew;
	}
	*pEnd=pNew;
}
void CMoveGenerator::ClearLike(Node_YouSha **ppHead)
{
	Node_YouSha *pDel=NULL;
	while(*ppHead!=NULL)
	{
		pDel=*ppHead;
		(*ppHead)=(*ppHead)->pNext;
		free(pDel);
	}
}
void CMoveGenerator::ClearData()
{
	m_MaxLen = 0;
	m_nMoveCount = 0;
	for(int i=0;i<20;i++)
	{   
		m_qi[i].x = 0;
		m_qi[i].y = 0;
		m_type[i] = 0;
		m_count[i] = 0;
		m_lianJ[i] = 0;
		m_MaxPos[i] = 0;
		for(int j=0;j<10;j++)
		{
			for(int k=0;k<10;k++)
			{
				m_step[i][j][k] = 0; //左下下==1 右下下==2  左上上==3   右上上==4
				m_step_king_kongge1[i][j][k] = 0;
				m_step_king_kongge2[i][j][k] = 0;
			}
		}
	}
}
//析构
CMoveGenerator::~CMoveGenerator()
{

}

