#if !defined(AFX_MINEWND_H__170F5015_C615_4FD9_AECD_37F27215F408__INCLUDED_)
#define AFX_MINEWND_H__170F5015_C615_4FD9_AECD_37F27215F408__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MineWnd.h : header file
//

typedef	struct
{
	UINT uRow;//方块所在行
	UINT uCol;//方块所在列
	UINT uState;//方块现在状态
	UINT uAttrib;//方块属性
	UINT uOldState;//方块原本状态
}MINEWND;
/////////////////////////////////////////////////////////////////////////////
// CMineWnd window

class CMineWnd : public CWnd
{
// Construction
public:
	CMineWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMineWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL OpenAround(UINT row, UINT col);//打开周围未被翻开的方块 并核查红旗预估 如果有红旗预估错误 返回FALSE 否则为TRUE
	UINT GetAroundFlags(UINT row, UINT col);//得到周围的红旗数
	void OnLRButtonUp(UINT row, UINT col);//恢复九宫格的状态
	void OnLRButtonDown(UINT row, UINT col);//按下九宫格
	BOOL SaveConfig();//保存设置与记录
	BOOL LoadConfig();//加载设置与记录
	void GetMineRect(UINT row, UINT col, CRect* rect);//通过行和列得到对应雷方块的矩形
	void SetCheckMark();//设置子菜单标记
	void SetCheckColor();//设置颜色子菜单标记
	void SetCheckSound();//设置声音子菜单标记
	void SetCheckLevel();//设置难度子菜单标记
	BOOL FreeSoundRsc();//释放声音资源
	BOOL LoadSoundRsc();//加载声音资源
	void Dead(UINT row, UINT col);//死亡 并在row col处显示爆炸
	void ExpandMines(UINT row, UINT col);//扩展周围雷方块
	void DrawDownNum(MINEWND* pMine, UINT num);//在pMine处画下周围雷数num
	BOOL Victory();//判断是否胜利 并且进行胜利处理
	BOOL IsMine(UINT row, UINT col);//判断row col处是否是雷
	UINT GetAroundNum(UINT row, UINT col);//得到周围雷的数量
	MINEWND* GetMine(UINT x, UINT y);//通过点坐标值得到该处雷方块的指针
	void DrawNumberArea(CPaintDC& dc);//绘制时间区域和剩余雷数区域
	void DrawShellArea(CPaintDC& dc);//绘制3D外壳
	void DrawButtonArea(CPaintDC& dc);//绘制笑脸区域
	void SetDataFromCustom(UINT uxNum, UINT uyNum, UINT uMineNum);//从用户设置的自定义难度对话框获取数据并设置
	void DrawMineArea(CPaintDC& dc);//绘制雷区
	void LayMine(UINT row, UINT col);//放置地雷 并且保证row col即第一次点下的地方不是雷
	void SizeWindow();//根据m_xNum m_yNum重新调整窗口大小
	BOOL LoadBitmap();//加载位图资源
	void InitGame();//初始化游戏
	virtual ~CMineWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMineWnd)
	afx_msg void OnAbout();
	afx_msg void OnHelp();
	afx_msg void OnMenuColor();
	afx_msg void OnMenuSound();
	afx_msg void OnLevelPrimary();
	afx_msg void OnLevelSecondary();
	afx_msg void OnLevelAdvance();
	afx_msg void OnLevelCustom();
	afx_msg void OnPaint();
	afx_msg void OnMenuStart();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnInitMenu(CMenu* pMenu);
	afx_msg void OnMenuExit();
	afx_msg void OnClose();
	afx_msg void OnMenuRank();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bLRBtnDown;//标识是否左右键同时按下
	CString m_strAdvCamp;//高级记录门派
	CString m_strAdvHolder;//高级记录保持者
	CString m_strSecCamp;//中级记录门派
	CString m_strSecHolder;//中级记录保持着
	CString m_strPriCamp;//初级记录门派
	CString m_strPriHolder;//初级记录保持者
	CMenu* m_pSubMenu;//"游戏"子菜单 用于设置选择标识
	void* m_pSoundVictory;//胜利声音
	void* m_pSoundDead;//失败声音
	void* m_pSoundClock;//时间声音
	UINT m_uGameLevel;//游戏等级
	UINT m_uAdvScore;//高级最高分数
	UINT m_uSecScore;//中级最高分数
	UINT m_uPriScore;//初级最高分数
	BOOL m_bBtnClick;//标识鼠标按下时 是否点击的笑脸
	MINEWND* m_pOldMine;//当鼠标移动时 记录上一刻的方块
	MINEWND* m_pNewMine;//记录当前鼠标位置所在方块
	CRect m_rcMineArea;//雷区域
	CRect m_rcBtn;//笑脸区域
	UINT m_uIdTimer;//定时器id 用于判断当前是否有定时器
	CRect m_rcTime;//时间区域
	CRect m_rcLeaNum;//剩余雷数区域
	CBrush m_brBG;//背景色
	CRect m_rcClient;//客户区区域
	MINEWND m_pMines[100][100];//允许最大的雷区规模
	UINT m_uyNum;//x方向雷数量
	UINT m_uxNum;//y方向雷数量
	UINT m_uBtnState;//当前笑脸状态
	CBitmap m_bmpBtn;//笑脸位图
	CBitmap m_bmpMine;//雷方块位图
	CBitmap m_bmpNum;//数字位图
	BOOL m_bSoundful;//是否有声音
	BOOL m_bColorful;//是否有颜色
	UINT m_uLeaveNum;//当前剩余雷数
	UINT m_uGameState;//当前游戏状态
	UINT m_uSpendTime;//当前所花时间
	UINT m_uMineNum;//当前雷总数

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MINEWND_H__170F5015_C615_4FD9_AECD_37F27215F408__INCLUDED_)
