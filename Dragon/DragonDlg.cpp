
// DragonDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Dragon.h"
#include "DragonDlg.h"
#include "afxdialogex.h"

#define BORDERWIDTH 50 //棋盘(左右)边缘的宽度
#define BORDERHEIGHT 50 //棋盘(上下)边缘的高度
#define GRILLEWIDTH 50  //棋盘上每个格子的高度
#define GRILLEHEIGHT 50 //棋盘上每个格子的宽度
#define NOCHESS   0     //无棋

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
const int iBL=50;//棋盘左侧边距
const int iBT=50;//棋盘上侧边距
const int iBS=50;//棋盘棋位单位
CMoveGenerator MG;
int m_ChessBoard[10][10];
const int InitChessBoard[10][10] =
{
	
	{ 0,-1, 0,-1, 0,-1, 0,-1, 0,-1},
	{-1, 0,-1, 0,-1, 0,-1, 0,-1, 0},
	{ 0,-1, 0,-1, 0,-1, 0,-1, 0,-1},
	{-1, 0,-1, 0,-1, 0,-1, 0,-1, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
	{ 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
	{ 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
	{ 1, 0, 1, 0, 1, 0, 1, 0, 1, 0}

};
void BecomeKing(int CurPosition[10][10]);
void MakeMoveSure(int m,int n,int nType,int CurPosition[10][10]);
void UnMakeMove(int CurPosition[10][10]);
void MakeMoveQi(CHESSMOVE* move,int CurPosition[10][10]);
BOOL IsGameOver(int nType)
{
	if(0 == MG.CreatePossibleMove(m_ChessBoard,1,1) && -1 == nType) return TRUE;
	if (0 == MG.CreatePossibleMove(m_ChessBoard,1,-1) && 1 == nType) return TRUE;
	return FALSE;
}
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDragonDlg 对话框



CDragonDlg::CDragonDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDragonDlg::IDD, pParent)
	, m_bGo(BLACK_GO)
{
	for(int i=0;i<10;i++)
	{
		for(int j=0;j<10;j++)
		{
			m_ChessBoard[i][j] = InitChessBoard[i][j];
		}
	}
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDragonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_Output);
}

BEGIN_MESSAGE_MAP(CDragonDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CDragonDlg 消息处理程序
CMoveGenerator MoveGenerator ;
BOOL CDragonDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_Chessman.Create(IDB_CHESS, 50, 50, RGB(80,80,80)); 

	BITMAP BitMap;
	m_BoardBmp.LoadBitmap(IDB_BOARD);
	m_BoardBmp.GetBitmap(&BitMap);
	m_nBoardWidth =  BitMap.bmWidth; 
	m_nBoardHeight = BitMap.bmHeight;
	m_BoardBmp.DeleteObject();


	memcpy(m_ChessBoard, InitChessBoard, sizeof(InitChessBoard));//初始化棋盘
	
	CMoveGenerator *pMG;
	CEveluation *pEvel;

	pEvel = new CEveluation;
	//此处修改搜索引擎
//	m_pSE = new CNegamaxEngine;//负极大值-最基本
//	m_pSE = new CAlphaBetaEngine;//alpha-Beta剪枝
//	m_pSE = new CFAlphaBetaEngine;//Fail-soft alpha-beta
//	m_pSE = new CAspirationSearch;//渴望搜索
//	m_pSE = new CPVS_Engine;//极小窗口搜索
//	m_pSE = new CIDAlphabeta;//迭代深化
//	m_pSE = new CAlphaBetaAndTT;//alpha-Beta剪枝+TT置换表
//	m_pSE = new CAlphabeta_HH;//alpha-Beta剪枝+HH历史启发
	m_pSE = new CNegaScout_TT_HH;//负极大值+TT置换表+HH历史启发
	pMG = &MoveGenerator;
	
	//此处设置搜索引擎深度
	m_pSE->SetSearchDepth(6);
	//设置走法生成器-只有一个
	m_pSE->SetMoveGenerator(pMG);
	//设置估值-只有一个
	m_pSE->SetEveluator(pEvel);
//	m_MoveChess.nChessID = NOCHESS;
	m_bGameOver = FALSE;//this code does not contents in books.
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDragonDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}
void DrawGo(CDC* pDC)
{
	CBrush jBrush;
	jBrush.CreateSolidBrush(RGB(0,255,0));  //绿
	pDC->SelectObject(&jBrush);
	pDC->Rectangle(iBL+11*iBS,iBT+iBS,iBL+12*iBS,iBT+8*iBS);
	pDC->SelectStockObject(BLACK_BRUSH);
	jBrush.DeleteObject();
	jBrush.CreateSolidBrush(RGB(0,0,255));  //绿
	pDC->SelectObject(&jBrush);
	pDC->Rectangle(iBL+12*iBS,iBT+iBS,iBL+13*iBS,iBT+8*iBS);
	pDC->SelectStockObject(BLACK_BRUSH);
	jBrush.DeleteObject();
}
// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDragonDlg::OnPaint()
{
	CPaintDC dc(this);
	CDC MemDC;
	int i, j;
	POINT pt;
	CBitmap *pOldBmp;

	DrawGo(&dc);

	MemDC.CreateCompatibleDC(&dc);
	m_BoardBmp.LoadBitmap(IDB_BOARD);
	pOldBmp = MemDC.SelectObject(&m_BoardBmp);
	for (i = 0; i < 10; i++)
		for (j = 0; j < 10; j++)
		{
			if (m_ChessBoard[i][j] == NOCHESS)
				continue;
			pt.x = j*GRILLEHEIGHT ;
			pt.y = i*GRILLEWIDTH ;
			switch(m_ChessBoard[i][j])
			{
			case WHITE:
				m_Chessman.Draw(&MemDC, 0, pt, ILD_TRANSPARENT);
				break;
			case BLACK: 
				m_Chessman.Draw(&MemDC, 1, pt, ILD_TRANSPARENT);
				break;
			case -2: 
				m_Chessman.Draw(&MemDC, 2, pt, ILD_TRANSPARENT);
				break;
			case 2: 
				m_Chessman.Draw(&MemDC, 3, pt, ILD_TRANSPARENT);	
				break;
			case 4: 
				m_Chessman.Draw(&MemDC, 4, pt, ILD_TRANSPARENT);	
				break;
			case 6: 
				m_Chessman.Draw(&MemDC, 5, pt, ILD_TRANSPARENT);
				break;
			case -4: 
				m_Chessman.Draw(&MemDC, 6, pt, ILD_TRANSPARENT);	
				break;
			case -6: 
				m_Chessman.Draw(&MemDC, 7, pt, ILD_TRANSPARENT);
				break;
			case -5: 
				m_Chessman.Draw(&MemDC, 8, pt, ILD_TRANSPARENT);
				break;
			case 5: 
				m_Chessman.Draw(&MemDC, 9, pt, ILD_TRANSPARENT);
				break;
			}
		}
		dc.BitBlt(BORDERWIDTH, BORDERHEIGHT, m_nBoardWidth, m_nBoardHeight, &MemDC, 0, 0, SRCCOPY);
		MemDC.SelectObject(&pOldBmp);
		MemDC.DeleteDC();
		m_BoardBmp.DeleteObject();
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDragonDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// CMyChessView message handlers
int iInArea(int x,int y,int x1,int y1,int x2,int y2)
{
	if(x>=x1 && x<=x2 && y>=y1 && y<=y2)
		return 1;
	else
		return 0;
}
extern int G_nCountEv;//记录估值结点数;
extern int G_nCountTT;
extern int G_nCountTTHH;
void CDragonDlg::iMove()          
{
	if (TRUE == m_bGameOver || WRITE_GO != m_bGo)
		return;
	m_bGo = BLACK_GO;
	int timecount;
	timecount = GetTickCount();
	m_pSE->SearchAGoodMove(m_ChessBoard);	
	BecomeKing(m_ChessBoard);//4.成王判断
	CString sNodeCount;
	sNodeCount.Format(L" Cost %d ms. %d Nodes were eveluated. %d TT Node Be Found.%d TTHH Node Be Found.", GetTickCount() - timecount,G_nCountEv,G_nCountTT,G_nCountTTHH);
	m_Output.SetWindowText(sNodeCount);
	G_nCountEv = 0;
	G_nCountTT = 0;
	G_nCountTTHH = 0;
	InvalidateRect(NULL, FALSE);
	UpdateWindow();
	
	if (IsGameOver(-1))
	{
		m_bGameOver = TRUE;
		MessageBox(L"Game Over! Press New Game to replay...");
	}
}
int m_iM;
int m_iN;
int m,n;
CHESSPOS m_MoveTPos[100];
CMoveGenerator oMG ;
int nPosMove;
//鼠标控制黑棋行进
void CDragonDlg::Move(int iX,int iY)     
{
	int nCountMove;
	
	if (TRUE == m_bGameOver || BLACK_GO != m_bGo)
		return;
//	memcpy(m_BackupChessBoard, m_ChessBoard, sizeof(m_ChessBoard));
	m_iM = (iY-iBT)/iBS;  //将座标转换成数组下标
	m_iN = (iX-iBL)/iBS;
	switch(m_ChessBoard[m_iM][m_iN])
	{
	case BLACK://找到当前被点击的点的走法，
	case 2*BLACK:
		m = m_iM;
		n = m_iN;
		UnMakeMove(m_ChessBoard);
		nCountMove = oMG.CreatePossibleMove(m_ChessBoard,0,1);
		nPosMove = 0;
		for(int i=0;i<nCountMove;i++)
		{
			if(m_iM == oMG.m_nMoveList[0][i].From.y && m_iN == oMG.m_nMoveList[0][i].From.x)
			{
				oMG.m_nMoveList[0][nPosMove] =  oMG.m_nMoveList[0][i];
				nPosMove++;
			}
		}//并显示出来
		for(int i=0;i<nPosMove;i++)
		{   
			//1.显示
			MakeMoveQi(&oMG.m_nMoveList[0][i],m_ChessBoard);
			//2.记录最后点坐标
			m_MoveTPos[i] = oMG.m_nMoveList[0][i].From;
		}
		break;
	case 5:
		nPosMove = 0;
		UnMakeMove(m_ChessBoard);
		nCountMove = oMG.CreatePossibleMove(m_ChessBoard,0,1);
		for(int i=0;i<nCountMove;i++)
		{
			if(m == oMG.m_nMoveList[0][i].From.y && n == oMG.m_nMoveList[0][i].From.x)
			{
				oMG.m_nMoveList[0][nPosMove] =  oMG.m_nMoveList[0][i];
				nPosMove++;
			}
		}
		for(int i=0;i<nPosMove;i++)
		{   
			//1.核对坐标
			if(m_iM == m_MoveTPos[i].y && m_iN == m_MoveTPos[i].x)
			{
				//2.按照找到坐标行棋
				MakeMoveQi(&oMG.m_nMoveList[0][i],m_ChessBoard);
				//3.按照走的就真走了
				MakeMoveSure(m,n,oMG.m_nMoveList[0][i].m_Type,m_ChessBoard);
				//4.成王判断
				BecomeKing(m_ChessBoard);
				m_bGo = WRITE_GO;
			}
		}
		InvalidateRect(NULL,FALSE);
		UpdateWindow();
		if (IsGameOver(1))
		{
			m_bGameOver = TRUE;
			MessageBox(L"Game Over! Press New Game to replay...");
		}
		break;
	}
}
void CDragonDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnLButtonDown(nFlags, point);
	int iX,iY;
	iX= point.x;
	iY= point.y;
	if(iInArea(iX,iY,iBL+11*iBS,iBT+iBS,iBL+12*iBS,iBT+8*iBS)) //如果机器行棋（行棋范围）
	{
		iMove();
//		UP_COUNT=1;
	}
	if(iInArea(iX,iY,iBL,iBT,iBL+iBS*10,iBT+iBS*10)) //棋盘范围
	{
		Move(iX,iY);
//		UP_COUNT=1;
	}
	if(iInArea(iX,iY,iBL+12*iBS,iBT+iBS,iBL+13*iBS,iBT+8*iBS)) //棋盘范围
	{
//		INIT();
	}
	InvalidateRect(NULL,FALSE);
	UpdateWindow();
}
void MakeMoveQi(CHESSMOVE* move,int CurPosition[10][10])
{
	if(1 == move->m_Type)
	{
		for(int i=0;i<move->m_Count;i++)
		{
			switch(move->step[i])
			{
			case Left_Down:
				CurPosition[move->From.y+2][move->From.x-2] = -5;
				if(-2 == CurPosition[move->From.y+1][move->From.x-1])
				{
					CurPosition[move->From.y+1][move->From.x-1] = -4;
				}
				else
				{
					CurPosition[move->From.y+1][move->From.x-1] = 4;
				}
				move->From.y += 2;
				move->From.x -= 2;
				break;
			case Right_Down:
				CurPosition[move->From.y+2][move->From.x+2] = -5;
				if(-2 == CurPosition[move->From.y+1][move->From.x+1])
				{
					CurPosition[move->From.y+1][move->From.x+1] = -4;
				}
				else
				{
					CurPosition[move->From.y+1][move->From.x+1] = 4;
				}
				move->From.y += 2;
				move->From.x += 2;
				break;
			case Left_Up:
				CurPosition[move->From.y-2][move->From.x-2] = -5;
				if(-2 == CurPosition[move->From.y-1][move->From.x-1])
				{
					CurPosition[move->From.y-1][move->From.x-1] = -4;
				}
				else
				{
					CurPosition[move->From.y-1][move->From.x-1] = 4;
				}
				move->From.y -= 2;
				move->From.x -= 2;
				break;
			case Right_Up:
				CurPosition[move->From.y-2][move->From.x+2] = -5;
				if(-2 == CurPosition[move->From.y-1][move->From.x+1])
				{
					CurPosition[move->From.y-1][move->From.x+1] = -4;
				}
				else
				{
					CurPosition[move->From.y-1][move->From.x+1] = 4;
				}
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
				CurPosition[move->From.y+1][move->From.x-1] = 5; 
				move->From.y += 1;
				move->From.x -= 1;
				break;
			case Right_Down:
				CurPosition[move->From.y+1][move->From.x+1] = 5; 
				move->From.y += 1;
				move->From.x += 1;
				break;
			case Left_Up:
				CurPosition[move->From.y-1][move->From.x-1] = 5; 
				move->From.y -= 1;
				move->From.x -= 1;
				break;
			case Right_Up:
				CurPosition[move->From.y-1][move->From.x+1] = 5; 
				move->From.y -= 1;
				move->From.x += 1;
				break;
			}
		}
		else
		{
			CurPosition[move->From.y][move->From.x] = 5;
		}
	}
	else
	{
		for(int i=0;i<move->m_Count;i++)
		{
			switch(move->step[i])
			{
			case Left_Down:
				CurPosition[move->From.y + move->step_king_kongge1[i] + 1 + move->step_king_kongge2[i]][move->From.x - move->step_king_kongge1[i] - 1 - move->step_king_kongge2[i]] = -5;
				if(-4 == CurPosition[move->From.y + move->step_king_kongge1[i] + 1][move->From.x - move->step_king_kongge1[i] - 1] || -2 == CurPosition[move->From.y + move->step_king_kongge1[i] + 1][move->From.x - move->step_king_kongge1[i] - 1])
				{
					CurPosition[move->From.y + move->step_king_kongge1[i] + 1][move->From.x - move->step_king_kongge1[i] - 1] = -4;
				}
				else
				{
					CurPosition[move->From.y + move->step_king_kongge1[i] + 1][move->From.x - move->step_king_kongge1[i] - 1] = 4;
				}
				move->From.y += (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				move->From.x -= (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				break;
			case Right_Down:
				CurPosition[move->From.y + move->step_king_kongge1[i] + 1 + move->step_king_kongge2[i]][move->From.x + move->step_king_kongge1[i] + 1 + move->step_king_kongge2[i]] = -5;
				if(-4 == CurPosition[move->From.y + move->step_king_kongge1[i] + 1][move->From.x - move->step_king_kongge1[i] - 1] || -2 == CurPosition[move->From.y + move->step_king_kongge1[i] + 1][move->From.x + move->step_king_kongge1[i] + 1])
				{
					CurPosition[move->From.y + move->step_king_kongge1[i] + 1][move->From.x + move->step_king_kongge1[i] + 1] = -4;
				}
				else
				{
					CurPosition[move->From.y + move->step_king_kongge1[i] + 1][move->From.x + move->step_king_kongge1[i] + 1] = 4;
				}
				move->From.y += (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				move->From.x += (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				break;
			case Left_Up:
				CurPosition[move->From.y - move->step_king_kongge1[i] - 1 - move->step_king_kongge2[i]][move->From.x - move->step_king_kongge1[i] - 1 - move->step_king_kongge2[i]] = -5;
				if(-4 == CurPosition[move->From.y + move->step_king_kongge1[i] + 1][move->From.x - move->step_king_kongge1[i] - 1] || -2 == CurPosition[move->From.y - move->step_king_kongge1[i] - 1][move->From.x - move->step_king_kongge1[i] - 1])
				{
					CurPosition[move->From.y - move->step_king_kongge1[i] - 1][move->From.x - move->step_king_kongge1[i] - 1] = -4;
				}
				else
				{
					CurPosition[move->From.y - move->step_king_kongge1[i] - 1][move->From.x - move->step_king_kongge1[i] - 1] = 4;
				}
				move->From.y -= (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				move->From.x -= (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				break;
			case Right_Up:
				CurPosition[move->From.y - move->step_king_kongge1[i] - 1 - move->step_king_kongge2[i]][move->From.x + move->step_king_kongge1[i] + 1 + move->step_king_kongge2[i]] = -5;
				if(-4 == CurPosition[move->From.y + move->step_king_kongge1[i] + 1][move->From.x - move->step_king_kongge1[i] - 1] || -2 == CurPosition[move->From.y - move->step_king_kongge1[i] - 1][move->From.x + move->step_king_kongge1[i] + 1])
				{
					CurPosition[move->From.y - move->step_king_kongge1[i] - 1][move->From.x + move->step_king_kongge1[i] + 1] = -4;
				}
				else
				{
					CurPosition[move->From.y - move->step_king_kongge1[i] - 1][move->From.x + move->step_king_kongge1[i] + 1] = 4;
				}
				move->From.y -= (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				move->From.x += (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				break;
			}
		}
		if(0 == move->m_Count)//没有吃子，只有行棋
		{
			switch(move->step[0])
			{
			case Left_Down:
				CurPosition[move->From.y+1+move->step_king_kongge1[0]][move->From.x-1-move->step_king_kongge1[0]] = 5; 
				move->From.y += (move->step_king_kongge1[0] + 1);
				move->From.x -= (move->step_king_kongge1[0] + 1);
				break;
			case Right_Down:
				CurPosition[move->From.y+1+move->step_king_kongge1[0]][move->From.x+1+move->step_king_kongge1[0]] = 5; 
				move->From.y += (move->step_king_kongge1[0] + 1);
				move->From.x += (move->step_king_kongge1[0] + 1);
				break;
			case Left_Up:
				CurPosition[move->From.y-1-move->step_king_kongge1[0]][move->From.x-1-move->step_king_kongge1[0]] = 5; 
				move->From.y -= (move->step_king_kongge1[0] + 1);
				move->From.x -= (move->step_king_kongge1[0] + 1);
				break;
			case Right_Up:
				CurPosition[move->From.y-1-move->step_king_kongge1[0]][move->From.x+1+move->step_king_kongge1[0]] = 5; 
				move->From.y -= (move->step_king_kongge1[0] + 1);
				move->From.x += (move->step_king_kongge1[0] + 1);
				break;
			}
		}
		else
		{
			CurPosition[move->From.y][move->From.x] = 5;
		}
	}
}
void MakeMoveSure(int m,int n,int nType,int CurPosition[10][10])
{
	CurPosition[m][n] = 0;
	for(int i=0;i<10;i++)
	{
		for(int j=0;j<10;j++)
		{
			switch(CurPosition[i][j])
			{
			case 5:
				CurPosition[i][j] = nType;
				break;
			case -5:
			case 4:
			case -4:
			case 6:
			case -6:
				CurPosition[i][j] = 0;
				break;
			}
		}
	}
}
void UnMakeMove(int CurPosition[10][10])
{
	for(int i=0;i<10;i++)
	{
		for(int j=0;j<10;j++)
		{
			switch(CurPosition[i][j])
			{
			case 5:
			case -5:
				CurPosition[i][j] = 0;
				break;
			case 4:
				CurPosition[i][j] = -1;
				break;
			case -4:
				CurPosition[i][j] = -2;
				break;
			case 6:
				CurPosition[i][j] = 1;
				break;
			case -6:
				CurPosition[i][j] = 2;
				break;
			}
		}
	}
}
void BecomeKing(int CurPosition[10][10])
{
	for(int i=0;i<10;i++)
	{
		if(1 == CurPosition[0][i])
		{
			CurPosition[0][i] = 2;
		}
		if(-1 == CurPosition[9][i])
		{
			CurPosition[9][i] = -2;
		}
	}
}




BOOL CDragonDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (!HIWORD(wParam))  
    {  
		switch(LOWORD(wParam))
		{
		case iMenu+0:
			m_bGo = BLACK_GO;
			break;
		case iMenu+1:
			m_bGo = WRITE_GO;
			break;
		case iMenu+2:
			m_pSE->SetSearchDepth(1);
			break;
		case iMenu+3:
			m_pSE->SetSearchDepth(2);
			break;
		case iMenu+4:
			m_pSE->SetSearchDepth(3);
			break;
		case iMenu+5:
			m_pSE->SetSearchDepth(4);
			break;
		case iMenu+6:
			m_pSE->SetSearchDepth(5);
			break;
		case iMenu+7:
			m_pSE->SetSearchDepth(6);
			break;
		case iMenu+8:
			m_pSE->SetSearchDepth(7);
			break;
		case iMenu+9:
			m_pSE->SetSearchDepth(8);
			break;
		case iMenu+10:
			m_pSE->SetSearchDepth(9);
			break;
		case iMenu+11:
			m_pSE->SetSearchDepth(10);
			break;
		}
    }  
	return CDialogEx::OnCommand(wParam, lParam);
}
