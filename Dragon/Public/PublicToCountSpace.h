#pragma once 

#ifndef __INCLUDE_PUBLICTOCOUNTSPACE_H__
#define __INCLUDE_PUBLICTOCOUNTSPACE_H__

class CPublicToCountSpace
{
public:
	CPublicToCountSpace();
	~CPublicToCountSpace();
	BYTE ZuoShang(BYTE x,BYTE y,int Board[10][10]);
	BYTE YouShang(BYTE x,BYTE y,int Board[10][10]);
	BYTE ZuoXia(BYTE x,BYTE y,int Board[10][10]);
	BYTE YouXia(BYTE x,BYTE y,int Board[10][10]);
};




#endif //__INCLUDE_PUBLICTOCOUNTSPACE_H__