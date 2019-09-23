#if !defined(AFX_DLGNOWREC_H__BF07B1C0_9DA2_4E18_96EB_976C279FB71B__INCLUDED_)
#define AFX_DLGNOWREC_H__BF07B1C0_9DA2_4E18_96EB_976C279FB71B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNowRec.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgNowRec dialog

class CDlgNowRec : public CDialog
{
// Construction
public:
	CDlgNowRec(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgNowRec)
	enum { IDD = IDD_NEWREC };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgNowRec)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgNowRec)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNOWREC_H__BF07B1C0_9DA2_4E18_96EB_976C279FB71B__INCLUDED_)
