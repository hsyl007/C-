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

	//��ʼ����Ҫ��Ա����
	m_uIdTimer = 0;
	m_brBG.CreateSolidBrush(RGB(192, 192, 192));
	m_pOldMine = NULL;
	m_pNewMine = NULL;
	m_pSoundClock = NULL;
	m_pSoundDead  = NULL;
	m_pSoundVictory=NULL;

	//��������
	LoadConfig();

	//��ʼ�����ڴ�С
	m_rcClient.SetRect(0,0,20+m_uxNum*16, 64+m_uyNum*16);
	m_rcTime.SetRect(m_rcClient.right-54, 15, m_rcClient.right-13, 40);
	m_rcBtn.SetRect(m_rcClient.right/2-12, 16, m_rcClient.right/2+12, 40);
	m_rcLeaNum.SetRect(16, 15, 57, 40);
	m_rcMineArea.SetRect(MINE_AREA_LEFT, MINE_AREA_TOP, 
		MINE_AREA_LEFT+m_uxNum*MINE_WIDTH, MINE_AREA_TOP+m_uyNum*MINE_HEIGHT);
	
	//��ʼ����Ϸ
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
	ShellAbout(this->m_hWnd, "ɨ��", "812915118@qq.com",NULL);
}

void CMineWnd::OnHelp() 
{
	// TODO: Add your command handler code here
	::WinExec("HH NTHelp.CHM", SW_SHOW);
}



void CMineWnd::OnMenuColor() 
{
	// TODO: Add your command handler code here
	//������ɫ���� ���ض�Ӧλͼ �ػ�
	m_bColorful = !m_bColorful;
	LoadBitmap();
	Invalidate();
}

void CMineWnd::OnMenuSound() 
{
	// TODO: Add your command handler code here
	//������������ ����������Դ 
	m_bSoundful = !m_bSoundful;
	if(m_bSoundful)
		LoadSoundRsc();
	else
		FreeSoundRsc();
}

