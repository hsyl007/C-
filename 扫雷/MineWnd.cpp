// MineWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Mine.h"
#include "MineWnd.h"
#include "resource.h"
#include "Mmsystem.h"

#include "DlgCustom.h"
#include "DlgRank.h"
#include "DlgNewRec.h"


#define ID_TIMER WM_USER+1

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMineWnd

CMineWnd::CMineWnd()
{

	//初始化必要成员变量
	m_uIdTimer = 0;
	m_brBG.CreateSolidBrush(RGB(192, 192, 192));
	m_pOldMine = NULL;
	m_pNewMine = NULL;
	m_pSoundClock = NULL;
	m_pSoundDead  = NULL;
	m_pSoundVictory=NULL;

	//加载配置
	LoadConfig();

	//初始化窗口大小
	m_rcClient.SetRect(0,0,20+m_uxNum*16, 64+m_uyNum*16);
	m_rcTime.SetRect(m_rcClient.right-54, 15, m_rcClient.right-13, 40);
	m_rcBtn.SetRect(m_rcClient.right/2-12, 16, m_rcClient.right/2+12, 40);
	m_rcLeaNum.SetRect(16, 15, 57, 40);
	m_rcMineArea.SetRect(MINE_AREA_LEFT, MINE_AREA_TOP, 
		MINE_AREA_LEFT+m_uxNum*MINE_WIDTH, MINE_AREA_TOP+m_uyNum*MINE_HEIGHT);
	
	//初始化游戏
	InitGame();
}

CMineWnd::~CMineWnd()
{
	SaveConfig();
	FreeSoundRsc();
}


BEGIN_MESSAGE_MAP(CMineWnd, CWnd)
	//{{AFX_MSG_MAP(CMineWnd)
	ON_COMMAND(IDM_ABOUT, OnAbout)
	ON_COMMAND(IDM_HELP, OnHelp)
	ON_COMMAND(IDM_COLOR, OnMenuColor)
	ON_COMMAND(IDM_SOUND, OnMenuSound)
	ON_COMMAND(IDM_LEVEL_PRIMARY, OnLevelPrimary)
	ON_COMMAND(IDM_LEVEL_SECONDARY, OnLevelSecondary)
	ON_COMMAND(IDM_LEVEL_ADVANCE, OnLevelAdvance)
	ON_COMMAND(IDM_LEVEL_CUSTOM, OnLevelCustom)
	ON_WM_PAINT()
	ON_COMMAND(IDM_START, OnMenuStart)
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_INITMENU()
	ON_COMMAND(IDM_EXIT, OnMenuExit)
	ON_WM_CLOSE()
	ON_COMMAND(IDM_RANK, OnMenuRank)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMineWnd message handlers

void CMineWnd::OnAbout() 
{
	// TODO: Add your command handler code here
	ShellAbout(this->m_hWnd, "扫雷", "812915118@qq.com",NULL);
}

void CMineWnd::OnHelp() 
{
	// TODO: Add your command handler code here
	::WinExec("HH NTHelp.CHM", SW_SHOW);
}



void CMineWnd::OnMenuColor() 
{
	// TODO: Add your command handler code here
	//设置颜色变量 加载对应位图 重绘
	m_bColorful = !m_bColorful;
	LoadBitmap();
	Invalidate();
}

void CMineWnd::OnMenuSound() 
{
	// TODO: Add your command handler code here
	//设置声音变量 加载声音资源 
	m_bSoundful = !m_bSoundful;
	if(m_bSoundful)
		LoadSoundRsc();
	else
		FreeSoundRsc();
}

void CMineWnd::InitGame()
{
	//根据m_bSoundful m_bColorful加载声音和位图
	LoadBitmap();
	LoadSoundRsc();

	//设置界面上一些必要数据
	m_uLeaveNum = m_uMineNum;
	m_uSpendTime = 0;
	m_uBtnState = BUTTON_NORMAL;
	m_uGameState = GS_WAIT;
	
	//销毁上一次游戏的定时器
	if(m_uIdTimer)
	{
		KillTimer(ID_TIMER);
		m_uIdTimer = 0;
	}
	//初始化雷方块状态 用于绘制界面
	for (UINT i=0; i<m_uyNum; i++)
		for (UINT j=0; j<m_uxNum; j++)
		{
			m_pMines[i][j].uAttrib = ATTRIB_EMPTY;
			m_pMines[i][j].uRow    = i;
			m_pMines[i][j].uCol    = j;
			m_pMines[i][j].uState  = STATE_NORMAL;
			m_pMines[i][j].uOldState=STATE_NORMAL;
		}
}

BOOL CMineWnd::LoadBitmap()
//根据颜色选项加载相应位图
{
	if (m_bColorful)
	{
		m_bmpBtn.DeleteObject();
		m_bmpBtn.LoadBitmap(IDB_BTN_COLOR);

		m_bmpMine.DeleteObject();
		m_bmpMine.LoadBitmap(IDB_MINE_COLOR);

		m_bmpNum.DeleteObject();
		m_bmpNum.LoadBitmap(IDB_NUM_COLOR);
	}
	else
	{
		m_bmpBtn.DeleteObject();
		m_bmpBtn.LoadBitmap(IDB_BTN_GRAY);
		
		m_bmpMine.DeleteObject();
		m_bmpMine.LoadBitmap(IDB_MINE_GRAY);
		
		m_bmpNum.DeleteObject();
		m_bmpNum.LoadBitmap(IDB_NUM_GRAY);
	}
	return TRUE;
}

void CMineWnd::SizeWindow()
//根据难度选择调整窗体大小
{
	UINT uWidth = DEFAULT_FRAME_X + m_uxNum*MINE_WIDTH + 
		LINE_WIDTH_0*3 + SIDE_WIDTH_0 + SIDE_WIDTH_1;

	UINT uHeight = DEFAULT_FRAME_Y + m_uyNum*MINE_HEIGHT + 
		LINE_WIDTH_0*3 + SIDE_WIDTH_0*2 + SIDE_WIDTH_1 + SHELL_S_H;

	SetWindowPos(&wndTopMost, 0, 0, uWidth, uHeight, SWP_NOZORDER | SWP_NOMOVE | SWP_NOCOPYBITS);

	//设置完成后需要更改类中所包含的需要修改的矩形成员变量
	GetClientRect(&m_rcClient);
	m_rcTime.SetRect(m_rcClient.right-54, 15, m_rcClient.right-13, 40);
	m_rcBtn.SetRect(m_rcClient.right/2-12, 16, m_rcClient.right/2+12, 40);
	m_rcLeaNum.SetRect(16, 15, 57, 40);
	m_rcMineArea.SetRect(MINE_AREA_LEFT, MINE_AREA_TOP, 
		MINE_AREA_LEFT+m_uxNum*MINE_WIDTH, MINE_AREA_TOP+m_uyNum*MINE_HEIGHT);
}

