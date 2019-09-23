// Mine.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Mine.h"

#include "MineWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMineApp

BEGIN_MESSAGE_MAP(CMineApp, CWinApp)
	//{{AFX_MSG_MAP(CMineApp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMineApp construction

CMineApp::CMineApp()
{

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMineApp object

CMineApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMineApp initialization

const CString classname = _T("MineClass");
BOOL CMineApp::InitInstance()
{
	//读取保存的设置
	char exeFullPath[MAX_PATH];
	GetModuleFileName(NULL, exeFullPath, MAX_PATH);
	CString strPath(exeFullPath);
	CString configPath =strPath.Left(strPath.ReverseFind('\\'));
	configPath += "\\config.ini";

	m_xMineNum = GetPrivateProfileInt("MINE", "XNUM", 9, configPath);
	m_yMineNum = GetPrivateProfileInt("MINE", "YNUM", 9, configPath);

	m_pMineWnd = new CMineWnd();
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(WNDCLASS));

	wc.hInstance     = AfxGetInstanceHandle();
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpfnWndProc   = ::DefWindowProc;
	wc.hCursor       = LoadCursor(IDC_ARROW);
	wc.hIcon         = LoadIcon(IDR_MAINFRAME);
	wc.lpszClassName = classname;
	wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU);

	if(!AfxRegisterClass(&wc))
	{
		AfxMessageBox("注册窗口失败！");
		return FALSE;
	}

	UINT uWidth = DEFAULT_FRAME_X + m_xMineNum*MINE_WIDTH + LINE_WIDTH_0*3 +
		SIDE_WIDTH_0 + SIDE_WIDTH_1;

	UINT uHeight = DEFAULT_FRAME_Y + m_yMineNum*MINE_HEIGHT + LINE_WIDTH_0*3 +
		SIDE_WIDTH_0*2 + SIDE_WIDTH_1 + SHELL_S_H;

	UINT uxPos = GetSystemMetrics(SM_CXSCREEN)/2 - uWidth/2;

	UINT uyPos = GetSystemMetrics(SM_CYSCREEN)/2 - uHeight/2;

	if(!m_pMineWnd->CreateEx(NULL, classname, _T("扫雷"), WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX,
		uxPos, uyPos, uWidth, uHeight, NULL, NULL))
	{
		AfxMessageBox("创建主窗口失败！");
		return FALSE;
	}
	
	m_pMainWnd = m_pMineWnd;
	m_pMineWnd->ShowWindow(SW_SHOW);
	m_pMineWnd->UpdateWindow();

	return TRUE;
	
}