void CMineWnd::InitGame()
{
	//����m_bSoundful m_bColorful����������λͼ
	LoadBitmap();
	LoadSoundRsc();

	//���ý�����һЩ��Ҫ����
	m_uLeaveNum = m_uMineNum;
	m_uSpendTime = 0;
	m_uBtnState = BUTTON_NORMAL;
	m_uGameState = GS_WAIT;
	
	//������һ����Ϸ�Ķ�ʱ��
	if(m_uIdTimer)
	{
		KillTimer(ID_TIMER);
		m_uIdTimer = 0;
	}
	//��ʼ���׷���״̬ ���ڻ��ƽ���
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
//������ɫѡ�������Ӧλͼ
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
//�����Ѷ�ѡ����������С
{
	UINT uWidth = DEFAULT_FRAME_X + m_uxNum*MINE_WIDTH + 
		LINE_WIDTH_0*3 + SIDE_WIDTH_0 + SIDE_WIDTH_1;

	UINT uHeight = DEFAULT_FRAME_Y + m_uyNum*MINE_HEIGHT + 
		LINE_WIDTH_0*3 + SIDE_WIDTH_0*2 + SIDE_WIDTH_1 + SHELL_S_H;

	SetWindowPos(&wndTopMost, 0, 0, uWidth, uHeight, SWP_NOZORDER | SWP_NOMOVE | SWP_NOCOPYBITS);

	//������ɺ���Ҫ������������������Ҫ�޸ĵľ��γ�Ա����
	GetClientRect(&m_rcClient);
	m_rcTime.SetRect(m_rcClient.right-54, 15, m_rcClient.right-13, 40);
	m_rcBtn.SetRect(m_rcClient.right/2-12, 16, m_rcClient.right/2+12, 40);
	m_rcLeaNum.SetRect(16, 15, 57, 40);
	m_rcMineArea.SetRect(MINE_AREA_LEFT, MINE_AREA_TOP, 
		MINE_AREA_LEFT+m_uxNum*MINE_WIDTH, MINE_AREA_TOP+m_uyNum*MINE_HEIGHT);
}

void CMineWnd::OnLevelPrimary() 
{
	//ѡ������Ѷ�  ��ʼ����Ϸ �������ڴ�С
	m_uGameLevel = LEVEL_PRIMARY;

	m_uxNum = PRIMARY_XNUM;
	m_uyNum = PRIMARY_YNUM;
	m_uMineNum = PRIMARY_MINENUM;
	InitGame();
	SizeWindow();
}

void CMineWnd::OnLevelSecondary() 
{
	//ѡ���м��Ѷ� ��ʼ����Ϸ �������ڴ�С
	m_uGameLevel = LEVEL_SECONDARY;

	m_uxNum = SECONDRY_XNUM;
	m_uyNum = SECONDRY_YNUM;
	m_uMineNum = SECONDRY_MINENUM;
	InitGame();
	SizeWindow();
}

void CMineWnd::OnLevelAdvance() 
{
	//ѡ���м��Ѷ� ��ʼ����Ϸ �������ڴ�С
	m_uGameLevel = LEVEL_ADVANCE;

	m_uxNum = ADVANCE_XNUM;
	m_uyNum = ADVANCE_YNUM;
	m_uMineNum = ADVANCE_MINENUM;
	InitGame();
	SizeWindow();
}

void CMineWnd::OnLevelCustom() 
{
	// �Զ����Ѷ� �����Զ���Ի����������
	m_uGameLevel = LEVEL_CUSTOM;

	CDlgCustom dlgctm;
	//����֮ǰ ��Ҫ��ʼ����ǰ���õ��Զ���Ի���
	dlgctm.InitialData(m_uxNum, m_uyNum, m_uMineNum);
	dlgctm.DoModal();

	InitGame();
	SizeWindow();
	Invalidate();//���Զ���Ի�����ʧ�� ��Ҫ�ػ���һ��
}

void CMineWnd::LayMine(UINT row, UINT col)
{
		//ʱ������
		srand((unsigned)time(NULL));

		UINT i, j;
		for(UINT index=0; index<m_uMineNum; )
		{
			i = rand()%m_uyNum;
			j = rand()%m_uxNum;
			//��һ�ΰ��µĵط���Ϊ��
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
	//����������
	CDC memdc;
	memdc.CreateCompatibleDC(&dc);
	memdc.SelectObject(&m_bmpMine);

	//��ͼ
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


	//�����ڴ�DC��һ�Ŵ�С���ڿͻ����Ļ���  ʹ�ø��Ӻ�����ͼ���ڸû����� 
	//��һ��BitBlt���ͻ��� ��������
	
	//��������DC
	CDC dcMem;
	CBitmap bitmap;
	dcMem.CreateCompatibleDC(&dc);
	bitmap.CreateCompatibleBitmap(&dc, m_rcClient.Width(), m_rcClient.Height());
	
	dcMem.SelectObject(&bitmap);
	dcMem.FillRect(&m_rcClient, &m_brBG);//���ñ���ɫ

	//���Ƹ������򵽼���DC��λͼ��
	DrawMineArea((CPaintDC&) dcMem);
	DrawButtonArea((CPaintDC&) dcMem);
	DrawShellArea((CPaintDC&) dcMem);
	DrawNumberArea((CPaintDC&) dcMem);

	//�ٽ�����DCλͼ������������Ļ
	dc.BitBlt(m_rcClient.left, m_rcClient.top, m_rcClient.right, m_rcClient.bottom, &dcMem, 0, 0, SRCCOPY);

	bitmap.DeleteObject();
	
	// Do not call CWnd::OnPaint() for painting messages
}



void CMineWnd::SetDataFromCustom(UINT uxNum, UINT uyNum, UINT uMineNum)
{
	//���Զ���Ի����еõ�����
	m_uxNum = uxNum;
	m_uyNum = uyNum;
	m_uMineNum = uMineNum;
}

void CMineWnd::DrawButtonArea(CPaintDC &dc)
{
	//����Ц��
	CDC memdc;
	memdc.CreateCompatibleDC(&dc);
	memdc.SelectObject(m_bmpBtn);

	UINT ux = m_rcClient.right/2 - 12;	//Ц����ťx������ʼλ��
	
	dc.BitBlt(ux, 16, 24, 24, &memdc, 0, 24*m_uBtnState, SRCCOPY);
	dc.Draw3dRect(ux-1, 15, 26, 26, RGB(128,128,128), RGB(128,128,128));//Ц���ⲿ��һ��3d���� ���������
}

void CMineWnd::DrawShellArea(CPaintDC &dc)	//���ƿͻ������е����
{
	//�����Ϸ����󷽵İ���
	dc.FillSolidRect(0, 0, m_rcClient.right, LINE_WIDTH_0, RGB(255, 255, 255));
	dc.FillSolidRect(0, 0, LINE_WIDTH_0, m_rcClient.bottom, RGB(255, 255, 255));

	//�����Ϸ���С��� //���ڼ��� ��2��
	dc.Draw3dRect(SHELL_S_START_X, SHELL_S_START_Y, m_rcClient.right-14, 
		SHELL_S_H, RGB(128,128,128), RGB(255,255,255));
	dc.Draw3dRect(SHELL_S_START_X+1, SHELL_S_START_Y+1, m_rcClient.right-16,
		SHELL_S_H-2, RGB(128,128,128), RGB(255,255,255));

	//���������Ĵ���� ���ڼ��� ��3��
	dc.Draw3dRect(SHELL_L_START_X, SHELL_L_START_Y, m_rcClient.right-14, 
		m_rcClient.bottom-SHELL_L_START_Y-SIDE_WIDTH_1, RGB(128,128,128), RGB(255,255,255));
	dc.Draw3dRect(SHELL_L_START_X+1, SHELL_L_START_Y+1, m_rcClient.right-16, 
		m_rcClient.bottom-SHELL_L_START_Y-SIDE_WIDTH_1-2, RGB(128,128,128), RGB(255,255,255));
	dc.Draw3dRect(SHELL_L_START_X+2, SHELL_L_START_Y+2, m_rcClient.right-18, 
		m_rcClient.bottom-SHELL_L_START_Y-SIDE_WIDTH_1-4, RGB(128,128,128), RGB(255,255,255));

}

void CMineWnd::DrawNumberArea(CPaintDC &dc)
{
	//�����������ֿ��3D���
	dc.Draw3dRect(16, 15, 41, 25, RGB(128,128,128), RGB(255,255,255));
	dc.Draw3dRect(m_rcClient.right-54, 15, 41, 25, RGB(128,128,128), RGB(255,255,255));

	CDC memdc;
	memdc.CreateCompatibleDC(&dc);
	memdc.SelectObject(m_bmpNum);
	//��ʣ������
	int num;
	if(m_uLeaveNum <= 0)//������С�ڵ�����ʱ���ٵݼ�
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

	//������ʱ����
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

	//��������
	if (m_pSoundClock)
	{
		sndPlaySound((LPCTSTR)m_pSoundClock, SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
	}
	//����ʱ��
	CRect rcTime(m_rcClient.right-54, 15, m_rcClient.right-13, 38);
	if (nIDEvent == ID_TIMER)
	{
		m_uSpendTime++;
		InvalidateRect(&rcTime,FALSE);
	}
	//������999
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
	m_bBtnClick = FALSE; //���水�����ʱ�Ƿ����İ�ť  ��������ɿ���ʱ��ť�ػ�ѡ��
	m_bLRBtnDown = FALSE;
	//ֻ���ư��µ���
	CRect rcMine;
	
	if(m_rcBtn.PtInRect(point))	//���°�ť  ���ư���Ч��
	{
		m_uBtnState = BUTTON_DOWN;
		m_bBtnClick = TRUE;
		InvalidateRect(m_rcBtn,FALSE);
	}
	else if (m_rcMineArea.PtInRect(point))	//���������������ʱ��ֻʹ����״̬��λ��״̬���׿��Ϊ����
	{
		switch(m_uGameState)
		{
			case GS_WAIT:
			case GS_RUN:
				m_pNewMine = GetMine(point.x, point.y);
				if(m_pNewMine == NULL)
					return;
				GetMineRect(m_pNewMine->uRow, m_pNewMine->uCol, &rcMine);
				if (m_pNewMine->uState == STATE_NORMAL)	//����
				{	
					m_pNewMine->uState = STATE_EMPTY;	//����
					InvalidateRect(&rcMine,FALSE);
				}
				if (m_pNewMine->uState == STATE_DICEY)	//δ֪״̬
				{
					m_pNewMine->uState = STATE_DICEY_DOWN;//��ʾΪ����
					InvalidateRect(&rcMine,FALSE);
				}
				break;

			case GS_DEAD:
			case GS_VICTORY:
				return;

			default:
				break;
		}
		//Ц������
		m_uBtnState = BUTTON_CLICK;
		InvalidateRect(m_rcBtn,FALSE);
		//���� ���ڻָ�
		m_pOldMine = m_pNewMine;

		if (nFlags == (MK_LBUTTON|MK_RBUTTON))
		{
			m_bLRBtnDown = TRUE;//���Ҽ�ͬʱ���µı�� ��������ƶ���ʶ
			OnLRButtonDown(m_pNewMine->uRow, m_pNewMine->uCol);
		}

	}
	else//�㵽�������� ֻ�ǳ��ֵ��Ǳ���
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

				//���Ҽ�ͬʱ����
				if (m_bLRBtnDown)
				{
					m_bLRBtnDown = FALSE;//��ֹ����������ԾŹ����ƶ�
					//�Ȼָ��Ź���״̬
					OnLRButtonUp(m_pOldMine->uRow, m_pOldMine->uCol);
					if (m_pOldMine->uState == STATE_EMPTY || m_pOldMine->uState == STATE_FLAG
						||m_pOldMine->uState == STATE_DICEY || m_pOldMine->uState == STATE_NORMAL)
					{
						m_uBtnState = BUTTON_NORMAL;
						break;
					}
					//ȷ���Ƿ���Դ���Χ�ķ���
					if(GetAroundFlags(m_pOldMine->uRow, m_pOldMine->uCol) == GetAroundNum(m_pOldMine->uRow,m_pOldMine->uCol))
						if(!OpenAround(m_pOldMine->uRow, m_pOldMine->uCol))//�жϴ�ʱ����Ԥ���������
						{
							Dead(m_pOldMine->uRow, m_pOldMine->uCol);
							return;
						}
					
				}
				else//ֻ���������
				{
					if (m_uGameState == GS_WAIT)//����ڵȴ�״̬  ��ʼ
 					{
 						if (m_uIdTimer)//����֮ǰ�Ķ�ʱ��
 						{
 							KillTimer(ID_TIMER);
 							m_uIdTimer = 0;
						}
 						m_uSpendTime = 0;//�趨��ʱ��
 						m_uIdTimer = SetTimer(ID_TIMER, 1000, NULL);
 
 						m_uGameState = GS_RUN;//��Ϸ״̬
 
 						//������  ��֤��һ�ΰ��µķ��鲻������
 						LayMine(m_pOldMine->uRow, m_pOldMine->uCol);
 					}
 					
 					if (m_pOldMine->uOldState == STATE_NORMAL)//�ж���һ��״̬(��ΪuState��ʱΪ����)
 					{
 						if (m_pOldMine->uAttrib == ATTRIB_MINE)
 						{
 							Dead(m_pOldMine->uRow,  m_pOldMine->uCol);
 							return;
 						}
 					
 						aroundNum = GetAroundNum(m_pOldMine->uRow, m_pOldMine->uCol);
 						if (aroundNum == 0)//��Χû���� ��չ������״̬
 						{
 							ExpandMines(m_pOldMine->uRow, m_pOldMine->uCol);
 						}
 						else//������Χ����
 						{
 							DrawDownNum(m_pOldMine, aroundNum);
 						}
 					}//����δ֪״̬  ������Ӧ
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
		else//������������� ������Ϸ״̬�ָ�Ц��
 
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
		//�ж�ʤ��
 		Victory();
 
 		CWnd::OnLButtonUp(nFlags, point);
 }


MINEWND* CMineWnd::GetMine(UINT x, UINT y)//ͨ�������õ���Ӧ�׷���ĵ�ַ
{
	//�ж��Ƿ�������
	if (x<=MINEAREA_FRAME_X || x>MINEAREA_FRAME_X+m_uxNum*MINE_WIDTH)
	{
		return NULL;
	}
	
	if (y<=MINEAREA_FRAME_Y || y>MINEAREA_FRAME_Y+m_uyNum*MINE_HEIGHT)
	{
		return NULL;
	}

	//�õ�������
	UINT col = (UINT)(x-MINEAREA_FRAME_X)/MINE_WIDTH;
	UINT row = (UINT)(y-MINEAREA_FRAME_Y)/MINE_HEIGHT;

	return &m_pMines[row][col];
}

void CMineWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(nFlags == MK_LBUTTON || nFlags == (MK_LBUTTON|MK_RBUTTON))
	{
		//�ڰ�ť������
		if (m_rcBtn.PtInRect(point))
		{
			switch(m_uGameState)
			{
			case GS_WAIT:
			case GS_RUN:
				if(m_bBtnClick)//����������ʱ������Ǹð�ť
					m_uBtnState = BUTTON_DOWN;//��ť����
				else
					m_uBtnState = BUTTON_CLICK;//������Ƶ��Ǳ���
			break;
			case GS_DEAD:			//�����Ϸ�Ѿ��������Ͳ����ٵ�����....
			case GS_VICTORY:
				m_uBtnState = BUTTON_DOWN;
			break;
			default:
				break;
			}
			InvalidateRect(m_rcBtn,FALSE);
			return;
		}
		

		//������
		CRect rcMine;
		if(m_rcMineArea.PtInRect(point))
		{
			switch(m_uGameState)
			{
			case GS_RUN:
			case GS_WAIT:
				m_pNewMine = GetMine(point.x, point.y);
			
				if(m_pNewMine == m_pOldMine)//��ǰ������������ͬһ�������������ͬΪ�� ֱ�ӷ���
					return;

				if (m_bLRBtnDown)//���Ҽ�ͬʱ���� �ƶ��߼���������� ֻ����������һ���Ź��� 
				{
					if (m_pOldMine)
					{
						OnLRButtonUp(m_pOldMine->uRow, m_pOldMine->uCol);
					}
					if (m_pNewMine)
					{
						OnLRButtonDown(m_pNewMine->uRow, m_pNewMine->uCol);
					}
					
					//ת�Ƶ� �ػ�	
					m_pOldMine = m_pNewMine;
					break;
				}

				if (m_pNewMine)//�����л�ʱ��Ҫ���ְ��µ�ǰ����Ч�����һָ���һ������״̬
				{
					//���µ�ǰ����
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
						//�ָ�(������)��һ������״̬
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
				
				//ת�Ƶ� �ػ�	
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
		else//����������  ��ʱ������m_pOldMine ֱ�ӻָ�m_pNewMine״̬
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
			//��������־����Ϊ�� ����ʹ������ٴδӷǿͻ����ص��߽��׿�ʱ 
			//��������m_pNewMine == m_pOldMine��ֹͣ���ư���Ч��
			m_pNewMine = m_pOldMine = NULL;
		}

	}
	CWnd::OnMouseMove(nFlags, point);
}



UINT CMineWnd::GetAroundNum(UINT row, UINT col)//�õ�ָ��������Χ������
{
	UINT rowmin;
	UINT rowmax;
	//�����߽����
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
	//�����߽����
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

	//����Ƿ���δ�������� �� δ֪��
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

	//�ı���Ϸ״̬ ����ʤ��Ц��
	m_uGameState = GS_VICTORY;
	m_uBtnState  = BUTTON_VICTORY;
	InvalidateRect(&m_rcBtn);

	//��������
	if(m_pSoundVictory)
	{
		sndPlaySound((LPCTSTR)m_pSoundVictory,SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
	}

	//���ٶ�ʱ��
	if (m_uIdTimer)
	{
		KillTimer(ID_TIMER);
		m_uIdTimer;
	}

	//�ж��Ƿ����˼�¼
	//������¼�¼ �����¼�¼�Ի��� ��¼�������ֺ�����
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

//���ư�����ʱ��Χ������
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
	//�Ȼ�����������Χ����
	UINT num = GetAroundNum(row, col);
	DrawDownNum(&m_pMines[row][col], num);

	if (num == 0)
	{
		
		UINT rowmin;
		UINT rowmax;
		//�����߽����
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
		//�����߽����
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

		//������Χ�����׵�δ�������� ������չ
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
	if(m_bSoundful)//��������
	{
		sndPlaySound((LPCTSTR)m_pSoundDead,SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
	}
	//���õ�ǰ���鱬ը
	m_pMines[row][col].uState = STATE_BLAST;
	m_pMines[row][col].uOldState = STATE_BLAST;

	for (UINT i=0; i<m_uyNum; i++)
		for (UINT j=0; j<m_uxNum; j++)
		{
			//��ʾ��δ���ֵ��׵���ʵλ��
			if (m_pMines[i][j].uState == STATE_NORMAL)
			{
				if (m_pMines[i][j].uAttrib == ATTRIB_MINE)
				{
					m_pMines[i][j].uOldState = STATE_MINE;
					m_pMines[i][j].uState = STATE_MINE;
				}
			}
			//��ʾ���û�Ԥ��������׷���(���������治����)
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
			//���Ҽ�ͬʱ����
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
				//�������� ������ʹ�ú�����С������ʱ �ſɱ��
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
				//��Ǳ�δ֪
				m_pNewMine->uState = STATE_DICEY;
				m_pNewMine->uOldState = STATE_DICEY;
				m_uLeaveNum++;
				break;
			case STATE_DICEY:
				//δ֪������
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
		//����ʣ�������͵���ķ���
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

		//�ж��Ƿ���ԾŹ�����չ����
		m_bLRBtnDown = FALSE;//��Ϊ�� ��ֹ����һ�������ԾŹ����ƶ�
		if (m_pOldMine->uState == STATE_EMPTY || m_pOldMine->uState == STATE_FLAG
			||m_pOldMine->uState == STATE_DICEY || m_pOldMine->uState == STATE_NORMAL)
			return;
		//ȷ���Ƿ���Դ���Χ�ķ���
		if(GetAroundFlags(m_pOldMine->uRow, m_pOldMine->uCol) == GetAroundNum(m_pOldMine->uRow,m_pOldMine->uCol))
			if(!OpenAround(m_pOldMine->uRow, m_pOldMine->uCol))//�жϴ�ʱ����Ԥ���������
			{
				Dead(m_pOldMine->uRow, m_pOldMine->uCol);
				return;
			}
	}
	Victory();
	CWnd::OnRButtonUp(nFlags, point);
}



//����������Դ
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

BOOL CMineWnd::FreeSoundRsc()//�ͷ�������Դ
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
	//��ʼ����Ա�����Ӳ˵�
	if (NULL == (m_pSubMenu = pMenu->GetSubMenu(0)))
	{
		AfxMessageBox("��ʼ���˵�ʧ��!");
		return;
	}
	else
		//��Ǹ����Ӳ˵�
	{
		SetCheckColor();
		SetCheckLevel();
		SetCheckMark();
		SetCheckSound();
	}
	// TODO: Add your message handler code here
	
}

void CMineWnd::SetCheckLevel()//�����ѡ���Ѷ�
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
	//���������Ӳ˵����
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
	//������ɫ�Ӳ˵����
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
	//�������кŵõ��׿����
	rect->SetRect(MINEAREA_FRAME_X+16*col, MINEAREA_FRAME_Y+16*row,
		MINEAREA_FRAME_X+16*col+16, MINEAREA_FRAME_Y+16*row+16);
}

BOOL CMineWnd::LoadConfig()//����ini������
{
	//�õ������ļ�config��·��
	char exeFullPath[MAX_PATH];
	GetModuleFileName(NULL, exeFullPath, MAX_PATH);
	CString strPath(exeFullPath);
	CString configPath =strPath.Left(strPath.ReverseFind('\\'));
	configPath += "\\config.ini";

	//�õ�MINE�ڵ��µ��Ѷȣ���������ģ
	m_uGameLevel= GetPrivateProfileInt("MINE", "LEVEL", 0, configPath);
	m_uMineNum	= GetPrivateProfileInt("MINE", "MINENUM", 10, configPath);
	m_uxNum		= GetPrivateProfileInt("MINE", "XNUM", 9, configPath);
	m_uyNum		= GetPrivateProfileInt("MINE", "YNUM", 9, configPath);

	//�õ�������¼
	char priHolder[80];
	char priCamp[80];
	GetPrivateProfileString("PRIMARY RECORD", "CAMP", "��üɽ", priCamp, 80, configPath);
	GetPrivateProfileString("PRIMARY RECORD", "HOLDER", "������", priHolder, 80, configPath);
	m_uPriScore = GetPrivateProfileInt("PRIMARY RECORD", "SCORE", 999, configPath);
	m_strPriCamp = priCamp;
	m_strPriHolder = priHolder;

	//�õ��м���¼
	char secHolder[80];
	char secCamp[80];
	GetPrivateProfileString("SECONDARY RECORD", "CAMP", "������", secCamp, 80, configPath);
	GetPrivateProfileString("SECONDARY RECORD", "HOLDER", "������", secHolder, 80, configPath);
	m_uSecScore = GetPrivateProfileInt("SECONDARY RECORD", "SCORE", 999, configPath);
	m_strSecCamp = secCamp;
	m_strSecHolder = secHolder;

	//�õ��߼���¼
	char advHolder[80];
	char advCamp[80];
	GetPrivateProfileString("ADVANCE RECORD", "CAMP", "����", advCamp, 80, configPath);
	GetPrivateProfileString("ADVANCE RECORD", "HOLDER", "��ѩ", advHolder, 80, configPath);
	m_uAdvScore = GetPrivateProfileInt("ADVANCE RECORD", "SCORE", 999, configPath);
	m_strAdvCamp = advCamp;
	m_strAdvHolder = advHolder;

	//�õ�������ɫ������
	m_bSoundful = GetPrivateProfileInt("OTHER CONFIG", "SOUNDFUL", 1, configPath);
	m_bColorful = GetPrivateProfileInt("OTHER CONFIG", "COLORFUL", 1, configPath);

	return TRUE;
	
 }

BOOL CMineWnd::SaveConfig()//�������ݵ�ini
{
	CString strLevel;	//�ȼ�
	CString strMineNum;	//����
	CString strxNum, stryNum;//��ģ
	CString strPriScore;//������¼
	CString strSecScore;//�м���¼
	CString  strAdvScore;//�߼���¼
	CString strSound,   strColor;//��������

	//�õ������ļ�config�ļ�
	char exeFullPath[MAX_PATH];
	GetModuleFileName(NULL, exeFullPath, MAX_PATH);
	CString strPath(exeFullPath);
	CString configPath =strPath.Left(strPath.ReverseFind('\\'));
	configPath += "\\config.ini";
	
	//�����Ѷ� ���� ��ģ�Ȳ���
	strLevel.Format("%u", m_uGameLevel);
	WritePrivateProfileString("MINE", "LEVEL", strLevel, configPath);

	strMineNum.Format("%u", m_uMineNum);
	WritePrivateProfileString("MINE", "MINENUM", strMineNum, configPath);

	strxNum.Format("%u", m_uxNum);
	stryNum.Format("%u", m_uyNum);
	WritePrivateProfileString("MINE", "XNUM", strxNum, configPath);
	WritePrivateProfileString("MINE", "YNUM", stryNum, configPath);
	
	//���������¼
	strPriScore.Format("%u", m_uPriScore);
	WritePrivateProfileString("PRIMARY RECORD", "CAMP", m_strPriCamp, configPath);
	WritePrivateProfileString("PRIMARY RECORD", "HOLDER", m_strPriHolder, configPath);
	WritePrivateProfileString("PRIMARY RECORD", "SCORE", strPriScore, configPath);

	//�����м���¼
	strSecScore.Format("%u", m_uSecScore);
	WritePrivateProfileString("SECONDARY RECORD", "CAMP", m_strSecCamp, configPath);
	WritePrivateProfileString("SECONDARY RECORD", "HOLDER", m_strSecHolder, configPath);
	WritePrivateProfileString("SECONDARY RECORD", "SCORE", strSecScore, configPath);

	//����߼���¼
	strAdvScore.Format("%u", m_uAdvScore);
	WritePrivateProfileString("ADVANCE RECORD", "CAMP", m_strAdvCamp, configPath);
	WritePrivateProfileString("ADVANCE RECORD", "HOLDER", m_strAdvHolder, configPath);
	WritePrivateProfileString("ADVANCE RECORD", "SCORE", strAdvScore, configPath);

	//�������� ��ɫ����
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
	//�õ�ǰ��߼�¼��ʼ�����а�Ի���
	dlgRank.SetPriRecord(m_strPriCamp, m_strPriHolder, m_uPriScore);
	dlgRank.SetSecRecord(m_strSecCamp, m_strSecHolder, m_uSecScore);
	dlgRank.SetAdvRecord(m_strAdvCamp, m_strAdvHolder, m_uAdvScore);
	dlgRank.DoModal();
}

void CMineWnd::OnLRButtonDown(UINT row, UINT col)
{
	UINT rowmin;
	UINT rowmax;
	//�����߽����
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
	//�����߽����
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
	//����Χ����������δ֪״̬���װ���
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
	//�ػ�ָ������
	CRect rcMines(MINE_AREA_LEFT+m_pMines[rowmin][colmin].uCol*16, MINE_AREA_TOP+m_pMines[rowmin][colmin].uRow*16,
		MINE_AREA_LEFT+m_pMines[rowmax][colmax].uCol*16, MINE_AREA_TOP+m_pMines[rowmax][colmax].uRow*16);
	InvalidateRect(&rcMines, FALSE);
}



void CMineWnd::OnLRButtonUp(UINT row, UINT col)
{
	UINT rowmin;
	UINT rowmax;
	//�����߽����
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
	//�����߽����
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
	//����Χ�ķ���ָ�ԭ��״̬
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
		//�ػ�ָ������
		CRect rcMines(MINE_AREA_LEFT+m_pMines[rowmin][colmin].uCol*16, MINE_AREA_TOP+m_pMines[rowmin][colmin].uRow*16,
			MINE_AREA_LEFT+m_pMines[rowmax][colmax].uCol*16, MINE_AREA_TOP+m_pMines[rowmax][colmax].uRow*16);
	InvalidateRect(&rcMines, FALSE);
}

UINT CMineWnd::GetAroundFlags(UINT row, UINT col)
{
	UINT rowmin;
	UINT rowmax;
	//�����߽����
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
	//�����߽����
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
	//��ȡ��Χ�ĺ�����
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
	//�����߽����
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
	//�����߽����
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
	//�����Ԥ��������� �򷵻�FALSE ���ҽ�m_pOldMineָ��Ԥ���������
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