void CMineWnd::OnLevelPrimary() 
{
	//选择初级难度  初始化游戏 调整窗口大小
	m_uGameLevel = LEVEL_PRIMARY;

	m_uxNum = PRIMARY_XNUM;
	m_uyNum = PRIMARY_YNUM;
	m_uMineNum = PRIMARY_MINENUM;
	InitGame();
	SizeWindow();
}

void CMineWnd::OnLevelSecondary() 
{
	//选择中级难度 初始化游戏 调整窗口大小
	m_uGameLevel = LEVEL_SECONDARY;

	m_uxNum = SECONDRY_XNUM;
	m_uyNum = SECONDRY_YNUM;
	m_uMineNum = SECONDRY_MINENUM;
	InitGame();
	SizeWindow();
}

void CMineWnd::OnLevelAdvance() 
{
	//选择中级难度 初始化游戏 调整窗口大小
	m_uGameLevel = LEVEL_ADVANCE;

	m_uxNum = ADVANCE_XNUM;
	m_uyNum = ADVANCE_YNUM;
	m_uMineNum = ADVANCE_MINENUM;
	InitGame();
	SizeWindow();
}

void CMineWnd::OnLevelCustom() 
{
	// 自定义难度 弹出自定义对话框进行设置
	m_uGameLevel = LEVEL_CUSTOM;

	CDlgCustom dlgctm;
	//弹出之前 需要初始化当前设置到自定义对话框
	dlgctm.InitialData(m_uxNum, m_uyNum, m_uMineNum);
	dlgctm.DoModal();

	InitGame();
	SizeWindow();
	Invalidate();//在自定义对话框消失后 需要重绘那一块
}

void CMineWnd::LayMine(UINT row, UINT col)
{
		//时间种子
		srand((unsigned)time(NULL));

		UINT i, j;
		for(UINT index=0; index<m_uMineNum; )
		{
			i = rand()%m_uyNum;
			j = rand()%m_uxNum;
			//第一次按下的地方不为雷
			if(i == row && j == col)
				continue;

			if (m_pMines[i][j].uAttrib != ATTRIB_MINE)
			{
				m_pMines[i][j].uAttrib = ATTRIB_MINE;
				index++;
			}
		}
}

void CMineWnd::DrawMineArea(CPaintDC &dc)
{
	//绘制雷区域
	CDC memdc;
	memdc.CreateCompatibleDC(&dc);
	memdc.SelectObject(&m_bmpMine);

	//贴图
	for (UINT i=0; i<m_uyNum; i++)
		for(UINT j=0; j<m_uxNum; j++)
		{
			 dc.StretchBlt(MINEAREA_FRAME_X + MINE_WIDTH*j, MINEAREA_FRAME_Y+MINE_HEIGHT*i,
				 MINE_WIDTH, MINE_HEIGHT, &memdc, 0, MINE_HEIGHT*m_pMines[i][j].uState, 16, 16, SRCCOPY);
		}
}

void CMineWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting


	//创建内存DC和一张大小等于客户区的画布  使得个子函数绘图都在该画布上 
	//再一并BitBlt到客户区 避免屏闪
	
	//创建兼容DC
	CDC dcMem;
	CBitmap bitmap;
	dcMem.CreateCompatibleDC(&dc);
	bitmap.CreateCompatibleBitmap(&dc, m_rcClient.Width(), m_rcClient.Height());
	
	dcMem.SelectObject(&bitmap);
	dcMem.FillRect(&m_rcClient, &m_brBG);//设置背景色

	//绘制各个区域到兼容DC的位图上
	DrawMineArea((CPaintDC&) dcMem);
	DrawButtonArea((CPaintDC&) dcMem);
	DrawShellArea((CPaintDC&) dcMem);
	DrawNumberArea((CPaintDC&) dcMem);

	//再将兼容DC位图整个拷贝到屏幕
	dc.BitBlt(m_rcClient.left, m_rcClient.top, m_rcClient.right, m_rcClient.bottom, &dcMem, 0, 0, SRCCOPY);

	bitmap.DeleteObject();
	
	// Do not call CWnd::OnPaint() for painting messages
}



void CMineWnd::SetDataFromCustom(UINT uxNum, UINT uyNum, UINT uMineNum)
{
	//从自定义对话框中得到数据
	m_uxNum = uxNum;
	m_uyNum = uyNum;
	m_uMineNum = uMineNum;
}

void CMineWnd::DrawButtonArea(CPaintDC &dc)
{
	//绘制笑脸
	CDC memdc;
	memdc.CreateCompatibleDC(&dc);
	memdc.SelectObject(m_bmpBtn);

	UINT ux = m_rcClient.right/2 - 12;	//笑脸按钮x方向起始位置
	
	dc.BitBlt(ux, 16, 24, 24, &memdc, 0, 24*m_uBtnState, SRCCOPY);
	dc.Draw3dRect(ux-1, 15, 26, 26, RGB(128,128,128), RGB(128,128,128));//笑脸外部加一个3d矩形 增加立体感
}

void CMineWnd::DrawShellArea(CPaintDC &dc)	//绘制客户区所有的外壳
{
	//绘制上方和左方的白条
	dc.FillSolidRect(0, 0, m_rcClient.right, LINE_WIDTH_0, RGB(255, 255, 255));
	dc.FillSolidRect(0, 0, LINE_WIDTH_0, m_rcClient.bottom, RGB(255, 255, 255));

	//绘制上方的小外壳 //向内加深 共2层
	dc.Draw3dRect(SHELL_S_START_X, SHELL_S_START_Y, m_rcClient.right-14, 
		SHELL_S_H, RGB(128,128,128), RGB(255,255,255));
	dc.Draw3dRect(SHELL_S_START_X+1, SHELL_S_START_Y+1, m_rcClient.right-16,
		SHELL_S_H-2, RGB(128,128,128), RGB(255,255,255));

	//绘制雷区的大外壳 向内加深 共3层
	dc.Draw3dRect(SHELL_L_START_X, SHELL_L_START_Y, m_rcClient.right-14, 
		m_rcClient.bottom-SHELL_L_START_Y-SIDE_WIDTH_1, RGB(128,128,128), RGB(255,255,255));
	dc.Draw3dRect(SHELL_L_START_X+1, SHELL_L_START_Y+1, m_rcClient.right-16, 
		m_rcClient.bottom-SHELL_L_START_Y-SIDE_WIDTH_1-2, RGB(128,128,128), RGB(255,255,255));
	dc.Draw3dRect(SHELL_L_START_X+2, SHELL_L_START_Y+2, m_rcClient.right-18, 
		m_rcClient.bottom-SHELL_L_START_Y-SIDE_WIDTH_1-4, RGB(128,128,128), RGB(255,255,255));

}

