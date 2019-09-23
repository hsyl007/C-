// DlgNowRec.cpp : implementation file
//

#include "stdafx.h"
#include "mine.h"
#include "DlgNowRec.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgNowRec dialog


CDlgNowRec::CDlgNowRec(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNowRec::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgNowRec)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgNowRec::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgNowRec)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNowRec, CDialog)
	//{{AFX_MSG_MAP(CDlgNowRec)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNowRec message handlers
