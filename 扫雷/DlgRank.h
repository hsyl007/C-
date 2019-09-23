#if !defined(AFX_DLGRANK_H__82002732_44DA_448F_AD93_D7D265895FC3__INCLUDED_)
#define AFX_DLGRANK_H__82002732_44DA_448F_AD93_D7D265895FC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRank.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgRank dialog

class CDlgRank : public CDialog
{
// Construction
public:
	void SetAdvRecord(CString camp, CString holder, UINT score);
	void SetSecRecord(CString camp,CString holder,UINT score);
	void SetPriRecord(CString camp, CString holder, UINT score);
	CDlgRank(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgRank)
	enum { IDD = IDD_RANK };
	CString	m_strAdvCamp;
	CString	m_strAdvHolder;
	CString	m_strPriCamp;
	CString	m_strPriHolder;
	CString	m_strAdvScore;
	CString	m_strPriScore;
	CString	m_strSecScore;
	CString	m_strSecCamp;
	CString	m_strSecHolder;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRank)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRank)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGRANK_H__82002732_44DA_448F_AD93_D7D265895FC3__INCLUDED_)