void CMineWnd::DrawNumberArea(CPaintDC &dc)
{
	//画出两个数字框的3D框边
	dc.Draw3dRect(16, 15, 41, 25, RGB(128,128,128), RGB(255,255,255));
	dc.Draw3dRect(m_rcClient.right-54, 15, 41, 25, RGB(128,128,128), RGB(255,255,255));

	CDC memdc;
	memdc.CreateCompatibleDC(&dc);
	memdc.SelectObject(m_bmpNum);
	//画剩余雷数
	int num;
	if(m_uLeaveNum <= 0)//当雷数小于等于零时不再递减
	{
		dc.BitBlt(16, 15, 13, 23, &memdc, 0, 11*23, SRCCOPY);
		
		dc.BitBlt(30, 15, 13, 23, &memdc, 0, 11*23, SRCCOPY);
		
		dc.BitBlt(44, 15, 13, 23, &memdc, 0, 11*23, SRCCOPY);
	}
	else
	{
		num = m_uLeaveNum/100;
		dc.BitBlt(16, 15, 13, 23, &memdc, 0, 276-23*(num+1), SRCCOPY);

		num = (m_uLeaveNum%100)/10;
		dc.BitBlt(30, 15, 13, 23, &memdc, 0, 276-23*(num+1), SRCCOPY);

		num = m_uLeaveNum%10;
		dc.BitBlt(44, 15, 13, 23, &memdc, 0, 276-23*(num+1), SRCCOPY);
	}

	//画所花时间数
	num = m_uSpendTime/100;
	dc.BitBlt(m_rcClient.right-54, 15, 13, 23, &memdc, 0, 276-23*(num+1), SRCCOPY);

	num = (m_uSpendTime%100)/10;
	dc.BitBlt(m_rcClient.right-40, 15, 13, 23, &memdc, 0, 276-23*(num+1), SRCCOPY);

	num = m_uSpendTime%10;
	dc.BitBlt(m_rcClient.right-26, 15, 13, 23, &memdc, 0, 276-23*(num+1), SRCCOPY);
}
	
void CMineWnd::OnMenuStart() 
{
	// TODO: Add your command handler code here
	switch(m_uGameState)
	{
	case GS_WAIT:
		break;
	case GS_RUN:
	case GS_DEAD:
	case GS_VICTORY:
		InitGame();
		Invalidate(FALSE);
		m_uGameState = GS_WAIT;
		break;
	}
}

void CMineWnd::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	//播放声音
	if (m_pSoundClock)
	{
		sndPlaySound((LPCTSTR)m_pSoundClock, SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
	}
	//绘制时间
	CRect rcTime(m_rcClient.right-54, 15, m_rcClient.right-13, 38);
	if (nIDEvent == ID_TIMER)
	{
		m_uSpendTime++;
		InvalidateRect(&rcTime,FALSE);
	}
	//不超过999
	if (m_uSpendTime == 999)
	{
		KillTimer(ID_TIMER);
		m_uIdTimer = 0;
	}
	CWnd::OnTimer(nIDEvent);
}



void CMineWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetCapture();
	m_bBtnClick = FALSE; //保存按下左键时是否点击的按钮  用于鼠标松开的时候按钮重绘选择
	m_bLRBtnDown = FALSE;
	//只绘制按下的雷
	CRect rcMine;
	
	if(m_rcBtn.PtInRect(point))	//按下按钮  绘制按下效果
	{
		m_uBtnState = BUTTON_DOWN;
		m_bBtnClick = TRUE;
		InvalidateRect(m_rcBtn,FALSE);
	}
	else if (m_rcMineArea.PtInRect(point))	//当鼠标在雷区按下时，只使正常状态和位置状态的雷块变为按下
	{
		switch(m_uGameState)
		{
			case GS_WAIT:
			case GS_RUN:
				m_pNewMine = GetMine(point.x, point.y);
				if(m_pNewMine == NULL)
					return;
				GetMineRect(m_pNewMine->uRow, m_pNewMine->uCol, &rcMine);
				if (m_pNewMine->uState == STATE_NORMAL)	//正常
				{	
					m_pNewMine->uState = STATE_EMPTY;	//按下
					InvalidateRect(&rcMine,FALSE);
				}
				if (m_pNewMine->uState == STATE_DICEY)	//未知状态
				{
					m_pNewMine->uState = STATE_DICEY_DOWN;//显示为按下
					InvalidateRect(&rcMine,FALSE);
				}
				break;

			case GS_DEAD:
			case GS_VICTORY:
				return;

			default:
				break;
		}
		//笑脸担忧
		m_uBtnState = BUTTON_CLICK;
		InvalidateRect(m_rcBtn,FALSE);
		//保存 用于恢复
		m_pOldMine = m_pNewMine;

		if (nFlags == (MK_LBUTTON|MK_RBUTTON))
		{
			m_bLRBtnDown = TRUE;//左右键同时按下的标记 用于鼠标移动辨识
			OnLRButtonDown(m_pNewMine->uRow, m_pNewMine->uCol);
		}

	}
	else//点到其他区域 只是出现担忧表情
	{
		if(m_uGameState == GS_WAIT || m_uGameState == GS_RUN)
			m_uBtnState = BUTTON_CLICK;
		InvalidateRect(&m_rcBtn,FALSE);
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CMineWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
 		// TODO: Add your message handler code here and/or call default
 		ReleaseCapture();
 
 		if (m_rcBtn.PtInRect(point))
 		{
 			switch(m_uGameState)
 			{
 			case GS_WAIT:
 				m_uBtnState = BUTTON_NORMAL;
				InvalidateRect(&m_rcBtn, FALSE);
 				break;
 			case GS_RUN:
 			case GS_DEAD:
 				InitGame();
 				Invalidate(FALSE);
 				break;
 			default:
 				break;
 			}
 			return;
 		}
 
 		UINT aroundNum;
 		if(m_rcMineArea.PtInRect(point))
 		{
			switch(m_uGameState)
 			{
 			case GS_WAIT:
 			case GS_RUN:
				m_pOldMine = GetMine(point.x, point.y);
				if (!m_pOldMine)
 					return;

				//左右键同时按下
				if (m_bLRBtnDown)
				{
					m_bLRBtnDown = FALSE;//防止放了左键后还以九宫格移动
					//先恢复九宫格状态
					OnLRButtonUp(m_pOldMine->uRow, m_pOldMine->uCol);
					if (m_pOldMine->uState == STATE_EMPTY || m_pOldMine->uState == STATE_FLAG
						||m_pOldMine->uState == STATE_DICEY || m_pOldMine->uState == STATE_NORMAL)
					{
						m_uBtnState = BUTTON_NORMAL;
						break;
					}
					//确认是否可以打开周围的方块
					if(GetAroundFlags(m_pOldMine->uRow, m_pOldMine->uCol) == GetAroundNum(m_pOldMine->uRow,m_pOldMine->uCol))
						if(!OpenAround(m_pOldMine->uRow, m_pOldMine->uCol))//判断打开时有无预估错误的雷
						{
							Dead(m_pOldMine->uRow, m_pOldMine->uCol);
							return;
						}
					
				}
				else//只是左键按下
				{
					if (m_uGameState == GS_WAIT)//如果在等待状态  则开始
 					{
 						if (m_uIdTimer)//销毁之前的定时器
 						{
 							KillTimer(ID_TIMER);
 							m_uIdTimer = 0;
						}
 						m_uSpendTime = 0;//设定定时器
 						m_uIdTimer = SetTimer(ID_TIMER, 1000, NULL);
 
 						m_uGameState = GS_RUN;//游戏状态
 
 						//放置雷  保证第一次按下的方块不会是雷
 						LayMine(m_pOldMine->uRow, m_pOldMine->uCol);
 					}
 					
 					if (m_pOldMine->uOldState == STATE_NORMAL)//判断上一个状态(因为uState此时为按下)
 					{
 						if (m_pOldMine->uAttrib == ATTRIB_MINE)
 						{
 							Dead(m_pOldMine->uRow,  m_pOldMine->uCol);
 							return;
 						}
 					
 						aroundNum = GetAroundNum(m_pOldMine->uRow, m_pOldMine->uCol);
 						if (aroundNum == 0)//周围没有雷 扩展四周雷状态
 						{
 							ExpandMines(m_pOldMine->uRow, m_pOldMine->uCol);
 						}
 						else//画出周围雷数
 						{
 							DrawDownNum(m_pOldMine, aroundNum);
 						}
 					}//对于未知状态  不予响应
 					else if (m_pOldMine->uOldState == STATE_DICEY)
					{
 						m_pOldMine->uState = STATE_DICEY;
 					}
				}
				m_uBtnState = BUTTON_NORMAL;
				break;
 			case GS_DEAD:
				m_uBtnState = BUTTON_DEAD;
				break;
 			case GS_VICTORY:
				m_uBtnState = BUTTON_VICTORY;
 				break;
 
 			default:
 				break;
 			}
		}
		else//点击到其他区域 根据游戏状态恢复笑脸
 
 		{
			if (m_uGameState == GS_WAIT || m_uGameState==GS_RUN)
			{
 				m_uBtnState = BUTTON_NORMAL;
			}
			else if (m_uGameState = GS_DEAD)
			{
				m_uBtnState = BUTTON_DEAD;
			}
			else
			{
				m_uBtnState = BUTTON_VICTORY;
			}
 		}
		InvalidateRect(&m_rcBtn, FALSE);
		//判断胜利
 		Victory();
 
 		CWnd::OnLButtonUp(nFlags, point);
 }


