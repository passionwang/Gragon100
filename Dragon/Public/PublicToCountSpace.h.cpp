#include "stdafx.h"
#include "PublicToCountSpace.h"

CPublicToCountSpace::CPublicToCountSpace()
{

}
CPublicToCountSpace::~CPublicToCountSpace()
{

}

//找出四个方向的空格数
BYTE CPublicToCountSpace::ZuoShang(BYTE x,BYTE y,int Board[10][10])
{
	BYTE i=0;
	if(0>=x||0>=y||x>=10||y>=10)
	{
		return 0;
	}
	while(1)
	{
		i++;
		if(0!=Board[x-i][y-i])
		{
			return i-1;
		}
		if(0==x-i||0==y-i)
		{
			return i;
		}
	}
}
BYTE CPublicToCountSpace::YouShang(BYTE x,BYTE y,int Board[10][10])
{
	BYTE i=0;
	if(0>=x||9<=y||x>=10||y>=10)
	{
		return 0;
	}
	while(1)
	{
		i++;
		if(0!=Board[x-i][y+i])
		{
			return i-1;
		}
		if(0==x-i||9==y+i)
		{
			return i;
		}
	}
}
BYTE CPublicToCountSpace::ZuoXia(BYTE x,BYTE y,int Board[10][10])
{
	BYTE i=0;
	if(9<=x||0>=y||x>=10||y>=10)
	{
		return 0;
	}
	while(1)
	{
		i++;
		if(0!=Board[x+i][y-i])
		{
			return i-1;
		}
		if(9==x+i||0==y-i)
		{
			return i;
		}
	}
}
BYTE CPublicToCountSpace::YouXia(BYTE x,BYTE y,int Board[10][10])
{
	BYTE i=0;
	if(9<=x||9<=y||x>=10||y>=10)
	{
		return 0;
	}
	while(1)
	{
		i++;
		if(0!=Board[x+i][y+i])
		{
			return i-1;
		}
		if(9==x+i||9==y+i)
		{
			return i;
		}
	}
}