#if !defined(AFX_DLGCUSTOM_H__30662A06_2945_4D6F_8867_360A8435EDAF__INCLUDED_)
#define AFX_DLGCUSTOM_H__30662A06_2945_4D6F_8867_360A8435EDAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCustom.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCustom dialog

class CDlgCustom : public CDialog
{
// Construction
public:
	void InitialData(UINT uxNum, UINT uyNum, UINT uMineNum);
	CDlgCustom(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCustom)
	enum { IDD = IDD_DLG_CUSTOM };
	UINT	m_uyNum;
	UINT	m_uxNum;
	UINT	m_uMineNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCustom)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCustom)
	afx_msg void OnOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCUSTOM_H__30662A06_2945_4D6F_8867_360A8435EDAF__INCLUDED_)
