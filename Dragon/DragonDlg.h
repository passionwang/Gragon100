
// DragonDlg.h : ͷ�ļ�
//
#pragma once
#include "FactorySearch.h"
#include "afxwin.h"
#include <stack>
using namespace std;
// CDragonDlg �Ի���
class CDragonDlg : public CDialogEx
{
// ����
public:
	CDragonDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DRAGON_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bGameOver;
	int m_bGo;
	
	POINT m_ptMoveChess;
	CBitmap m_BoardBmp;
	CImageList m_Chessman;
	int m_nBoardWidth; 
	int m_nBoardHeight;
	CSearchEngine *m_pSE;
	int m_SearchDepth;
	CMoveGenerator *m_pMG;
	CEveluation *m_pEvel;
	CFactoryToSearch m_Factory;
	stack<CUndoNode*> m_stackUndo;
	
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void iMove();
	//�����ƺ����н�
	void Move(int iX,int iY);   
	CEdit m_Output;
	void UndoPosition();
	void DestoryListPosition();
	void TranslateToChessBoard();
	void TranslateToTranslateBoard();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	CEdit m_Edit_Nodes;
	CEdit m_Edit_TT;
	CEdit m_Edit_TTHH;
	CEdit m_Edit_ETime;
	CEdit m_Edit_GTime;
	CEdit m_Edit_WriteCount;
	CEdit m_Edit_Black_Count;
	int m_All_Time;
	CEdit m_Edit_AllTime;
};
