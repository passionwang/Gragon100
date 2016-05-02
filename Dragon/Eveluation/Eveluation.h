#pragma once

#ifndef __INCLUDE_EVELUATION_H__
#define __INCLUDE_EVELUATION_H__
#include "PublicToCountSpace.h"
class CEveluation : public CPublicToCountSpace
{
public:
	CEveluation();
	~CEveluation();
	int Eveluate(int position[10][10], int TURE,int nCeng);
public:
	//1.计数，放置位置
	void count_qi_num();
	//2.边线个数
	void count_bianxian_num();
	//3.链数量
	void count_lian_num();
	//4.三角形数量
	void count_sanjiao_num();
	//5.左中右
	void count_zuozhongyou();
	//6.压制数量
	void count_yazhi_num();
	//7.安全值
	void AnQuanZhi();
	//8孔
	void Kong();
	//9.中间
	void ZhongJian();
	//10.防止成王
	void FangZhiChengWang();
	//11.王压制
	void WY();
	void shuzuTwo_shuzuOne(BYTE i,BYTE j,BYTE shuzu[51]);
public:
	//黑白棋各自数组
	BYTE m_shuzu_white[51];
	BYTE m_shuzu_black[51];
	//棋盘
	int m_position[10][10];
	//黑白
	int m_nntype;
	//双方分值
	int m_value_white;
	int m_value_black;
	//棋子个数
	BYTE m_num_white;
	BYTE m_num_black;
	BYTE m_num_king_white;
	BYTE m_num_king_black;
	//每行数量
	BYTE m_num_hang_white[10];
	BYTE m_num_hang_black[10];
	//每列数量
	BYTE m_num_lie_white[10];
	BYTE m_num_lie_black[10];
	//边线个数
	BYTE m_num_bianxian_white;
	BYTE m_num_bianxian_black;
	//链的个数
	BYTE m_num_lian_white;
	BYTE m_num_lian_black;
	//三角形个数
	BYTE m_num_sanjiao_white;
	BYTE m_num_sanjiao_black;
	//左中右
	BYTE m_num_left_white;
	BYTE m_num_middle_white;
	BYTE m_num_right_white;
	BYTE m_num_left_black;
	BYTE m_num_middle_black;
	BYTE m_num_right_black;
	//压制
	BYTE m_num_yazhi_white;
	BYTE m_num_yazhi_black;
};

#endif //__INCLUDE_EVELUATION_H__