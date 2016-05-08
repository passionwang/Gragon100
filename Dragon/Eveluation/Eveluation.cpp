#include "stdafx.h"
#include "Eveluation.h"

//构造
CEveluation::CEveluation()
{
	//定义黑白棋各自数组
}
//析构
CEveluation::~CEveluation()
{

}
int G_nCountEv;//记录估值结点数
//估值
int CEveluation::Eveluate(int position[10][10], int TURE,int nCeng)
{
	G_nCountEv++;
	m_value_white = 0;//初始化分数
	m_value_black = 0;
	for(BYTE i=0;i<51;i++)//初始化黑白棋各自数组
	{
		m_shuzu_white[i] = 0;
		m_shuzu_black[i] = 0;
	}
	m_nntype = TURE;
	for(BYTE i=0;i<10;i++)//初始化棋盘
	{
		for(BYTE j=0;j<10;j++)
		{
			m_position[i][j] = position[i][j];
		}
	}
	count_qi_num();       //1.棋子数量
	count_bianxian_num(); //2.边线个数
	count_lian_num();     //3.链数量
	count_sanjiao_num();  //4.三角形数量
	count_zuozhongyou();  //5.左中右
	count_yazhi_num();    //6.压制数量
	AnQuanZhi();          //7安全值
	Kong();               //8孔
	ZhongJian();          //9.走两颗棋中间——————加400
	FangZhiChengWang();   //10.防止成王
	WY();                 //11.王压制
	if (nCeng)
	{
		return m_value_black - m_value_white;
	}
	else
	{
		return  m_value_white-m_value_black ;
	}
}
//1.计数，放置位置
void CEveluation::count_qi_num()
{
	m_num_white = 0;//初始化棋子个数
	m_num_black = 0;
	m_num_king_white = 0;
	m_num_king_black = 0;
	for(BYTE i=0;i<10;i++)
	{
		m_num_hang_white[i] = 0;//每行数量
		m_num_hang_black[i] = 0;
		m_num_lie_white[i] = 0;//每列数量
		m_num_lie_black[i] = 0;
	}
	for(BYTE i=0;i<10;i++)
	{
		for(BYTE j=0;j<10;j++)
		{
			if(-1 == m_position[i][j])//白棋
			{
				m_num_white++;//计算白棋普通子数
				m_num_hang_white[i]++;//计算各行白棋的棋子数
				m_num_lie_white[j]++;//计算每列棋子
				shuzuTwo_shuzuOne(i,j,m_shuzu_white);//将各自棋子位置放入对应的数组位置
			}
			if(-2 == m_position[i][j])//白王
			{
				m_num_king_white++;
				//m_num_hang_white[i]++;//主要考虑优先成王加分，故此处王的数量不加入
				m_num_lie_white[j]++;
				shuzuTwo_shuzuOne(i,j,m_shuzu_white);
			}
			if(1 == m_position[i][j])//黑棋
			{
				m_num_black++;
				m_num_hang_black[i]++;
				m_num_lie_black[j]++;
				shuzuTwo_shuzuOne(i,j,m_shuzu_black);
			}
			if(2 == m_position[i][j])//黑王
			{
				m_num_king_black++;
				//m_num_hang_black[i]++;
				m_num_lie_black[j]++;
				shuzuTwo_shuzuOne(i,j,m_shuzu_black);
			}
		}
	}
	//估值  
	m_value_white += 5000*m_num_white; //子力差值5000
	m_value_black += 5000*m_num_black;
	m_value_white += 40000*m_num_king_white;//王差值4000
	m_value_black += 40000*m_num_king_black;
	m_value_white += 80*m_num_hang_white[0];//行数加分//优先成王//白棋
	m_value_white += 85*m_num_hang_white[1];
	m_value_white += 90*m_num_hang_white[2];
	m_value_white += 95*m_num_hang_white[3];
	m_value_white += 100*m_num_hang_white[4];
	m_value_white += 110*m_num_hang_white[5];
	m_value_white += 120*m_num_hang_white[6];
	m_value_white += 150*m_num_hang_white[7];
	m_value_white += 180*m_num_hang_white[8];
	m_value_white += 200*m_num_hang_white[9];
	m_value_black += 200*m_num_hang_black[0];//黑棋
	m_value_black += 180*m_num_hang_black[1];
	m_value_black += 150*m_num_hang_black[2];
	m_value_black += 120*m_num_hang_black[3];
	m_value_black += 110*m_num_hang_black[4];
	m_value_black += 100*m_num_hang_black[5];
	m_value_black += 95*m_num_hang_black[6];
	m_value_black += 90*m_num_hang_black[7];
	m_value_black += 85*m_num_hang_black[8];
	m_value_black += 80*m_num_hang_black[9];
	if(m_num_white >=10 && m_num_black >= 10 )//列数加分//对峙//
	{   //起始状态此项不重要
		for(BYTE i=0;i<10;i++)
		{
			m_value_white += 30*m_num_lie_white[i];
			m_value_black += 30*m_num_lie_black[i];
		}
	}
	else
	{   //后期很重要
		for(BYTE i=0;i<10;i++)
		{
			m_value_white += 300*m_num_lie_white[i];
			m_value_black += 300*m_num_lie_black[i];
		}
	}
	if(m_num_white >=10 && m_num_black >= 10)//27.28.29位加分，50
	{
		m_value_white += 50*m_shuzu_white[22] + 50*m_shuzu_white[23] + 50*m_shuzu_white[24];
	    m_value_black += 50*m_shuzu_black[27] + 50*m_shuzu_black[28] + 50*m_shuzu_black[29];
	}
}
//2.边线个数
void CEveluation::count_bianxian_num()
{
	m_num_bianxian_white = 0;//边线个数
	m_num_bianxian_black=0;
	for(BYTE i=1;i<=5;i++)//白
	{  
		m_num_bianxian_white += m_shuzu_white[i]; //上//下//左//右
		m_num_bianxian_white += m_shuzu_white[i+45];
		m_num_bianxian_white += m_shuzu_white[10*(i-1)+6];
		m_num_bianxian_white += m_shuzu_white[10*(i-1)+5];
		m_num_bianxian_black += m_shuzu_black[i];//黑    //上//下//左//右
		m_num_bianxian_black += m_shuzu_black[i+45];
		m_num_bianxian_black += m_shuzu_black[10*(i-1)+6];
		m_num_bianxian_black += m_shuzu_black[10*(i-1)+5];
	}//去重
	m_num_bianxian_white = m_num_bianxian_white-m_shuzu_white[5]-m_shuzu_white[46];
	m_num_bianxian_black = m_num_bianxian_black-m_shuzu_black[5]-m_shuzu_black[46];
	//估值   
	m_value_white += 40*m_num_bianxian_white;//边线子力40
	m_value_black += 40*m_num_bianxian_black;
}
//3.链数量
void CEveluation::count_lian_num()
{
	m_num_lian_white = 0;//链的个数
	m_num_lian_black = 0;
	//白棋64
	m_num_lian_white = (m_shuzu_white[4]+m_shuzu_white[10]+m_shuzu_white[15])/3+(m_shuzu_white[6]+m_shuzu_white[11]+m_shuzu_white[17])/3+(m_shuzu_white[2]+m_shuzu_white[7]+m_shuzu_white[11])/3+(m_shuzu_white[23]+m_shuzu_white[28]+m_shuzu_white[32])/3
		          +(m_shuzu_white[3]+m_shuzu_white[9]+m_shuzu_white[14])/3+(m_shuzu_white[11]+m_shuzu_white[17]+m_shuzu_white[22])/3+(m_shuzu_white[7]+m_shuzu_white[11]+m_shuzu_white[16])/3+(m_shuzu_white[28]+m_shuzu_white[32]+m_shuzu_white[37])/3
				  +(m_shuzu_white[9]+m_shuzu_white[14]+m_shuzu_white[20])/3+(m_shuzu_white[17]+m_shuzu_white[22]+m_shuzu_white[28])/3+(m_shuzu_white[3]+m_shuzu_white[8]+m_shuzu_white[12])/3+(m_shuzu_white[32]+m_shuzu_white[37]+m_shuzu_white[41])/3
				  +(m_shuzu_white[14]+m_shuzu_white[20]+m_shuzu_white[25])/3+(m_shuzu_white[22]+m_shuzu_white[28]+m_shuzu_white[33])/3+(m_shuzu_white[8]+m_shuzu_white[12]+m_shuzu_white[17])/3+(m_shuzu_white[37]+m_shuzu_white[41]+m_shuzu_white[46])/3
				  +(m_shuzu_white[2]+m_shuzu_white[8]+m_shuzu_white[13])/3+(m_shuzu_white[28]+m_shuzu_white[33]+m_shuzu_white[39])/3+(m_shuzu_white[12]+m_shuzu_white[17]+m_shuzu_white[21])/3+(m_shuzu_white[15]+m_shuzu_white[20]+m_shuzu_white[24])/3
				  +(m_shuzu_white[8]+m_shuzu_white[13]+m_shuzu_white[19])/3+(m_shuzu_white[33]+m_shuzu_white[39]+m_shuzu_white[44])/3+(m_shuzu_white[17]+m_shuzu_white[21]+m_shuzu_white[26])/3+(m_shuzu_white[20]+m_shuzu_white[24]+m_shuzu_white[29])/3
				  +(m_shuzu_white[13]+m_shuzu_white[19]+m_shuzu_white[24])/3+(m_shuzu_white[39]+m_shuzu_white[44]+m_shuzu_white[50])/3+(m_shuzu_white[4]+m_shuzu_white[9]+m_shuzu_white[13])/3+(m_shuzu_white[24]+m_shuzu_white[29]+m_shuzu_white[33])/3
				  +(m_shuzu_white[19]+m_shuzu_white[24]+m_shuzu_white[30])/3+(m_shuzu_white[16]+m_shuzu_white[21]+m_shuzu_white[27])/3+(m_shuzu_white[9]+m_shuzu_white[13]+m_shuzu_white[18])/3+(m_shuzu_white[29]+m_shuzu_white[33]+m_shuzu_white[38])/3
				  +(m_shuzu_white[24]+m_shuzu_white[30]+m_shuzu_white[35])/3+(m_shuzu_white[21]+m_shuzu_white[27]+m_shuzu_white[32])/3+(m_shuzu_white[13]+m_shuzu_white[18]+m_shuzu_white[22])/3+(m_shuzu_white[33]+m_shuzu_white[38]+m_shuzu_white[42])/3
				  +(m_shuzu_white[1]+m_shuzu_white[7]+m_shuzu_white[12])/3+(m_shuzu_white[27]+m_shuzu_white[32]+m_shuzu_white[38])/3+(m_shuzu_white[18]+m_shuzu_white[22]+m_shuzu_white[27])/3+(m_shuzu_white[38]+m_shuzu_white[42]+m_shuzu_white[47])/3
				  +(m_shuzu_white[7]+m_shuzu_white[12]+m_shuzu_white[18])/3+(m_shuzu_white[32]+m_shuzu_white[38]+m_shuzu_white[43])/3+(m_shuzu_white[22]+m_shuzu_white[27]+m_shuzu_white[31])/3+(m_shuzu_white[25]+m_shuzu_white[30]+m_shuzu_white[34])/3
				  +(m_shuzu_white[12]+m_shuzu_white[18]+m_shuzu_white[23])/3+(m_shuzu_white[38]+m_shuzu_white[43]+m_shuzu_white[49])/3+(m_shuzu_white[27]+m_shuzu_white[31]+m_shuzu_white[36])/3+(m_shuzu_white[30]+m_shuzu_white[34]+m_shuzu_white[39])/3
				  +(m_shuzu_white[18]+m_shuzu_white[23]+m_shuzu_white[29])/3+(m_shuzu_white[26]+m_shuzu_white[31]+m_shuzu_white[37])/3+(m_shuzu_white[5]+m_shuzu_white[10]+m_shuzu_white[14])/3+(m_shuzu_white[34]+m_shuzu_white[39]+m_shuzu_white[43])/3
				  +(m_shuzu_white[23]+m_shuzu_white[29]+m_shuzu_white[34])/3+(m_shuzu_white[31]+m_shuzu_white[37]+m_shuzu_white[42])/3+(m_shuzu_white[10]+m_shuzu_white[14]+m_shuzu_white[19])/3+(m_shuzu_white[39]+m_shuzu_white[43]+m_shuzu_white[48])/3
				  +(m_shuzu_white[29]+m_shuzu_white[34]+m_shuzu_white[40])/3+(m_shuzu_white[37]+m_shuzu_white[42]+m_shuzu_white[48])/3+(m_shuzu_white[14]+m_shuzu_white[19]+m_shuzu_white[23])/3+(m_shuzu_white[35]+m_shuzu_white[40]+m_shuzu_white[44])/3
				  +(m_shuzu_white[34]+m_shuzu_white[40]+m_shuzu_white[45])/3+(m_shuzu_white[36]+m_shuzu_white[41]+m_shuzu_white[47])/3+(m_shuzu_white[19]+m_shuzu_white[23]+m_shuzu_white[28])/3+(m_shuzu_white[40]+m_shuzu_white[44]+m_shuzu_white[49])/3;
	//黑棋
	m_num_lian_black = (m_shuzu_black[4]+m_shuzu_black[10]+m_shuzu_black[15])/3+(m_shuzu_black[6]+m_shuzu_black[11]+m_shuzu_black[17])/3+(m_shuzu_black[2]+m_shuzu_black[7]+m_shuzu_black[11])/3+(m_shuzu_black[23]+m_shuzu_black[28]+m_shuzu_black[32])/3
		          +(m_shuzu_black[3]+m_shuzu_black[9]+m_shuzu_black[14])/3+(m_shuzu_black[11]+m_shuzu_black[17]+m_shuzu_black[22])/3+(m_shuzu_black[7]+m_shuzu_black[11]+m_shuzu_black[16])/3+(m_shuzu_black[28]+m_shuzu_black[32]+m_shuzu_black[37])/3
				  +(m_shuzu_black[9]+m_shuzu_black[14]+m_shuzu_black[20])/3+(m_shuzu_black[17]+m_shuzu_black[22]+m_shuzu_black[28])/3+(m_shuzu_black[3]+m_shuzu_black[8]+m_shuzu_black[12])/3+(m_shuzu_black[32]+m_shuzu_black[37]+m_shuzu_black[41])/3
				  +(m_shuzu_black[14]+m_shuzu_black[20]+m_shuzu_black[25])/3+(m_shuzu_black[22]+m_shuzu_black[28]+m_shuzu_black[33])/3+(m_shuzu_black[8]+m_shuzu_black[12]+m_shuzu_black[17])/3+(m_shuzu_black[37]+m_shuzu_black[41]+m_shuzu_black[46])/3
				  +(m_shuzu_black[2]+m_shuzu_black[8]+m_shuzu_black[13])/3+(m_shuzu_black[28]+m_shuzu_black[33]+m_shuzu_black[39])/3+(m_shuzu_black[12]+m_shuzu_black[17]+m_shuzu_black[21])/3+(m_shuzu_black[15]+m_shuzu_black[20]+m_shuzu_black[24])/3
				  +(m_shuzu_black[8]+m_shuzu_black[13]+m_shuzu_black[19])/3+(m_shuzu_black[33]+m_shuzu_black[39]+m_shuzu_black[44])/3+(m_shuzu_black[17]+m_shuzu_black[21]+m_shuzu_black[26])/3+(m_shuzu_black[20]+m_shuzu_black[24]+m_shuzu_black[29])/3
				  +(m_shuzu_black[13]+m_shuzu_black[19]+m_shuzu_black[24])/3+(m_shuzu_black[39]+m_shuzu_black[44]+m_shuzu_black[50])/3+(m_shuzu_black[4]+m_shuzu_black[9]+m_shuzu_black[13])/3+(m_shuzu_black[24]+m_shuzu_black[29]+m_shuzu_black[33])/3
				  +(m_shuzu_black[19]+m_shuzu_black[24]+m_shuzu_black[30])/3+(m_shuzu_black[16]+m_shuzu_black[21]+m_shuzu_black[27])/3+(m_shuzu_black[9]+m_shuzu_black[13]+m_shuzu_black[18])/3+(m_shuzu_black[29]+m_shuzu_black[33]+m_shuzu_black[38])/3
				  +(m_shuzu_black[24]+m_shuzu_black[30]+m_shuzu_black[35])/3+(m_shuzu_black[21]+m_shuzu_black[27]+m_shuzu_black[32])/3+(m_shuzu_black[13]+m_shuzu_black[18]+m_shuzu_black[22])/3+(m_shuzu_black[33]+m_shuzu_black[38]+m_shuzu_black[42])/3
				  +(m_shuzu_black[1]+m_shuzu_black[7]+m_shuzu_black[12])/3+(m_shuzu_black[27]+m_shuzu_black[32]+m_shuzu_black[38])/3+(m_shuzu_black[18]+m_shuzu_black[22]+m_shuzu_black[27])/3+(m_shuzu_black[38]+m_shuzu_black[42]+m_shuzu_black[47])/3
				  +(m_shuzu_black[7]+m_shuzu_black[12]+m_shuzu_black[18])/3+(m_shuzu_black[32]+m_shuzu_black[38]+m_shuzu_black[43])/3+(m_shuzu_black[22]+m_shuzu_black[27]+m_shuzu_black[31])/3+(m_shuzu_black[25]+m_shuzu_black[30]+m_shuzu_black[34])/3
				  +(m_shuzu_black[12]+m_shuzu_black[18]+m_shuzu_black[23])/3+(m_shuzu_black[38]+m_shuzu_black[43]+m_shuzu_black[49])/3+(m_shuzu_black[27]+m_shuzu_black[31]+m_shuzu_black[36])/3+(m_shuzu_black[30]+m_shuzu_black[34]+m_shuzu_black[39])/3
				  +(m_shuzu_black[18]+m_shuzu_black[23]+m_shuzu_black[29])/3+(m_shuzu_black[26]+m_shuzu_black[31]+m_shuzu_black[37])/3+(m_shuzu_black[5]+m_shuzu_black[10]+m_shuzu_black[14])/3+(m_shuzu_black[34]+m_shuzu_black[39]+m_shuzu_black[43])/3
				  +(m_shuzu_black[23]+m_shuzu_black[29]+m_shuzu_black[34])/3+(m_shuzu_black[31]+m_shuzu_black[37]+m_shuzu_black[42])/3+(m_shuzu_black[10]+m_shuzu_black[14]+m_shuzu_black[19])/3+(m_shuzu_black[39]+m_shuzu_black[43]+m_shuzu_black[48])/3
				  +(m_shuzu_black[29]+m_shuzu_black[34]+m_shuzu_black[40])/3+(m_shuzu_black[37]+m_shuzu_black[42]+m_shuzu_black[48])/3+(m_shuzu_black[14]+m_shuzu_black[19]+m_shuzu_black[23])/3+(m_shuzu_black[35]+m_shuzu_black[40]+m_shuzu_black[44])/3
				  +(m_shuzu_black[34]+m_shuzu_black[40]+m_shuzu_black[45])/3+(m_shuzu_black[36]+m_shuzu_black[41]+m_shuzu_black[47])/3+(m_shuzu_black[19]+m_shuzu_black[23]+m_shuzu_black[28])/3+(m_shuzu_black[40]+m_shuzu_black[44]+m_shuzu_black[49])/3;
	//估值   
	m_value_white += 80*m_num_bianxian_white;//链的差值80
	m_value_black += 80*m_num_bianxian_black;
}
//4.三角形数量
void CEveluation::count_sanjiao_num()
{
	m_num_sanjiao_white = 0;//三角形个数
	m_num_sanjiao_black = 0;
	for(BYTE i=0;i<9;i++)//白棋
	{   //上两个，下一个
		for(BYTE j=0;j<4;j++)
		{
			if(i%2==0)
			{
				m_num_sanjiao_white+=(m_shuzu_white[5*i+1+j]+m_shuzu_white[5*i+2+j]+m_shuzu_white[5*i+7+j])/3;
			}
			else
			{
				m_num_sanjiao_white+=(m_shuzu_white[5*i+1+j]+m_shuzu_white[5*i+2+j]+m_shuzu_white[5*i+6+j])/3;
			}
		}
	}
	for(BYTE i=0;i<9;i++)//上一个，下两个
	{
		for(BYTE j=0;j<5;j++)
		{   //出去不能构成三角形的单独点
			if(0==i&&4==j || 1==i&&0==j || 2==i&&4==j || 3==i&&0==j || 4==i&&4==j
				|| 5==i&&0==j || 6==i&&4==j || 7==i&&0==j || 8==i&&4==j)
			{
				continue;
			}
			if(i%2==0)
			{
				m_num_sanjiao_white+=(m_shuzu_white[5*i+1+j]+m_shuzu_white[5*i+6+j]+m_shuzu_white[5*i+7+j])/3;
			}
			else
			{
				m_num_sanjiao_white+=(m_shuzu_white[5*i+1+j]+m_shuzu_white[5*i+5+j]+m_shuzu_white[5*i+6+j])/3;
			}
		}
	}
	for(BYTE i=0;i<9;i++)//左两个，右一个
	{
		for(BYTE j=0;j<40;j+=10)
		{
			if(i%2==0)
			{
				m_num_sanjiao_white+=(m_shuzu_white[6+i/2+j]+m_shuzu_white[11+i/2+j]+m_shuzu_white[16+i/2+j])/3;
			}
			else
			{
				m_num_sanjiao_white+=(m_shuzu_white[(i+1)/2+j]+m_shuzu_white[(i+1)/2+6+j]+m_shuzu_white[(i+1)/2+10+j])/3;
			}
		}
	}
	for(BYTE i=0;i<9;i++)//左一个，右两个
	{
		for(BYTE j=0;j<50;j+=10)
		{   //出去不能构成三角形的单独点
			if(0==i&&40==j || 1==i&&0==j || 2==i&&40==j || 3==i&&0==j || 4==i&&40==j
				|| 5==i&&0==j || 6==i&&40==j || 7==i&&0==j || 8==i&&40==j)
			{
				continue;
			}
			if(i%2==0)
			{
				m_num_sanjiao_white+=(m_shuzu_white[6+i/2+j-5]+m_shuzu_white[6+i/2+j]+m_shuzu_white[6+i/2+j+5])/3;
			}
			else
			{
				m_num_sanjiao_white+=(m_shuzu_white[(i+1)/2+j]+m_shuzu_white[(i+1)/2-4+j]+m_shuzu_white[(i+1)/2+6+j])/3;
			}
		}
	}
	for(BYTE i=0;i<9;i++)//黑棋  //上两个，下一个
	{
		for(BYTE j=0;j<4;j++)
		{
			if(i%2==0)
			{
				m_num_sanjiao_black+=(m_shuzu_black[5*i+1+j]+m_shuzu_black[5*i+2+j]+m_shuzu_black[5*i+7+j])/3;
			}
			else
			{
				m_num_sanjiao_black+=(m_shuzu_black[5*i+1+j]+m_shuzu_black[5*i+2+j]+m_shuzu_black[5*i+6+j])/3;
			}
		}
	}
	for(BYTE i=0;i<9;i++)//上一个，下两个
	{
		for(BYTE j=0;j<5;j++)
		{   //出去不能构成三角形的单独点
			if(0==i&&4==j || 1==i&&0==j || 2==i&&4==j || 3==i&&0==j || 4==i&&4==j
				|| 5==i&&0==j || 6==i&&4==j || 7==i&&0==j || 8==i&&4==j)
			{
				continue;
			}
			if(i%2==0)
			{
				m_num_sanjiao_black+=(m_shuzu_black[5*i+1+j]+m_shuzu_black[5*i+6+j]+m_shuzu_black[5*i+7+j])/3;
			}
			else
			{
				m_num_sanjiao_black+=(m_shuzu_black[5*i+1+j]+m_shuzu_black[5*i+5+j]+m_shuzu_black[5*i+6+j])/3;
			}
		}
	}
	for(BYTE i=0;i<9;i++)//左两个，右一个
	{
		for(BYTE j=0;j<40;j+=10)
		{
			if(i%2==0)
			{
				m_num_sanjiao_black+=(m_shuzu_black[6+i/2+j]+m_shuzu_black[11+i/2+j]+m_shuzu_black[16+i/2+j])/3;
			}
			else
			{
				m_num_sanjiao_black+=(m_shuzu_black[(i+1)/2+j]+m_shuzu_black[(i+1)/2+6+j]+m_shuzu_black[(i+1)/2+10+j])/3;
			}
		}
	}
	for(BYTE i=0;i<9;i++)//左一个，右两个
	{
		for(BYTE j=0;j<50;j+=10)
		{   //出去不能构成三角形的单独点
			if(0==i&&40==j || 1==i&&0==j || 2==i&&40==j || 3==i&&0==j || 4==i&&40==j
				|| 5==i&&0==j || 6==i&&40==j || 7==i&&0==j || 8==i&&40==j)
			{
				continue;
			}
			if(i%2==0)
			{
				m_num_sanjiao_black+=(m_shuzu_black[6+i/2+j-5]+m_shuzu_black[6+i/2+j]+m_shuzu_black[6+i/2+j+5])/3;
			}
			else
			{
				m_num_sanjiao_black+=(m_shuzu_black[(i+1)/2+j]+m_shuzu_black[(i+1)/2-4+j]+m_shuzu_black[(i+1)/2+6+j])/3;
			}
		}
	}
	//估值   //链的差值80
	m_value_white += 100*m_num_sanjiao_white;
	m_value_black += 100*m_num_sanjiao_black;
}
//5.左中右//343
void CEveluation::count_zuozhongyou()
{
	m_num_left_white = 0;//初始化左中右数据
	m_num_middle_white = 0;
	m_num_right_white = 0;
	m_num_left_black = 0;
	m_num_middle_black = 0;
	m_num_right_black = 0;
	for(BYTE i=0;i<5;i++)//白棋
	{
		m_num_left_white += m_shuzu_white[6+10*i];//左
		m_num_left_white += m_shuzu_white[1+10*i];
		m_num_left_white += m_shuzu_white[7+10*i];
		m_num_middle_white += m_shuzu_white[2+10*i];//中
		m_num_middle_white += m_shuzu_white[8+10*i];
		m_num_middle_white += m_shuzu_white[3+10*i];
		m_num_middle_white += m_shuzu_white[9+10*i];
		m_num_right_white += m_shuzu_white[4+10*i];//右
		m_num_right_white += m_shuzu_white[10+10*i];
		m_num_right_white += m_shuzu_white[5+10*i];
	}
	for(BYTE i=0;i<5;i++)//黑棋
	{
		m_num_left_black += m_shuzu_black[6+10*i];//左
		m_num_left_black += m_shuzu_black[1+10*i];
		m_num_left_black += m_shuzu_black[7+10*i];
		m_num_middle_black += m_shuzu_black[2+10*i];//中
		m_num_middle_black += m_shuzu_black[8+10*i];
		m_num_middle_black += m_shuzu_black[3+10*i];
		m_num_middle_black += m_shuzu_black[9+10*i];
		m_num_right_black += m_shuzu_black[4+10*i];//右
		m_num_right_black += m_shuzu_black[10+10*i];
		m_num_right_black += m_shuzu_black[5+10*i];
	}
	//估值
	if(m_num_left_white - m_num_left_black >= 2 || m_num_left_white - m_num_left_black <= -2)//左中右加分100
	{
		m_value_white += 100*m_num_left_white;
		m_value_black += 100*m_num_left_black;
	}
	if(m_num_middle_white - m_num_middle_black >= 2 || m_num_middle_white - m_num_middle_black <= -2)
	{
		m_value_white += 100*m_num_middle_white;
		m_value_black += 100*m_num_middle_black;
	}
	if(m_num_right_white - m_num_left_black >= 2 || m_num_left_white - m_num_right_black <= -2)
	{
		m_value_white += 100*m_num_right_white;
		m_value_black += 100*m_num_left_black;
	}
}
//6.压制数量
void CEveluation::count_yazhi_num()
{
	m_num_yazhi_white = 0;//初始化数据
	m_num_yazhi_black = 0;
	for(BYTE i=1;i<=50;i++)//白棋压制黑棋
	{  
		if(1 == m_shuzu_white[i])
		{
			switch(i)
			{
	//1.只能控制左下一个空点：
			case 5://满足 10点空位，14，15只有一个黑棋,14位不能有白棋
				if(0==m_shuzu_white[10] && 0==m_shuzu_black[10] && 1==(m_shuzu_black[14]+m_shuzu_black[15]) && 0==m_shuzu_white[14])
				{
					m_num_yazhi_white++;
				}
				break;
			//10也只能控制左下一个，只是坐标不同
			case 10://满足 10点空位，14，15只有一个黑棋
				if(0==m_shuzu_white[14] && 0==m_shuzu_black[14] && 1==(m_shuzu_black[19]+m_shuzu_black[20]) && 0==m_shuzu_white[19])
				{
					m_num_yazhi_white++;
				}
				break;
	//2.只能控制下方两个，其中一个在左边线
			case 1://满足 7点空位，12，11只有一个黑棋
				if(0==m_shuzu_white[7] && 0==m_shuzu_black[7] && 1==(m_shuzu_black[12]+m_shuzu_black[11]) && 0==m_shuzu_white[12])
				{
					m_num_yazhi_white++;
				}
				break;
			//只能控制下方两个
			case 2://满足 左下点空位，左下的左右只有一个
			case 3://满足 右下点空位，右下的左右只有一个
			case 4:
				if(0==m_shuzu_white[i+5] && 0==m_shuzu_black[i+5] && 1==(m_shuzu_black[i+9]+m_shuzu_black[i+10]) && 0==m_shuzu_white[i+9]
					|| 0==m_shuzu_white[i+6] && 0==m_shuzu_black[i+6] && 1==(m_shuzu_black[i+10]+m_shuzu_black[i+11]) && 0==m_shuzu_white[i+11])
				{
					m_num_yazhi_white++;
				}
			//7，8，9也是只能控制下方两个点，只是坐标不同
			case 7:
			case 8:
			case 9:
				if(0==m_shuzu_white[i+4] && 0==m_shuzu_black[i+4] && 1==(m_shuzu_black[i+9]+m_shuzu_black[i+10]) && 0==m_shuzu_white[i+9]
					|| 0==m_shuzu_white[i+5] && 0==m_shuzu_black[i+5] && 1==(m_shuzu_black[i+10]+m_shuzu_black[i+11]) && 0==m_shuzu_white[i+11])
				{
					m_num_yazhi_white++;
				}
				break;
	//3.只能控制右方两个，上面的在边线
			case 6://满足 右下点空位，右下的左右只有一个
				if(0==m_shuzu_white[11] && 0==m_shuzu_black[11] && 1==(m_shuzu_black[16]+m_shuzu_black[17]) && 0==m_shuzu_white[17])
				{
					m_num_yazhi_white++;
				}
				break;
			//只能控制右方两个
			case 16://满足 右上点空位，右右有一个
			case 26://满足 右下点空位，右下的左右只有一个
			case 36:
				if(0==m_shuzu_white[i-5] && 0==m_shuzu_black[i-5] && 1==m_shuzu_black[i+1] && 0==m_shuzu_white[i-9]
					|| 0==m_shuzu_white[i+5] && 0==m_shuzu_black[i+5] && 1==(m_shuzu_black[i+10]+m_shuzu_black[i+11]) && 0==m_shuzu_white[i+11])
				{
					m_num_yazhi_white++;
				}
				break;
			//11，21，31，也是只能控制右方两个，只是坐标不同
			case 11://满足 右上点空位，右右有一个
			case 21://满足 右下点空位，右下的左右只有一个
			case 31:
				if(0==m_shuzu_white[i-4] && 0==m_shuzu_black[i-4] && 1==m_shuzu_black[i+1] && 0==m_shuzu_white[i-9]
					|| 0==m_shuzu_white[i+6] && 0==m_shuzu_black[i+6] && 1==(m_shuzu_black[i+10]+m_shuzu_black[i+11]) && 0==m_shuzu_white[i+11])
				{
					m_num_yazhi_white++;
				}
				break;
	//4.只能控制左方两个，下面的在边线
			case 45://满足 40空位，44有一个
				if(0==m_shuzu_white[40] && 0==m_shuzu_black[40] && 1==m_shuzu_black[44] && 0==m_shuzu_white[34])
				{
					m_num_yazhi_white++;
				}
				break;
			//只能控制左方两个
			case 15://满足 左上点空位，左左有一个
			case 25://满足 左下点空位，左下的左右只有一个
			case 35:
				if(0==m_shuzu_white[i-5] && 0==m_shuzu_black[i-5] && 1==m_shuzu_black[i-1] && 0==m_shuzu_white[i-11]
					|| 0==m_shuzu_white[i+5] && 0==m_shuzu_black[i+5] && 1==(m_shuzu_black[i+9]+m_shuzu_black[i+10]) && 0==m_shuzu_white[i+9])
				{
					m_num_yazhi_white++;
				}
				break;
			//20，30，40也能控制左方两个，坐标不同
			case 20://满足 左上点空位，左左有一个
			case 30://满足 左下点空位，左下的左右只有一个
			case 40:
				if(0==m_shuzu_white[i-6] && 0==m_shuzu_black[i-6] && 1==m_shuzu_black[i-1] && 0==m_shuzu_white[i-11]
					|| 0==m_shuzu_white[i+4] && 0==m_shuzu_black[i+4] && 1==(m_shuzu_black[i+9]+m_shuzu_black[i+10]) && 0==m_shuzu_white[i+9])
				{
					m_num_yazhi_white++;
				}
				break;
	//5.只能控制右上
			case 41:
				if(0==m_shuzu_white[37] && 0==m_shuzu_black[37] && 1==m_shuzu_black[42] && 0==m_shuzu_white[32])
				{
					m_num_yazhi_white++;
				}
				break;
	//6.只能控制上方两个
			case 42:
			case 43:
			case 44:
				if(0==m_shuzu_white[i-5] && 0==m_shuzu_black[i-5] && 1==m_shuzu_black[i-1] && 0==m_shuzu_white[i-11]
				|| 0==m_shuzu_white[i-4] && 0==m_shuzu_black[i-4] && 1==m_shuzu_black[i+1] && 0==m_shuzu_white[i-9])
				{
					m_num_yazhi_white++;
				}
				break;
	//能控制4个方向
			case 12://左上i-5空位，i-1黑棋
			case 13://右上i-4空位，i+1黑棋
			case 14://左下i+5空位，左下的左右有一个
			case 22://右下i+6空位，右下的左右有一个
			case 23:
			case 24:
			case 32:
			case 33:
			case 34:
				if(0==m_shuzu_white[i-5] && 0==m_shuzu_black[i-5] && 1==m_shuzu_black[i-1] && 0==m_shuzu_white[i-11]
				|| 0==m_shuzu_white[i-4] && 0==m_shuzu_black[i-4] && 1==m_shuzu_black[i+1] && 0==m_shuzu_white[i-9]
				|| 0==m_shuzu_white[i+5] && 0==m_shuzu_black[i+5] && 1==(m_shuzu_black[i+9]+m_shuzu_black[i+10]) && 0==m_shuzu_white[i+9]
				|| 0==m_shuzu_white[i+6] && 0==m_shuzu_black[i+6] && 1==(m_shuzu_black[i+10]+m_shuzu_black[i+11]) && 0==m_shuzu_white[i+11])
				{
					m_num_yazhi_white++;
				}
				break;
		    //能控制4个方向，只是坐标不同
			case 17://左上i-6空位，i-1黑棋
			case 18://右上i-5空位，i+1黑棋
			case 19://左下i+4空位，左下的左右有一个
			case 27://右下i+5空位，右下的左右有一个
			case 28:
			case 29:
			case 37:
			case 38:
			case 39:
				if(0==m_shuzu_white[i-6] && 0==m_shuzu_black[i-6] && 1==m_shuzu_black[i-1] && 0==m_shuzu_white[i-11]
				|| 0==m_shuzu_white[i-5] && 0==m_shuzu_black[i-5] && 1==m_shuzu_black[i+1] && 0==m_shuzu_white[i-9]
				|| 0==m_shuzu_white[i+4] && 0==m_shuzu_black[i+4] && 1==(m_shuzu_black[i+9]+m_shuzu_black[i+10]) && 0==m_shuzu_white[i+9]
				|| 0==m_shuzu_white[i+5] && 0==m_shuzu_black[i+5] && 1==(m_shuzu_black[i+10]+m_shuzu_black[i+11]) && 0==m_shuzu_white[i+11])
				{
					m_num_yazhi_white++;
				}
				break;
			}
		}
		//黑棋压制白棋
		if(1 == m_shuzu_black[i])
		{
			switch(i)
			{
	//1.只能控制右上一个空点：
			case 46://满足 41点空位，36，37只有一个白棋
				if(0==m_shuzu_black[41] && 0==m_shuzu_white[41] && 1==(m_shuzu_white[36]+m_shuzu_white[37]) && 0==m_shuzu_black[37])
				{
					m_num_yazhi_black++;
				}
				break;
			//10也只能控制右上一个，只是坐标不同
			case 41://满足 37点空位，31，32只有一个白棋
				if(0==m_shuzu_black[37] && 0==m_shuzu_white[37] && 1==(m_shuzu_white[31]+m_shuzu_white[32]) && 0==m_shuzu_black[32])
				{
					m_num_yazhi_black++;
				}
				break;
	//2.只能控制上方两个，其中一个在右边线
			case 50://满足 44点空位，39，40只有一个黑棋
				if(0==m_shuzu_black[44] && 0==m_shuzu_white[44] && 1==(m_shuzu_white[39]+m_shuzu_white[40]) && 0==m_shuzu_black[39])
				{
					m_num_yazhi_black++;
				}
				break;
			//只能控制上方两个
			case 47://满足 左上点空位，左上的左右只有一个
			case 48://满足 右上点空位，右上的左右只有一个
			case 49:
				if(0==m_shuzu_black[i-6] && 0==m_shuzu_white[i-6] && 1==(m_shuzu_white[i-11]+m_shuzu_white[i-10]) && 0==m_shuzu_black[i-11]
					|| 0==m_shuzu_black[i-5] && 0==m_shuzu_white[i-5] && 1==(m_shuzu_white[i-10]+m_shuzu_white[i-9]) && 0==m_shuzu_black[i-9])
				{
					m_num_yazhi_black++;
				}
			//42，43，44也是只能控制上方两个点，只是坐标不同
			case 42:
			case 43:
			case 44:
				if(0==m_shuzu_black[i-5] && 0==m_shuzu_white[i-5] && 1==(m_shuzu_white[i-11]+m_shuzu_white[i-10]) && 0==m_shuzu_black[i-11]
					|| 0==m_shuzu_black[i-4] && 0==m_shuzu_white[i-4] && 1==(m_shuzu_white[i-10]+m_shuzu_white[i-9]) && 0==m_shuzu_black[i-9])
				{
					m_num_yazhi_black++;
				}
				break;
	//3.只能控制右方两个，上面的在边线
			case 6://满足 右下点空位，右右有一个
				if(0==m_shuzu_black[11] && 0==m_shuzu_white[11] && 1==m_shuzu_white[7] && 0==m_shuzu_black[17])
				{
					m_num_yazhi_black++;
				}
				break;
			//只能控制右方两个
			case 16://满足 右上点空位，右上的左右只有一个
			case 26://满足 右下点空位，右右有一个
			case 36:
				if(0==m_shuzu_black[i+5] && 0==m_shuzu_white[i+5] && 1==m_shuzu_white[i+1] && 0==m_shuzu_black[i+11]
					|| 0==m_shuzu_black[i-5] && 0==m_shuzu_white[i-5] && 1==(m_shuzu_white[i-10]+m_shuzu_white[i-11]) && 0==m_shuzu_black[i-9])
				{
					m_num_yazhi_black++;
				}
				break;
			//11，21，31，也是只能控制右方两个，只是坐标不同
			case 11://满足 右上点空位，右上的左右只有一个
			case 21://满足 右下点空位，右右有一个
			case 31:
				if(0==m_shuzu_black[i+6] && 0==m_shuzu_white[i+6] && 1==m_shuzu_white[i+1] && 0==m_shuzu_black[i+11]
					|| 0==m_shuzu_black[i-4] && 0==m_shuzu_white[i-4] && 1==(m_shuzu_white[i-10]+m_shuzu_white[i-9]) && 0==m_shuzu_black[i-9])
				{
					m_num_yazhi_black++;
				}
				break;
	//4.只能控制左方两个，下面的在边线
			case 45://满足 40空位，34，35有一个
				if(0==m_shuzu_black[40] && 0==m_shuzu_white[40] && 1==(m_shuzu_white[34]+m_shuzu_white[35]) && 0==m_shuzu_black[34])
				{
					m_num_yazhi_black++;
				}
				break;
			//只能控制左方两个
			case 15://满足 左上点空位，左上的左右只有一个
			case 25://满足 左下点空位，左左有一个
			case 35:
				if(0==m_shuzu_black[i+5] && 0==m_shuzu_white[i+5] && 1==m_shuzu_white[i-1] && 0==m_shuzu_black[i+9]
					|| 0==m_shuzu_black[i-5] && 0==m_shuzu_white[i-5] && 1==(m_shuzu_white[i-9]+m_shuzu_white[i-10]) && 0==m_shuzu_black[i-11])
				{
					m_num_yazhi_black++;
				}
				break;
			//20，30，40也能控制左方两个，坐标不同
			case 20:
			case 30:
			case 40:
				if(0==m_shuzu_black[i+4] && 0==m_shuzu_white[i+4] && 1==m_shuzu_white[i-1] && 0==m_shuzu_black[i+9]
					|| 0==m_shuzu_black[i-6] && 0==m_shuzu_white[i-6] && 1==(m_shuzu_white[i-9]+m_shuzu_white[i-10]) && 0==m_shuzu_black[i-11])
				{
					m_num_yazhi_black++;
				}
				break;
	//5.只能控制左下
			case 10:
				if(0==m_shuzu_black[14] && 0==m_shuzu_white[14] && 1==m_shuzu_white[9] && 0==m_shuzu_black[19])
				{
					m_num_yazhi_black++;
				}
				break;
	//6.只能控制下方两个
			case 7:
			case 8:
			case 9:
				if(0==m_shuzu_black[i+4] && 0==m_shuzu_white[i+4] && 1==m_shuzu_white[i-1] && 0==m_shuzu_black[i+9]
				|| 0==m_shuzu_black[i+5] && 0==m_shuzu_white[i+5] && 1==m_shuzu_white[i+1] && 0==m_shuzu_black[i+11])
				{
					m_num_yazhi_black++;
				}
				break;
	//能控制4个方向
			case 12://左上i-5空位，左上的左右有一个
			case 13://右上i-4空位，右上的左右有一个
			case 14://左下i+5空位，i-1白棋
			case 22://右下i+6空位，i+1白棋
			case 23:
			case 24:
			case 32:
			case 33:
			case 34:
				if(0==m_shuzu_black[i+5] && 0==m_shuzu_white[i+5] && 1==m_shuzu_white[i-1] && 0==m_shuzu_black[i+9]
				|| 0==m_shuzu_black[i+6] && 0==m_shuzu_white[i+6] && 1==m_shuzu_white[i+1] && 0==m_shuzu_black[i+11]
				|| 0==m_shuzu_black[i-5] && 0==m_shuzu_white[i-5] && 1==(m_shuzu_white[i-11]+m_shuzu_white[i-10]) && 0==m_shuzu_black[i-11]
				|| 0==m_shuzu_black[i-4] && 0==m_shuzu_white[i-4] && 1==(m_shuzu_white[i-10]+m_shuzu_white[i-9]) && 0==m_shuzu_black[i-9])
				{
					m_num_yazhi_black++;
				}
				break;
		    //能控制4个方向，只是坐标不同
			case 17://左上i-6空位，左上的左右有一个
			case 18://右上i-5空位，右上的左右有一个
			case 19://左下i+4空位，i-1白棋
			case 27://右下i+5空位，i+1白棋
			case 28:
			case 29:
			case 37:
			case 38:
			case 39:
				if(0==m_shuzu_black[i+4] && 0==m_shuzu_white[i+4] && 1==m_shuzu_white[i-1] && 0==m_shuzu_black[i+9]
				|| 0==m_shuzu_black[i+5] && 0==m_shuzu_white[i+5] && 1==m_shuzu_white[i+1] && 0==m_shuzu_black[i+11]
				|| 0==m_shuzu_black[i-6] && 0==m_shuzu_white[i-6] && 1==(m_shuzu_white[i-11]+m_shuzu_white[i-10]) && 0==m_shuzu_black[i-11]
				|| 0==m_shuzu_black[i-5] && 0==m_shuzu_white[i-5] && 1==(m_shuzu_white[i-10]+m_shuzu_white[i-9]) && 0==m_shuzu_black[i-9])
				{
					m_num_yazhi_black++;
				}
				break;
			}
		}
	}
	//估值
	m_value_white += 10*m_num_yazhi_white;//压制加分
	m_value_black += 10*m_num_yazhi_black;
}
//7.安全值
void CEveluation::AnQuanZhi()
{
	//int n=0;
	//int score[20]={0};
	//for(BYTE i=0;i<10;i++)
	//{
	//	for(BYTE j=0;j<10;j++)
	//	{
	//		if(-1*m_type==m_position[i][j]||-2*m_type==m_position[i][j])//是白棋
	//		{
	//			//考虑棋子是否被孤立
	//			if(0==ZuoXia(i,j,m_position))
	//			{
	//				//安全值
	//				if(9==i||0==j||m_position[i+1][j-1]==-1*m_type||m_position[i+1][j-1]==-2*m_type)//到底或左下是自己的棋子
	//				{
	//					score[n]+=5;
	//				}
	//			}
	//			if(0==ZuoShang (i,j,m_position))
	//			{
	//				if(0==i||0==j||m_position[i-1][j-1]==-1*m_type||m_position[i-1][j-1]==-2*m_type)
	//				{
	//					score[n]+=5;
	//				}
	//			}
	//			if(0==YouXia (i,j,m_position))
	//			{
	//				if(9==i||9==j||m_position[i+1][j+1]==-1*m_type||m_position[i+1][j+1]==-2*m_type)
	//				{
	//					score[n]+=5;
	//				}
	//			}
	//			if(0==YouShang (i,j,m_position))
	//			{
	//				if(0==i||9==j||m_position[i-1][j+1]==-1*m_type||m_position[i-1][j+1]==-2*m_type)
	//				{
	//					score[n]+=5;
	//				}
	//			}
	//			n++;
	//			if(n>=20)
	//			{
	//				break;
	//			}
	//		}
	//	}
	//}
	////估值
	//if(-1 == m_type)//判断黑白
	//{
	//	for(BYTE i=0;i<20;i++)
	//	{
	//		m_value_white += score[i];
	//	}
	//}
	//else
	//{
	//	for(BYTE i=0;i<20;i++)
	//	{
	//		m_value_black += score[i];
	//	}
	//}
}
//8.孔
void CEveluation::Kong()
{
	BYTE i,j;
	for(i=0;i<10;i++)
	{
		for(j=0;j<10;j++)
		{
			if(1 == m_nntype)
			{
				if(i<=7&&j>0&&j<9)
				{
					if(((-1==m_position[i][j]||-2==m_position[i][j])&&(-1==m_position[i+1][j-1]||-2==m_position[i+1][j-1])&&(-1==m_position[i+2][j]||-2==m_position[i+2][j])&&(0==m_position[i+1][j+1]))
						||((-1==m_position[i][j]||-2==m_position[i][j])&&(-1==m_position[i+1][j+1]||-2==m_position[i+1][j+1])&&(-1==m_position[i+2][j]||-2==m_position[i+2][j])&&(0==m_position[i+1][j-1]))
						||((-1==m_position[i][j]||-2==m_position[i][j])&&(-1==m_position[i+1][j-1]||-2==m_position[i+1][j-1])&&(-1==m_position[i+1][j+1]||-2==m_position[i+1][j+1])&&(0==m_position[i+2][j]))
						||((0==m_position[i][j])&&(-1==m_position[i+1][j-1]||-2==m_position[i+1][j-1])&&(-1==m_position[i+1][j+1]||-2==m_position[i+1][j+1])&&(-1==m_position[i+2][j]||-2==m_position[i+2][j])))
					{
						m_value_black -= 100;
					}
				}
			}
			if(-1 == m_nntype)
			{
				if(i>=2&&j>0&&j<9)
				{
					if(((1==m_position[i][j]||2==m_position[i][j])&&(1==m_position[i-1][j-1]||2==m_position[i-1][j-1])&&(1==m_position[i-2][j]||2==m_position[i-2][j])&&(0==m_position[i-1][j+1]))
						||((1==m_position[i][j]||2==m_position[i][j])&&(1==m_position[i-1][j-1]||2==m_position[i-1][j-1])&&(1==m_position[i-1][j+1]||2==m_position[i-1][j+1])&&(0==m_position[i-2][j]))
						||((1==m_position[i][j]||2==m_position[i][j])&&(1==m_position[i-1][j+1]||2==m_position[i-1][j+1])&&(1==m_position[i-2][j]||2==m_position[i-2][j])||(0==m_position[i-1][j-1]))
						||((1==m_position[i-1][j-1]||2==m_position[i-1][j-1])&&(1==m_position[i-1][j+1]||2==m_position[i-1][j+1])&&(1==m_position[i-2][j]||2==m_position[i-2][j])&&(0==m_position[i][j])))
					{
						m_value_white -= 100;
					}	
				}
			}
		}
	}
}
//9.走两颗棋中间——————加400
void CEveluation::ZhongJian()
{
	BYTE i,j;
	for(i=0;i<10;i++)
	{
		for(j=0;j<10;j++)
		{
			if(i>0 && i<9 && j>0 && j<9)
			{
				if(1 == m_nntype)
				{
					if((1==m_position[i-1][j-1]||2==m_position[i-1][j-1])&&(-1==m_position[i][j]||-2==m_position[i][j])&&(1==m_position[i+1][j+1]||2==m_position[i+1][j+1]))
					{   //估值   加分
						m_value_black += 40;
					}
					if((1==m_position[i-1][j+1]||2==m_position[i-1][j+1])&&(-1==m_position[i][j]||-2==m_position[i][j])&&(1==m_position[i+1][j-1]||2==m_position[i+1][j-1]))
					{
						m_value_black += 40;
					}
				}
				if(-1 == m_nntype)
				{
					if((-1==m_position[i-1][j-1]||-2==m_position[i-1][j-1])&&(1==m_position[i][j]||2==m_position[i][j])&&(-1==m_position[i+1][j+1]||-2==m_position[i+1][j+1]))
					{
						m_value_white += 40;
					}
					if((-1==m_position[i-1][j+1]||-2==m_position[i-1][j+1])&&(1==m_position[i][j]||2==m_position[i][j])&&(-1==m_position[i+1][j-1]||-2==m_position[i+1][j-1]))
					{
						m_value_white += 40;
					}
				}
		    }
	   }
    }
}
//10.防止成王
void CEveluation::FangZhiChengWang()
{
	BYTE j;
	if(1 == m_nntype)
	{
		if(0 == m_position[0][3])
		{
			m_value_black -= 80;
		}
		if(0 == m_position[0][7])
		{
			m_value_black -= 80;
		}
		for(j=0;j<9;j++)
		{
			if(0 == m_position[0][j] && ((-1 == m_position[1][j-1] && 1 == m_position[1][j+1]) || (1 == m_position[1][j-1] && -1 == m_position[1][j+1])))
			{
				m_value_black -= 300;
			}
		}
	}
	if(-1 == m_nntype)
	{
		if(0 == m_position[9][2])
		{
			m_value_white -= 80;
		}
		if(0 == m_position[9][6])
		{
			m_value_white -= 80;
		}
		for(j=1;j<10;j++)
		{
			if(0==m_position[9][j] && ((-1==m_position[8][j-1] && 1==m_position[8][j+1])||(1==m_position[8][j-1] && -1==m_position[8][j+1])))
			{
				m_value_white -= 300;
			}
		}	
	}	
}
//11.王压制
void CEveluation::WY()
{
	BYTE i,j,k,m;
	BYTE move_count[20]={0};
	BYTE ys_KongGe,zs_KongGe,zx_KongGe,yx_KongGe;
	BYTE KongGe1 = 0,KongGe2 = 0;
	BYTE p = -1,max = 0;
	for(i=0;i<10;i++)
	{
		for(j=0;j<10;j++)
		{
			if(1 == m_nntype)
			{
				if(-2 == m_position[i][j])
				{
					p++;
					ys_KongGe = YouShang(i,j,m_position);
					zs_KongGe = ZuoShang(i,j,m_position);
					yx_KongGe =YouXia(i,j,m_position);
					zx_KongGe = ZuoXia(i,j,m_position);
					if(ys_KongGe>0)
					{
						for(m=1;m<ys_KongGe;m++)
						{
							if(1==m_position[i-m+1][j+m+1] || 2==m_position[i-m+1][j+m+1])
							{
								move_count[p]++;
							}
						}
					}
					if(zs_KongGe>0)
					{
						for(m=1;m<zs_KongGe;m++)
						{
							if(1==m_position[i-m+1][j-m-1]||2==m_position[i-m+1][j-m-1])
							{
								move_count[p]++;
							}
						}
					}
					if(yx_KongGe>0)
					{
						for(m=1;m<yx_KongGe;m++)
						{
							if(1==m_position[i+m+1][j+m-1]||2==m_position[i+m+1][j+m-1])
							{
								move_count[p]++;
							}
						}
					}
					if(zx_KongGe>0)
					{
						for(m=1;m<zx_KongGe;m++)
						{
							if(1==m_position[i+m+1][j-m+1]||2==m_position[i+m+1][j-m+1])
							{
								move_count[p]++;
							}
						}
					}
				}
			}
			if(-1 == m_nntype)
			{
				if(2 == m_position[i][j])
				{
					p++;
					ys_KongGe =YouShang(i,j,m_position);
					zs_KongGe=ZuoShang(i,j,m_position);
					yx_KongGe=YouXia(i,j,m_position);
					zx_KongGe=ZuoXia(i,j,m_position);
					if(ys_KongGe>0)
					{
						for(m=1;m<ys_KongGe;m++)
						{
							if(-1==m_position[i-m-1][j+m-1]||-2==m_position[i-m-1][j+m-1])
							{
								move_count[p]++;
							}
						}
					}
					if(zs_KongGe>0)
					{
						for(m=1;m<zs_KongGe;m++)
						{
							if(-1==m_position[i-m-1][j-m+1]||-2==m_position[i-m-1][j-m+1])
							{
								move_count[p]++;
							}
						}
					}
					if(yx_KongGe>0)
					{
						for(m=1;m<yx_KongGe;m++)
						{
							if(-1==m_position[i+m-1][j+m+1]||-2==m_position[i+m-1][j+m+1])
							{
								move_count[p]++;
							}
						}
					}
					if(zx_KongGe>0)
					{
						for(m=1;m<zx_KongGe;m++)
						{
							if(-1==m_position[i+m-1][j-m-1]||-2==m_position[i+m-1][j-m-1])
							{
								move_count[p]++;
							}
						}
					}
				}
			}
		}
	}
	max = move_count[0];
	for(k=1;k<p;k++)
	{
		if(move_count[p]>max)
		{
			max = move_count[p];
		}
	}
	if(1 == m_nntype)
	{
		switch(max)
		{
		case 1:
			m_value_black += 2000;
			break;
		case 2:
			m_value_black += 4000;
			break;
		case 3:
			m_value_black += 6000;
			break;
		case 4:
			m_value_black += 8000;
			break;
		case 5:
			m_value_black += 10000;
			break;
		case 6:
			m_value_black += 12000;
			break;
		case 7:
			m_value_black += 14000;
			break;
		case 8:
			m_value_black += 16000;
			break;
		case 9:
			m_value_black += 18000;
			break;
		case 10:
			m_value_black += 20000;
			break;
		case 11:
			m_value_black += 22000;
			break;
		case 12:
			m_value_black += 24000;
			break;
		case 13:
			m_value_black += 26000;
			break;
		case 14:
			m_value_black += 28000;
			break;
		}
	}
	if(-1 == m_nntype)
	{
		switch(max)
		{
		case 1:
			m_value_white += 2000;
			break;
		case 2:
			m_value_white += 4000;
			break;
		case 3:
			m_value_white += 6000;
			break;
		case 4:
			m_value_white += 8000;
			break;
		case 5:
			m_value_white += 10000;
			break;
		case 6:
			m_value_white += 12000;
			break;
		case 7:
			m_value_white += 14000;
			break;
		case 8:
			m_value_white += 16000;
			break;
		case 9:
			m_value_white += 18000;
			break;
		case 10:
			m_value_white += 20000;
			break;
		case 11:
			m_value_white += 22000;
			break;
		case 12:
			m_value_white += 24000;
			break;
		case 13:
			m_value_white += 26000;
			break;
		case 14:
			m_value_white += 28000;
			break;
		}
	}
}

void CEveluation::shuzuTwo_shuzuOne(BYTE i,BYTE j,BYTE shuzu[51])
{
	switch(i)
	{
	case 0:
		shuzu[(j+1)/2]=1;
		break;
	case 1:
		shuzu[6+j/2]=1;
		break;
	case 2:
		shuzu[10+(j+1)/2]=1;
		break;
	case 3:
		shuzu[16+j/2]=1;
		break;
	case 4:
		shuzu[20+(j+1)/2]=1;
		break;
	case 5:
		shuzu[26+j/2]=1;
		break;
	case 6:
		shuzu[30+(j+1)/2]=1;
		break;
	case 7:
		shuzu[36+j/2]=1;
		break;
	case 8:
		shuzu[40+(j+1)/2]=1;
		break;
	case 9:
		shuzu[46+j/2]=1;
		break;
	}
}