MINEWND* CMineWnd::GetMine(UINT x, UINT y)//通过坐标点得到对应雷方块的地址
{
	//判断是否在雷区
	if (x<=MINEAREA_FRAME_X || x>MINEAREA_FRAME_X+m_uxNum*MINE_WIDTH)
	{
		return NULL;
	}
	
	if (y<=MINEAREA_FRAME_Y || y>MINEAREA_FRAME_Y+m_uyNum*MINE_HEIGHT)
	{
		return NULL;
	}

	//得到行列数
	UINT col = (UINT)(x-MINEAREA_FRAME_X)/MINE_WIDTH;
	UINT row = (UINT)(y-MINEAREA_FRAME_Y)/MINE_HEIGHT;

	return &m_pMines[row][col];
}

void CMineWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(nFlags == MK_LBUTTON || nFlags == (MK_LBUTTON|MK_RBUTTON))
	{
		//在按钮区域内
		if (m_rcBtn.PtInRect(point))
		{
			switch(m_uGameState)
			{
			case GS_WAIT:
			case GS_RUN:
				if(m_bBtnClick)//如果左键按下时点击的是该按钮
					m_uBtnState = BUTTON_DOWN;//按钮按下
				else
					m_uBtnState = BUTTON_CLICK;//否则绘制担忧表情
			break;
			case GS_DEAD:			//如果游戏已经结束，就不用再担忧了....
			case GS_VICTORY:
				m_uBtnState = BUTTON_DOWN;
			break;
			default:
				break;
			}
			InvalidateRect(m_rcBtn,FALSE);
			return;
		}
		

		//在雷区
		CRect rcMine;
		if(m_rcMineArea.PtInRect(point))
		{
			switch(m_uGameState)
			{
			case GS_RUN:
			case GS_WAIT:
				m_pNewMine = GetMine(point.x, point.y);
			
				if(m_pNewMine == m_pOldMine)//当前后两个方块是同一个方块或者他们同为空 直接返回
					return;

				if (m_bLRBtnDown)//左右键同时按下 移动逻辑与左键类似 只不过这里是一个九宫格 
				{
					if (m_pOldMine)
					{
						OnLRButtonUp(m_pOldMine->uRow, m_pOldMine->uCol);
					}
					if (m_pNewMine)
					{
						OnLRButtonDown(m_pNewMine->uRow, m_pNewMine->uCol);
					}
					
					//转移点 重绘	
					m_pOldMine = m_pNewMine;
					break;
				}

				if (m_pNewMine)//方块切换时需要出现按下当前方块效果并且恢复上一个方块状态
				{
					//按下当前方块
					GetMineRect(m_pNewMine->uRow, m_pNewMine->uCol, &rcMine);
					if (m_pNewMine->uState == STATE_NORMAL)
					{
						m_pNewMine->uState = STATE_EMPTY;
						InvalidateRect(&rcMine,FALSE);
					}
					if (m_pNewMine->uState == STATE_DICEY)
					{
						m_pNewMine->uState = STATE_DICEY_DOWN;
						InvalidateRect(&rcMine,FALSE);
					}
					
				}
				if(m_pOldMine)
				{
						//恢复(即弹起)上一个方块状态
				GetMineRect(m_pOldMine->uRow,m_pOldMine->uCol, &rcMine);
				if (m_pOldMine->uOldState == STATE_NORMAL)
				{
					m_pOldMine->uState = STATE_NORMAL;
					InvalidateRect(&rcMine,FALSE);
				}
				if(m_pOldMine->uOldState == STATE_DICEY)
				{
					m_pOldMine->uState = STATE_DICEY;
					InvalidateRect(&rcMine,FALSE);
				}
			}
				
				//转移点 重绘	
			m_pOldMine = m_pNewMine;
			break;
				

			case GS_DEAD:
				m_uBtnState = BUTTON_DEAD;
				InvalidateRect(&m_rcBtn,FALSE);
				return;
			case GS_VICTORY:
				m_uBtnState = BUTTON_VICTORY;
				InvalidateRect(&m_rcBtn,FALSE);
				return;
			}
    		
		}
		else//在其他区域  此时不必用m_pOldMine 直接恢复m_pNewMine状态
		{
			switch(m_uGameState)
			{
				case GS_WAIT:
				case GS_RUN:
					if(m_pOldMine)
					{
						GetMineRect(m_pOldMine->uRow, m_pOldMine->uCol, &rcMine);
						if (m_pOldMine->uOldState == STATE_NORMAL)
						{
							m_pOldMine->uState = STATE_NORMAL;
							InvalidateRect(&rcMine,FALSE);	
						}
						if (m_pOldMine->uOldState == STATE_DICEY)
						{
							m_pOldMine->uState = STATE_DICEY;
							InvalidateRect(&rcMine,FALSE);
						}
					}
					break;

				case GS_VICTORY:
					m_uBtnState = BUTTON_VICTORY;
					break;
				case GS_DEAD:
					m_uBtnState = BUTTON_DEAD;
					break;
				default:
					break;

			}
			//将两个标志雷置为空 这样使得鼠标再次从非客户区回到边界雷快时 
			//不会由于m_pNewMine == m_pOldMine而停止绘制按下效果
			m_pNewMine = m_pOldMine = NULL;
		}

	}
	CWnd::OnMouseMove(nFlags, point);
}



