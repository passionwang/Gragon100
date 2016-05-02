
// DragonDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Dragon.h"
#include "DragonDlg.h"
#include "afxdialogex.h"

#define BORDERWIDTH 50 //����(����)��Ե�Ŀ���
#define BORDERHEIGHT 50 //����(����)��Ե�ĸ߶�
#define GRILLEWIDTH 50  //������ÿ�����ӵĸ߶�
#define GRILLEHEIGHT 50 //������ÿ�����ӵĿ���
#define NOCHESS   0     //����

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
const int iBL=50;//�������߾�
const int iBT=50;//�����ϲ�߾�
const int iBS=50;//������λ��λ
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
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CDragonDlg �Ի���



CDragonDlg::CDragonDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDragonDlg::IDD, pParent)
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


// CDragonDlg ��Ϣ��������
CMoveGenerator MoveGenerator ;
BOOL CDragonDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵������ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ����Ӷ���ĳ�ʼ������
	m_Chessman.Create(IDB_CHESS, 50, 50, RGB(80,80,80)); 

	BITMAP BitMap;
	m_BoardBmp.LoadBitmap(IDB_BOARD);
	m_BoardBmp.GetBitmap(&BitMap);
	m_nBoardWidth =  BitMap.bmWidth; 
	m_nBoardHeight = BitMap.bmHeight;
	m_BoardBmp.DeleteObject();


	memcpy(m_ChessBoard, InitChessBoard, sizeof(InitChessBoard));//��ʼ������
	
	CMoveGenerator *pMG;
	CEveluation *pEvel;

	pEvel = new CEveluation;
	//�˴��޸���������
//	m_pSE = new CNegamaxEngine;//������ֵ-�����
//	m_pSE = new CAlphaBetaEngine;//alpha-Beta��֦
//	m_pSE = new CFAlphaBetaEngine;//Fail-soft alpha-beta
//	m_pSE = new CAspirationSearch;//��������
//	m_pSE = new CPVS_Engine;//��С��������
//	m_pSE = new CIDAlphabeta;//�����
//	m_pSE = new CAlphaBetaAndTT;//alpha-Beta��֦+TT�û���
//	m_pSE = new CAlphabeta_HH;//alpha-Beta��֦+HH��ʷ����
	m_pSE = new CNegaScout_TT_HH;//������ֵ+TT�û���+HH��ʷ����
	pMG = &MoveGenerator;
	
	//�˴����������������
	m_pSE->SetSearchDepth(6);
	//�����߷�������-ֻ��һ��
	m_pSE->SetMoveGenerator(pMG);
	//���ù�ֵ-ֻ��һ��
	m_pSE->SetEveluator(pEvel);
//	m_MoveChess.nChessID = NOCHESS;
	m_bGameOver = FALSE;//this code does not contents in books.
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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
	jBrush.CreateSolidBrush(RGB(0,255,0));  //��
	pDC->SelectObject(&jBrush);
	pDC->Rectangle(iBL+11*iBS,iBT+iBS,iBL+12*iBS,iBT+8*iBS);
	pDC->SelectStockObject(BLACK_BRUSH);
	jBrush.DeleteObject();
	jBrush.CreateSolidBrush(RGB(0,0,255));  //��
	pDC->SelectObject(&jBrush);
	pDC->Rectangle(iBL+12*iBS,iBT+iBS,iBL+13*iBS,iBT+8*iBS);
	pDC->SelectStockObject(BLACK_BRUSH);
	jBrush.DeleteObject();
}
// �����Ի���������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
extern int G_nCountEv;//��¼��ֵ�����;
extern int G_nCountTT;
extern int G_nCountTTHH;
void CDragonDlg::iMove()          
{
	if (TRUE == m_bGameOver)
		return;

	int timecount;
	timecount = GetTickCount();
	m_pSE->SearchAGoodMove(m_ChessBoard);	
	BecomeKing(m_ChessBoard);//4.�����ж�
	CString sNodeCount;
	sNodeCount.Format(L" Cost %d ms. %d Nodes were eveluated. %d TT Node Be Found.%d TTHH Node Be Found.", GetTickCount() - timecount,G_nCountEv,G_nCountTT,G_nCountTTHH);
//	sNodeCount.Format(L" Cost %d ms. %d Nodes were eveluated.", GetTickCount() - timecount,count);
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
//�����ƺ����н�
void CDragonDlg::Move(int iX,int iY)     
{
	int nCountMove;
	
	if (TRUE == m_bGameOver/*m_bGameOver*/)
		return;
//	memcpy(m_BackupChessBoard, m_ChessBoard, sizeof(m_ChessBoard));
	m_iM = (iY-iBT)/iBS;  //������ת���������±�
	m_iN = (iX-iBL)/iBS;
	switch(m_ChessBoard[m_iM][m_iN])
	{
	case BLACK://�ҵ���ǰ������ĵ���߷���
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
		}//����ʾ����
		for(int i=0;i<nPosMove;i++)
		{   
			//1.��ʾ
			MakeMoveQi(&oMG.m_nMoveList[0][i],m_ChessBoard);
			//2.��¼��������
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
			//1.�˶�����
			if(m_iM == m_MoveTPos[i].y && m_iN == m_MoveTPos[i].x)
			{
				//2.�����ҵ���������
				MakeMoveQi(&oMG.m_nMoveList[0][i],m_ChessBoard);
				//3.�����ߵľ�������
				MakeMoveSure(m,n,oMG.m_nMoveList[0][i].m_Type,m_ChessBoard);
				//4.�����ж�
				BecomeKing(m_ChessBoard);
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
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
	CDialogEx::OnLButtonDown(nFlags, point);
	int iX,iY;
	iX= point.x;
	iY= point.y;
	if(iInArea(iX,iY,iBL+11*iBS,iBT+iBS,iBL+12*iBS,iBT+8*iBS)) //����������壨���巶Χ��
	{
		iMove();
//		UP_COUNT=1;
	}
	if(iInArea(iX,iY,iBL,iBT,iBL+iBS*10,iBT+iBS*10)) //���̷�Χ
	{
		Move(iX,iY);
//		UP_COUNT=1;
	}
	if(iInArea(iX,iY,iBL+12*iBS,iBT+iBS,iBL+13*iBS,iBT+8*iBS)) //���̷�Χ
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
		if(0 == move->m_Count)//û�г��ӣ�ֻ������
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
		if(0 == move->m_Count)//û�г��ӣ�ֻ������
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

