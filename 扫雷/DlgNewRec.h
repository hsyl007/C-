#if !defined(AFX_DLGNEWREC_H__86CD5CFE_07E9_44E0_B377_D8C6C56434C2__INCLUDED_)
#define AFX_DLGNEWREC_H__86CD5CFE_07E9_44E0_B377_D8C6C56434C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNewRec.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgNewRec dialog

class CDlgNewRec : public CDialog
{
// Construction
public:
	CString GetRecHolder();
	CString GetRecCamp();
	void SetLevel(UINT uLevel);
	CDlgNewRec(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgNewRec)
	enum { IDD = IDD_NEWREC };
	CString	m_strCamp;
	CString	m_strHolder;
	CString	m_strLevel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgNewRec)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgNewRec)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNEWREC_H__86CD5CFE_07E9_44E0_B377_D8C6C56434C2__INCLUDED_)