UINT CMineWnd::GetAroundNum(UINT row, UINT col)//得到指定方块周围的雷数
{
	UINT rowmin;
	UINT rowmax;
	//衡量边界情况
	if (row == 0)
	{
		rowmin = 0;
		rowmax = row+2;
	}
	else if (row == m_uyNum-1)
	{
		rowmin = row-1;
		rowmax = row+1;
	}
	else
	{
		rowmin = row-1;
		rowmax = row+2;
	}

	UINT colmin;
	UINT colmax;
	//衡量边界情况
	if (col == 0)
	{
		colmin = 0;
		colmax = col+2;
	}
	else if (col == m_uxNum-1)
	{
		colmin = col-1;
		colmax = col+1;
	}
	else
	{
		colmin = col-1;
		colmax = col+2;
	}

	UINT around = 0;
	for (UINT i=rowmin; i<rowmax; i++)
	{
		for (UINT j=colmin; j<colmax; j++)
		{
			if (m_pMines[i][j].uAttrib == ATTRIB_MINE)
			{
				around++;
			}
		}
	}
	return around;
}

BOOL CMineWnd::IsMine(UINT row, UINT col)
{
	return m_pMines[row][col].uAttrib == ATTRIB_MINE;
}

BOOL CMineWnd::Victory()
{

	//检测是否还有未翻开的雷 和 未知雷
	for (UINT i=0; i<m_uyNum; i++)
	{
		for (UINT j=0; j<m_uxNum; j++)
		{
			if (m_pMines[i][j].uState == STATE_NORMAL)
			{
				return FALSE;
			}
			if (m_pMines[i][j].uState == STATE_DICEY)
			{
				return FALSE;
			}
		}
	}

	//改变游戏状态 绘制胜利笑脸
	m_uGameState = GS_VICTORY;
	m_uBtnState  = BUTTON_VICTORY;
	InvalidateRect(&m_rcBtn);

	//播放声音
	if(m_pSoundVictory)
	{
		sndPlaySound((LPCTSTR)m_pSoundVictory,SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
	}

	//销毁定时器
	if (m_uIdTimer)
	{
		KillTimer(ID_TIMER);
		m_uIdTimer;
	}

	//判读是否破了纪录
	//如果是新纪录 弹出新纪录对话框 记录高手名字和门派
	CDlgNewRec dlgNew;
	switch(m_uGameLevel)
	{
	case LEVEL_PRIMARY:
		if (m_uSpendTime < m_uPriScore)
		{
			dlgNew.SetLevel(LEVEL_PRIMARY);
			if(IDOK == dlgNew.DoModal())
			{
				m_strPriCamp = dlgNew.GetRecCamp();
				m_strPriHolder = dlgNew.GetRecHolder();
				m_uPriScore = m_uSpendTime;
			}
		}
		break;
	case LEVEL_SECONDARY:
		if (m_uSpendTime < m_uSecScore)
		{
			dlgNew.SetLevel(LEVEL_SECONDARY);
			if(IDOK == dlgNew.DoModal())
			{
				m_strSecCamp = dlgNew.GetRecCamp();
				m_strSecCamp = dlgNew.GetRecHolder();
				m_uSecScore = m_uSpendTime;
			}
		}
		break;

	case LEVEL_ADVANCE:
		if (m_uSpendTime < m_uAdvScore)
		{
			dlgNew.SetLevel(LEVEL_ADVANCE);
			if(IDOK == dlgNew.DoModal())
			{
				m_strAdvCamp = dlgNew.GetRecCamp();
				m_strAdvHolder = dlgNew.GetRecHolder();
				m_uAdvScore = m_uSpendTime;
			}
		}
		break;
	}
	return TRUE;
}

//绘制按下雷时周围的数字
void CMineWnd::DrawDownNum(MINEWND* pMine, UINT num)
{
	pMine->uState = 15-num;
	pMine->uOldState = 15-num;
	CRect rect(MINEAREA_FRAME_X+pMine->uCol*16, MINEAREA_FRAME_Y+pMine->uRow*16,
		MINEAREA_FRAME_X+pMine->uCol*16+16, MINEAREA_FRAME_Y+pMine->uRow*16+16);


	InvalidateRect(&rect);
}

void CMineWnd::ExpandMines(UINT row, UINT col)
{
	//先画出本方块周围雷数
	UINT num = GetAroundNum(row, col);
	DrawDownNum(&m_pMines[row][col], num);

	if (num == 0)
	{
		
		UINT rowmin;
		UINT rowmax;
		//衡量边界情况
		if (row == 0)
		{
			rowmin = 0;
			rowmax = row+2;
		}
		else if (row == m_uyNum-1)
		{
			rowmin = row-1;
			rowmax = row+1;
		}
		else
		{
			rowmin = row-1;
			rowmax = row+2;
		}
		
		UINT colmin;
		UINT colmax;
		//衡量边界情况
		if (col == 0)
		{
			colmin = 0;
			colmax = col+2;
		}
		else if (col == m_uxNum-1)
		{
			colmin = col-1;
			colmax = col+1;
		}
		else
		{
			colmin = col-1;
			colmax = col+2;
		}

		//对于周围不是雷的未翻开方块 继续扩展
		for(UINT i=rowmin; i<rowmax; i++)
			for (UINT j=colmin; j<colmax; j++)
			{
				if (!(i==row&&j==col) && m_pMines[i][j].uState == STATE_NORMAL && 
					m_pMines[i][j].uAttrib != ATTRIB_MINE)
				{
						ExpandMines(i, j);
				}
			}
	}
}

void CMineWnd::Dead(UINT row, UINT col)
{
	if(m_bSoundful)//声音播放
	{
		sndPlaySound((LPCTSTR)m_pSoundDead,SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
	}
	//设置当前方块爆炸
	m_pMines[row][col].uState = STATE_BLAST;
	m_pMines[row][col].uOldState = STATE_BLAST;

	for (UINT i=0; i<m_uyNum; i++)
		for (UINT j=0; j<m_uxNum; j++)
		{
			//显示出未发现的雷的真实位置
			if (m_pMines[i][j].uState == STATE_NORMAL)
			{
				if (m_pMines[i][j].uAttrib == ATTRIB_MINE)
				{
					m_pMines[i][j].uOldState = STATE_MINE;
					m_pMines[i][j].uState = STATE_MINE;
				}
			}
			//提示出用户预估错误的雷方块(及红旗下面不是雷)
			else if (m_pMines[i][j].uState == STATE_FLAG)
			{
				if (m_pMines[i][j].uAttrib != ATTRIB_MINE)
				{
					m_pMines[i][j].uOldState = STATE_ERROR;
					m_pMines[i][j].uState = STATE_ERROR;
				}
			}
		}
	m_uBtnState = BUTTON_DEAD;
	m_uGameState = GS_DEAD;
	if (m_uIdTimer)
	{
		KillTimer(ID_TIMER);
		m_uIdTimer = 0;
	}
	Invalidate(FALSE);
}

void CMineWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bLRBtnDown = FALSE;
	CRect rcMine;
	if (m_rcMineArea.PtInRect(point))
	{
		switch(m_uGameState)
		{
		case GS_WAIT:
		case GS_RUN:
			m_pNewMine = GetMine(point.x, point.y);
			if (!m_pNewMine)
			{
				return;
			}
			//左右键同时按下
			if (nFlags == (MK_LBUTTON|MK_RBUTTON))
			{
				OnLRButtonDown(m_pNewMine->uRow, m_pNewMine->uCol);
				m_bLRBtnDown = TRUE;
				return;
			}
			GetMineRect(m_pNewMine->uRow, m_pNewMine->uCol, &rcMine);
			switch(m_pNewMine->uState)
			{
			case STATE_NORMAL:
				//正常变标记 仅当已使用红旗数小于雷数时 才可标记
				if (m_uLeaveNum>0)
				{
					m_pNewMine->uState = STATE_FLAG;
					m_pNewMine->uOldState = STATE_FLAG;
					m_uLeaveNum--;
					break;
				}
				else
					return;
			case STATE_FLAG:
				//标记变未知
				m_pNewMine->uState = STATE_DICEY;
				m_pNewMine->uOldState = STATE_DICEY;
				m_uLeaveNum++;
				break;
			case STATE_DICEY:
				//未知变正常
				m_pNewMine->uState = STATE_NORMAL;
				m_pNewMine->uOldState = STATE_NORMAL;
				break;
			default:
				break;
			}
			break;
		case GS_DEAD:
		case GS_VICTORY:
				return;
		}
	}
	if (m_pNewMine)
	{
		//绘制剩余雷数和点击的方块
		InvalidateRect(&rcMine);
		InvalidateRect(&m_rcLeaNum);
	}
	CWnd::OnRButtonDown(nFlags, point);
}

void CMineWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bLRBtnDown)
	{
		m_pOldMine = GetMine(point.x,point.y);
		if (m_pOldMine == NULL)
			return;
		OnLRButtonUp(m_pOldMine->uRow, m_pOldMine->uCol);

		//判读是否可以九宫格扩展雷区
		m_bLRBtnDown = FALSE;//置为假 防止放了一个键后还以九宫格移动
		if (m_pOldMine->uState == STATE_EMPTY || m_pOldMine->uState == STATE_FLAG
			||m_pOldMine->uState == STATE_DICEY || m_pOldMine->uState == STATE_NORMAL)
			return;
		//确认是否可以打开周围的方块
		if(GetAroundFlags(m_pOldMine->uRow, m_pOldMine->uCol) == GetAroundNum(m_pOldMine->uRow,m_pOldMine->uCol))
			if(!OpenAround(m_pOldMine->uRow, m_pOldMine->uCol))//判断打开时有无预估错误的雷
			{
				Dead(m_pOldMine->uRow, m_pOldMine->uCol);
				return;
			}
	}
	Victory();
	CWnd::OnRButtonUp(nFlags, point);
}



