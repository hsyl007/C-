// DlgRank.cpp : implementation file
//

#include "stdafx.h"
#include "mine.h"
#include "DlgRank.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRank dialog


CDlgRank::CDlgRank(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRank::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRank)
	m_strAdvCamp = _T("");
	m_strAdvHolder = _T("");
	m_strPriCamp = _T("");
	m_strPriHolder = _T("");
	m_strAdvScore = _T("");
	m_strPriScore = _T("");
	m_strSecScore = _T("");
	m_strSecCamp = _T("");
	m_strSecHolder = _T("");
	//}}AFX_DATA_INIT
}


void CDlgRank::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRank)
	DDX_Text(pDX, IDC_ADV_CAMP, m_strAdvCamp);
	DDX_Text(pDX, IDC_ADV_HOLDER, m_strAdvHolder);
	DDX_Text(pDX, IDC_PRI_CAMP, m_strPriCamp);
	DDX_Text(pDX, IDC_PRI_HOLDER, m_strPriHolder);
	DDX_Text(pDX, IDC_ADV_BEST, m_strAdvScore);
	DDX_Text(pDX, IDC_PRI_BEST, m_strPriScore);
	DDX_Text(pDX, IDC_SEC_BEST, m_strSecScore);
	DDX_Text(pDX, IDC_SEC_CAMP, m_strSecCamp);
	DDX_Text(pDX, IDC_SEC_HOLDER, m_strSecHolder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRank, CDialog)
	//{{AFX_MSG_MAP(CDlgRank)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRank message handlers



void CDlgRank::SetPriRecord(CString camp, CString holder, UINT score)
{
	m_strPriCamp = camp;
	m_strPriHolder = holder;
	m_strPriScore.Format("%d%s", score, "√Î");
}

void CDlgRank::SetSecRecord(CString camp, CString holder, UINT score)
{
	m_strSecCamp = camp;
	m_strSecHolder = holder;
	m_strSecScore.Format("%d%s", score, "√Î");
}

void CDlgRank::SetAdvRecord(CString camp, CString holder, UINT score)
{
	m_strAdvCamp = camp;
	m_strAdvHolder = holder;
	m_strAdvScore.Format("%d%s", score, "√Î");
}
