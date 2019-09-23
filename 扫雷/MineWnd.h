#if !defined(AFX_MINEWND_H__170F5015_C615_4FD9_AECD_37F27215F408__INCLUDED_)
#define AFX_MINEWND_H__170F5015_C615_4FD9_AECD_37F27215F408__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MineWnd.h : header file
//

typedef	struct
{
	UINT uRow;//����������
	UINT uCol;//����������
	UINT uState;//��������״̬
	UINT uAttrib;//��������
	UINT uOldState;//����ԭ��״̬
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
	BOOL OpenAround(UINT row, UINT col);//����Χδ�������ķ��� ���˲����Ԥ�� ����к���Ԥ������ ����FALSE ����ΪTRUE
	UINT GetAroundFlags(UINT row, UINT col);//�õ���Χ�ĺ�����
	void OnLRButtonUp(UINT row, UINT col);//�ָ��Ź����״̬
	void OnLRButtonDown(UINT row, UINT col);//���¾Ź���
	BOOL SaveConfig();//�����������¼
	BOOL LoadConfig();//�����������¼
	void GetMineRect(UINT row, UINT col, CRect* rect);//ͨ���к��еõ���Ӧ�׷���ľ���
	void SetCheckMark();//�����Ӳ˵����
	void SetCheckColor();//������ɫ�Ӳ˵����
	void SetCheckSound();//���������Ӳ˵����
	void SetCheckLevel();//�����Ѷ��Ӳ˵����
	BOOL FreeSoundRsc();//�ͷ�������Դ
	BOOL LoadSoundRsc();//����������Դ
	void Dead(UINT row, UINT col);//���� ����row col����ʾ��ը
	void ExpandMines(UINT row, UINT col);//��չ��Χ�׷���
	void DrawDownNum(MINEWND* pMine, UINT num);//��pMine��������Χ����num
	BOOL Victory();//�ж��Ƿ�ʤ�� ���ҽ���ʤ������
	BOOL IsMine(UINT row, UINT col);//�ж�row col���Ƿ�����
	UINT GetAroundNum(UINT row, UINT col);//�õ���Χ�׵�����
	MINEWND* GetMine(UINT x, UINT y);//ͨ��������ֵ�õ��ô��׷����ָ��
	void DrawNumberArea(CPaintDC& dc);//����ʱ�������ʣ����������
	void DrawShellArea(CPaintDC& dc);//����3D���
	void DrawButtonArea(CPaintDC& dc);//����Ц������
	void SetDataFromCustom(UINT uxNum, UINT uyNum, UINT uMineNum);//���û����õ��Զ����ѶȶԻ����ȡ���ݲ�����
	void DrawMineArea(CPaintDC& dc);//��������
	void LayMine(UINT row, UINT col);//���õ��� ���ұ�֤row col����һ�ε��µĵط�������
	void SizeWindow();//����m_xNum m_yNum���µ������ڴ�С
	BOOL LoadBitmap();//����λͼ��Դ
	void InitGame();//��ʼ����Ϸ
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
	BOOL m_bLRBtnDown;//��ʶ�Ƿ����Ҽ�ͬʱ����
	CString m_strAdvCamp;//�߼���¼����
	CString m_strAdvHolder;//�߼���¼������
	CString m_strSecCamp;//�м���¼����
	CString m_strSecHolder;//�м���¼������
	CString m_strPriCamp;//������¼����
	CString m_strPriHolder;//������¼������
	CMenu* m_pSubMenu;//"��Ϸ"�Ӳ˵� ��������ѡ���ʶ
	void* m_pSoundVictory;//ʤ������
	void* m_pSoundDead;//ʧ������
	void* m_pSoundClock;//ʱ������
	UINT m_uGameLevel;//��Ϸ�ȼ�
	UINT m_uAdvScore;//�߼���߷���
	UINT m_uSecScore;//�м���߷���
	UINT m_uPriScore;//������߷���
	BOOL m_bBtnClick;//��ʶ��갴��ʱ �Ƿ�����Ц��
	MINEWND* m_pOldMine;//������ƶ�ʱ ��¼��һ�̵ķ���
	MINEWND* m_pNewMine;//��¼��ǰ���λ�����ڷ���
	CRect m_rcMineArea;//������
	CRect m_rcBtn;//Ц������
	UINT m_uIdTimer;//��ʱ��id �����жϵ�ǰ�Ƿ��ж�ʱ��
	CRect m_rcTime;//ʱ������
	CRect m_rcLeaNum;//ʣ����������
	CBrush m_brBG;//����ɫ
	CRect m_rcClient;//�ͻ�������
	MINEWND m_pMines[100][100];//��������������ģ
	UINT m_uyNum;//x����������
	UINT m_uxNum;//y����������
	UINT m_uBtnState;//��ǰЦ��״̬
	CBitmap m_bmpBtn;//Ц��λͼ
	CBitmap m_bmpMine;//�׷���λͼ
	CBitmap m_bmpNum;//����λͼ
	BOOL m_bSoundful;//�Ƿ�������
	BOOL m_bColorful;//�Ƿ�����ɫ
	UINT m_uLeaveNum;//��ǰʣ������
	UINT m_uGameState;//��ǰ��Ϸ״̬
	UINT m_uSpendTime;//��ǰ����ʱ��
	UINT m_uMineNum;//��ǰ������

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MINEWND_H__170F5015_C615_4FD9_AECD_37F27215F408__INCLUDED_)