//加载声音资源
BOOL CMineWnd::LoadSoundRsc()
{
	if (m_bSoundful)
	{
		HMODULE hMudle;
		hMudle = AfxGetResourceHandle();

		HRSRC hrsrc;
		hrsrc = FindResource(hMudle, MAKEINTRESOURCE(IDR_WAVE_CLOCK), _T("WAVE"));
		m_pSoundClock = LoadResource(hMudle, hrsrc);

		hrsrc = FindResource(hMudle, MAKEINTRESOURCE(IDR_WAVE_DEAD), _T("WAVE"));
		m_pSoundDead = LoadResource(hMudle, hrsrc);

		hrsrc = FindResource(hMudle, MAKEINTRESOURCE(IDR_WAVE_VICTORY), _T("WAVE"));
		m_pSoundVictory = LoadResource(hMudle, hrsrc);

		return TRUE;
	}
	return FALSE;
}

BOOL CMineWnd::FreeSoundRsc()//释放声音资源
{
	if (m_pSoundClock)
	{
		FreeResource(m_pSoundClock);
	}
	if (m_pSoundDead)
	{
		FreeResource(m_pSoundDead);
	}
	if (m_pSoundVictory)
	{
		FreeResource(m_pSoundVictory);
	}
	return TRUE;
}

void CMineWnd::OnInitMenu(CMenu* pMenu) 
{
	CWnd::OnInitMenu(pMenu);
	//初始化成员变量子菜单
	if (NULL == (m_pSubMenu = pMenu->GetSubMenu(0)))
	{
		AfxMessageBox("初始化菜单失败!");
		return;
	}
	else
		//标记各个子菜单
	{
		SetCheckColor();
		SetCheckLevel();
		SetCheckMark();
		SetCheckSound();
	}
	// TODO: Add your message handler code here
	
}

void CMineWnd::SetCheckLevel()//标记所选的难度
{
	if (m_pSubMenu)
	{
		m_pSubMenu->CheckMenuItem(IDM_LEVEL_PRIMARY, MF_UNCHECKED|MF_BYCOMMAND);
		m_pSubMenu->CheckMenuItem(IDM_LEVEL_SECONDARY, MF_UNCHECKED|MF_BYCOMMAND);
		m_pSubMenu->CheckMenuItem(IDM_LEVEL_ADVANCE, MF_UNCHECKED|MF_BYCOMMAND);
		m_pSubMenu->CheckMenuItem(IDM_LEVEL_CUSTOM, MF_UNCHECKED|MF_BYCOMMAND);
		switch(m_uGameLevel)
		{
		case LEVEL_PRIMARY:
			m_pSubMenu->CheckMenuItem(IDM_LEVEL_PRIMARY, MF_CHECKED|MF_BYCOMMAND);
			break;
		case LEVEL_SECONDARY:
			m_pSubMenu->CheckMenuItem(IDM_LEVEL_SECONDARY, MF_CHECKED|MF_BYCOMMAND);
			break;
		case LEVEL_ADVANCE:
			m_pSubMenu->CheckMenuItem(IDM_LEVEL_ADVANCE, MF_CHECKED|MF_BYCOMMAND);
			break;
		case LEVEL_CUSTOM:
			m_pSubMenu->CheckMenuItem(IDM_LEVEL_CUSTOM, MF_CHECKED|MF_BYCOMMAND);
			break;
		default:
			break;
		}
	}
}

