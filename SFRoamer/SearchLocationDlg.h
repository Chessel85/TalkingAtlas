#pragma once
#include "afxdialogex.h"
#include "ChangeEdit.h"


// CSearchLocationDlg dialog
class CSFRoamerDoc;

class CSearchLocationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSearchLocationDlg)

public:
	CSearchLocationDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CSearchLocationDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SEARCH_LOCATIONS };
#endif

//Methods
public:
	void SetDocument(CSFRoamerDoc* pDoc);
	void SearchTextChanged(CString rsText );
	int GetSelectedSearchResult();


	//Attributes
private:
	CChangeEdit m_editSearchText;
	CListBox m_lstSearchResults;
	CStatic m_lblSearchText;
	CStatic m_lblResults;
	CSFRoamerDoc* m_pDoc;
	int m_ResultsIndex;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
//	afx_msg void OnBnClickedOk();
//	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnBnClickedOk();
};
