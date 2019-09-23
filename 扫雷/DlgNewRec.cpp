// DlgNewRec.cpp : implementation file
//

#include "stdafx.h"
#include "mine.h"
#include "DlgNewRec.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgNewRec dialog


CDlgNewRec::CDlgNewRec(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNewRec::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgNewRec)
	m_strCamp = _T("");
	m_strHolder = _T("");
	m_strLevel = _T("");
	//}}AFX_DATA_INIT
}


void CDlgNewRec::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgNewRec)
	DDX_Text(pDX, IDC_EDIT_CAMP, m_strCamp);
	DDX_Text(pDX, IDC_EDIT_HOLDER, m_strHolder);
	DDX_Text(pDX, IDC_LEVEL, m_strLevel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNewRec, CDialog)
	//{{AFX_MSG_MAP(CDlgNewRec)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNewRec message handlers

void CDlgNewRec::SetLevel(UINT uLevel)
{
	switch(uLevel)
	{
	case LEVEL_PRIMARY:
		m_strLevel = "初级记录:";
		break;
	case LEVEL_SECONDARY:
		m_strLevel = "中级记录:";
		break;
	case LEVEL_ADVANCE:
		m_strLevel = "高级记录:";
		break;
	default:
		break;
	}
}

CString CDlgNewRec::GetRecCamp()
{
	return m_strCamp;
}

CString CDlgNewRec::GetRecHolder()
{
	return m_strHolder;
}