void CMineWnd::SetCheckSound()
{
	//设置声音子菜单标记
	if (m_pSubMenu)
	{
		if (m_bSoundful)
			m_pSubMenu->CheckMenuItem(IDM_SOUND, MF_CHECKED|MF_BYCOMMAND);
		else
			m_pSubMenu->CheckMenuItem(IDM_SOUND, MF_UNCHECKED|MF_BYCOMMAND);
	}
}

void CMineWnd::SetCheckColor()
{
	//设置颜色子菜单标记
	if (m_pSubMenu)
	{
		if(m_bColorful)
			m_pSubMenu->CheckMenuItem(IDM_COLOR, MF_CHECKED|MF_BYCOMMAND);
		else
			m_pSubMenu->CheckMenuItem(IDM_COLOR, MF_UNCHECKED|MF_BYCOMMAND);
	}
}

void CMineWnd::SetCheckMark()
{

}

void CMineWnd::GetMineRect(UINT row, UINT col, CRect *rect)
{
	//根据行列号得到雷块矩形
	rect->SetRect(MINEAREA_FRAME_X+16*col, MINEAREA_FRAME_Y+16*row,
		MINEAREA_FRAME_X+16*col+16, MINEAREA_FRAME_Y+16*row+16);
}

BOOL CMineWnd::LoadConfig()//加载ini中数据
{
	//得到配置文件config的路径
	char exeFullPath[MAX_PATH];
	GetModuleFileName(NULL, exeFullPath, MAX_PATH);
	CString strPath(exeFullPath);
	CString configPath =strPath.Left(strPath.ReverseFind('\\'));
	configPath += "\\config.ini";

	//得到MINE节点下的难度，雷数，规模
	m_uGameLevel= GetPrivateProfileInt("MINE", "LEVEL", 0, configPath);
	m_uMineNum	= GetPrivateProfileInt("MINE", "MINENUM", 10, configPath);
	m_uxNum		= GetPrivateProfileInt("MINE", "XNUM", 9, configPath);
	m_uyNum		= GetPrivateProfileInt("MINE", "YNUM", 9, configPath);

	//得到初级记录
	char priHolder[80];
	char priCamp[80];
	GetPrivateProfileString("PRIMARY RECORD", "CAMP", "峨眉山", priCamp, 80, configPath);
	GetPrivateProfileString("PRIMARY RECORD", "HOLDER", "张三丰", priHolder, 80, configPath);
	m_uPriScore = GetPrivateProfileInt("PRIMARY RECORD", "SCORE", 999, configPath);
	m_strPriCamp = priCamp;
	m_strPriHolder = priHolder;

	//得到中级记录
	char secHolder[80];
	char secCamp[80];
	GetPrivateProfileString("SECONDARY RECORD", "CAMP", "洞子桥", secCamp, 80, configPath);
	GetPrivateProfileString("SECONDARY RECORD", "HOLDER", "王半仙", secHolder, 80, configPath);
	m_uSecScore = GetPrivateProfileInt("SECONDARY RECORD", "SCORE", 999, configPath);
	m_strSecCamp = secCamp;
	m_strSecHolder = secHolder;

	//得到高级记录
	char advHolder[80];
	char advCamp[80];
	GetPrivateProfileString("ADVANCE RECORD", "CAMP", "西门", advCamp, 80, configPath);
	GetPrivateProfileString("ADVANCE RECORD", "HOLDER", "吹雪", advHolder, 80, configPath);
	m_uAdvScore = GetPrivateProfileInt("ADVANCE RECORD", "SCORE", 999, configPath);
	m_strAdvCamp = advCamp;
	m_strAdvHolder = advHolder;

	//得到声音颜色等配置
	m_bSoundful = GetPrivateProfileInt("OTHER CONFIG", "SOUNDFUL", 1, configPath);
	m_bColorful = GetPrivateProfileInt("OTHER CONFIG", "COLORFUL", 1, configPath);

	return TRUE;
	
 }

BOOL CMineWnd::SaveConfig()//保存数据到ini
{
	CString strLevel;	//等级
	CString strMineNum;	//雷数
	CString strxNum, stryNum;//规模
	CString strPriScore;//初级记录
	CString strSecScore;//中级记录
	CString  strAdvScore;//高级记录
	CString strSound,   strColor;//其他配置

	//得到配置文件config文件
	char exeFullPath[MAX_PATH];
	GetModuleFileName(NULL, exeFullPath, MAX_PATH);
	CString strPath(exeFullPath);
	CString configPath =strPath.Left(strPath.ReverseFind('\\'));
	configPath += "\\config.ini";
	
	//保存难度 雷数 规模等参数
	strLevel.Format("%u", m_uGameLevel);
	WritePrivateProfileString("MINE", "LEVEL", strLevel, configPath);

	strMineNum.Format("%u", m_uMineNum);
	WritePrivateProfileString("MINE", "MINENUM", strMineNum, configPath);

	strxNum.Format("%u", m_uxNum);
	stryNum.Format("%u", m_uyNum);
	WritePrivateProfileString("MINE", "XNUM", strxNum, configPath);
	WritePrivateProfileString("MINE", "YNUM", stryNum, configPath);
	
	//保存初级记录
	strPriScore.Format("%u", m_uPriScore);
	WritePrivateProfileString("PRIMARY RECORD", "CAMP", m_strPriCamp, configPath);
	WritePrivateProfileString("PRIMARY RECORD", "HOLDER", m_strPriHolder, configPath);
	WritePrivateProfileString("PRIMARY RECORD", "SCORE", strPriScore, configPath);

	//保存中级记录
	strSecScore.Format("%u", m_uSecScore);
	WritePrivateProfileString("SECONDARY RECORD", "CAMP", m_strSecCamp, configPath);
	WritePrivateProfileString("SECONDARY RECORD", "HOLDER", m_strSecHolder, configPath);
	WritePrivateProfileString("SECONDARY RECORD", "SCORE", strSecScore, configPath);

	//保存高级记录
	strAdvScore.Format("%u", m_uAdvScore);
	WritePrivateProfileString("ADVANCE RECORD", "CAMP", m_strAdvCamp, configPath);
	WritePrivateProfileString("ADVANCE RECORD", "HOLDER", m_strAdvHolder, configPath);
	WritePrivateProfileString("ADVANCE RECORD", "SCORE", strAdvScore, configPath);

	//保存声音 颜色设置
	strSound.Format("%d", m_bSoundful);
	WritePrivateProfileString("OHTER CONFIG", "SOUNDERFUL", strSound, configPath);
	strColor.Format("%d", m_bColorful);
	WritePrivateProfileString("OHTER CONFIG", "COLORFUL", strColor, configPath);

	return TRUE;
}

