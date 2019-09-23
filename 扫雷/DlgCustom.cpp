// DlgCustom.cpp : implementation file
//

#include "stdafx.h"
#include "mine.h"
#include "DlgCustom.h"
#include "MineWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCustom dialog


CDlgCustom::CDlgCustom(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCustom::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCustom)
	m_uyNum = 0;
	m_uxNum = 0;
	m_uMineNum = 0;
	//}}AFX_DATA_INIT
}


void CDlgCustom::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCustom)
	DDX_Text(pDX, IDC_EDIT1, m_uyNum);
	DDV_MinMaxUInt(pDX, m_uyNum, 8, 100);
	DDX_Text(pDX, IDC_EDIT2, m_uxNum);
	DDV_MinMaxUInt(pDX, m_uxNum, 8, 100);
	DDX_Text(pDX, IDC_EDIT3, m_uMineNum);
	DDV_MinMaxUInt(pDX, m_uMineNum, 1, 1000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCustom, CDialog)
	//{{AFX_MSG_MAP(CDlgCustom)
	ON_BN_CLICKED(IDC_OK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCustom message handlers

void CDlgCustom::OnOk() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_uMineNum > m_uxNum*m_uyNum)
	{
		MessageBox("ÄáÂê,¿Óµù°¡", "-.-", MB_OK);
		return;
	}
	CMineWnd* pMineWnd = (CMineWnd*)AfxGetMainWnd();
	pMineWnd->SetDataFromCustom(m_uxNum, m_uyNum, m_uMineNum);
	CDialog::OnOK();
}

void CDlgCustom::InitialData(UINT uxNum, UINT uyNum, UINT uMineNum)
{
	m_uxNum = uxNum;
	m_uyNum = uyNum;
	m_uMineNum = uMineNum;
}
