
// DragonDlg.h : 头文件
//

#pragma once
#include "AlphaBetaEngine.h"
#include "NegamaxEngine.h"
#include "FAlphaBetaEngine.h"
#include "AspirationSearch.h"
#include "PVS_Engine.h"
#include "IDAlphabeta.h"
#include "AlphaBetaAndTT.h"
#include "Alphabeta_HH.h"
#include "NegaScout_TT_HH.h"
#include "afxwin.h"

// CDragonDlg 对话框
class CDragonDlg : public CDialogEx
{
// 构造
public:
	CDragonDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DRAGON_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bGameOver;
	BOOL m_bGo;
	int m_BackupChessBoard[10][10];
//	MOVECHESS m_MoveChess;
	POINT m_ptMoveChess;
	CBitmap m_BoardBmp;
	CImageList m_Chessman;
	int m_nBoardWidth; 
	int m_nBoardHeight;
	CSearchEngine *m_pSE;
	
	
//	CMoveGenerator m_oMG;
	
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void iMove();
	//鼠标控制黑棋行进
	void Move(int iX,int iY);   
	CEdit m_Output;
};