void CMineWnd::OnMenuExit() 
{
	// TODO: Add your command handler code here
	if(IDOK == AfxMessageBox("Are You Sure ?", MB_OKCANCEL))
	{
		PostQuitMessage(0);
	}
}

void CMineWnd::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	SaveConfig();
	CWnd::OnClose();
}

void CMineWnd::OnMenuRank() 
{
	// TODO: Add your command handler code here
	CDlgRank dlgRank;
	//用当前最高纪录初始化排行榜对话框
	dlgRank.SetPriRecord(m_strPriCamp, m_strPriHolder, m_uPriScore);
	dlgRank.SetSecRecord(m_strSecCamp, m_strSecHolder, m_uSecScore);
	dlgRank.SetAdvRecord(m_strAdvCamp, m_strAdvHolder, m_uAdvScore);
	dlgRank.DoModal();
}

void CMineWnd::OnLRButtonDown(UINT row, UINT col)
{
	UINT rowmin;
	UINT rowmax;
	//衡量边界情况
	if (row == 0)
	{
		rowmin = 0;
		rowmax = row+2;
	}
	else if (row == m_uyNum-1)
	{
		rowmin = row-1;
		rowmax = row+1;
	}
	else
	{
		rowmin = row-1;
		rowmax = row+2;
	}
	
	UINT colmin;
	UINT colmax;
	//衡量边界情况
	if (col == 0)
	{
		colmin = 0;
		colmax = col+2;
	}
	else if (col == m_uxNum-1)
	{
		colmin = col-1;
		colmax = col+1;
	}
	else
	{
		colmin = col-1;
		colmax = col+2;
	}
	//将周围所有正常或未知状态的雷按下
	for(UINT i=rowmin; i<rowmax; i++)
		for (UINT j=colmin; j<colmax; j++)
		{
			if (m_pMines[i][j].uOldState == STATE_NORMAL)
			{
				m_pMines[i][j].uState = STATE_EMPTY;
			}
			if (m_pMines[i][j].uOldState == STATE_DICEY)
			{
				m_pMines[i][j].uState = STATE_DICEY_DOWN;
			}
		}
	//重绘指定区域
	CRect rcMines(MINE_AREA_LEFT+m_pMines[rowmin][colmin].uCol*16, MINE_AREA_TOP+m_pMines[rowmin][colmin].uRow*16,
		MINE_AREA_LEFT+m_pMines[rowmax][colmax].uCol*16, MINE_AREA_TOP+m_pMines[rowmax][colmax].uRow*16);
	InvalidateRect(&rcMines, FALSE);
}



void CMineWnd::OnLRButtonUp(UINT row, UINT col)
{
	UINT rowmin;
	UINT rowmax;
	//衡量边界情况
	if (row == 0)
	{
		rowmin = 0;
		rowmax = row+2;
	}
	else if (row == m_uyNum-1)
	{
		rowmin = row-1;
		rowmax = row+1;
	}
	else
	{
		rowmin = row-1;
		rowmax = row+2;
	}
	
	UINT colmin;
	UINT colmax;
	//衡量边界情况
	if (col == 0)
	{
		colmin = 0;
		colmax = col+2;
	}
	else if (col == m_uxNum-1)
	{
		colmin = col-1;
		colmax = col+1;
	}
	else
	{
		colmin = col-1;
		colmax = col+2;
	}
	//将周围的方块恢复原本状态
	for(UINT i=rowmin; i<rowmax; i++)
		for (UINT j=colmin; j<colmax; j++)
		{
			if (m_pMines[i][j].uOldState == STATE_NORMAL)
			{
				m_pMines[i][j].uState = STATE_NORMAL;
			}
			if (m_pMines[i][j].uOldState == STATE_DICEY)
			{
				m_pMines[i][j].uState = STATE_DICEY;
			}
		}
		//重绘指定区域
		CRect rcMines(MINE_AREA_LEFT+m_pMines[rowmin][colmin].uCol*16, MINE_AREA_TOP+m_pMines[rowmin][colmin].uRow*16,
			MINE_AREA_LEFT+m_pMines[rowmax][colmax].uCol*16, MINE_AREA_TOP+m_pMines[rowmax][colmax].uRow*16);
	InvalidateRect(&rcMines, FALSE);
}

UINT CMineWnd::GetAroundFlags(UINT row, UINT col)
{
	UINT rowmin;
	UINT rowmax;
	//衡量边界情况
	if (row == 0)
	{
		rowmin = 0;
		rowmax = row+2;
	}
	else if (row == m_uyNum-1)
	{
		rowmin = row-1;
		rowmax = row+1;
	}
	else
	{
		rowmin = row-1;
		rowmax = row+2;
	}
	
	UINT colmin;
	UINT colmax;
	//衡量边界情况
	if (col == 0)
	{
		colmin = 0;
		colmax = col+2;
	}
	else if (col == m_uxNum-1)
	{
		colmin = col-1;
		colmax = col+1;
	}
	else
	{
		colmin = col-1;
		colmax = col+2;
	}
	//获取周围的红旗数
	UINT flagNum = 0;
	for(UINT i=rowmin; i<rowmax; i++)
		for (UINT j=colmin; j<colmax; j++)
		{
			if (m_pMines[i][j].uState == STATE_FLAG)
				flagNum++;
		}
	return flagNum;
}

BOOL CMineWnd::OpenAround(UINT row, UINT col)
{
	UINT rowmin;
	UINT rowmax;
	//衡量边界情况
	if (row == 0)
	{
		rowmin = 0;
		rowmax = row+2;
	}
	else if (row == m_uyNum-1)
	{
		rowmin = row-1;
		rowmax = row+1;
	}
	else
	{
		rowmin = row-1;
		rowmax = row+2;
	}
	
	UINT colmin;
	UINT colmax;
	//衡量边界情况
	if (col == 0)
	{
		colmin = 0;
		colmax = col+2;
	}
	else if (col == m_uxNum-1)
	{
		colmin = col-1;
		colmax = col+1;
	}
	else
	{
		colmin = col-1;
		colmax = col+2;
	}
	//如果有预估错误的雷 则返回FALSE 并且将m_pOldMine指向预估错误的雷
	UINT arroud;
	for(UINT i=rowmin; i<rowmax; i++)
		for (UINT j=colmin; j<colmax; j++)
		{
			if (m_pMines[i][j].uState==STATE_FLAG&&m_pMines[i][j].uAttrib!=ATTRIB_MINE)
			{
				m_pOldMine = &m_pMines[i][j];
				return FALSE;
			}
			if (m_pMines[i][j].uState == STATE_NORMAL)
			{
				if((arroud = GetAroundNum(i,j)) == 0)
					ExpandMines(i,j);
				else
					DrawDownNum(&m_pMines[i][j], arroud);
			}
		}
	return TRUE;
}
