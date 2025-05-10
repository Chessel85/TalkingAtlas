// SearchLocationDlg.cpp : implementation file
//

#include "pch.h"
#include "SFRoamer.h"
#include "afxdialogex.h"
#include "SearchLocationDlg.h"
#include "SFRoamerDoc.h" 

constexpr  UINT ID_EDIT_SEARCH_TEXT = 11000;
constexpr UINT ID_LISTBOX_RESULTS = 11001;


// CSearchLocationDlg dialog

IMPLEMENT_DYNAMIC(CSearchLocationDlg, CDialogEx)

CSearchLocationDlg::CSearchLocationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SEARCH_LOCATIONS, pParent)
{

}

CSearchLocationDlg::~CSearchLocationDlg()
{
}

void CSearchLocationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSearchLocationDlg, CDialogEx)
//	ON_BN_CLICKED(IDOK, &CSearchLocationDlg::OnBnClickedOk)
//	ON_WM_CHAR()
ON_BN_CLICKED(IDOK, &CSearchLocationDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSearchLocationDlg message handlers


BOOL CSearchLocationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//Create label and edit box for search text 
	m_lblSearchText.Create(_T("Location name:"), WS_CHILD | WS_VISIBLE, CRect(10, 10, 100, 25), this);
	m_editSearchText.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(10, 30, 100, 45), this, ID_EDIT_SEARCH_TEXT );
	m_editSearchText.SubclassDlgItem(IDD_SEARCH_LOCATIONS, this);
	m_editSearchText.SetParentDlg(this);

	//Label and control for search results listbox 
	m_lblResults.Create(_T("Results:"), WS_CHILD | WS_VISIBLE, CRect(10, 50, 100, 65), this);
	m_lstSearchResults.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP , CRect(10, 70, 100, 200), this, ID_LISTBOX_RESULTS );


	// Set tab order
	::SetWindowPos(m_lblSearchText.GetSafeHwnd(), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	::SetWindowPos(m_editSearchText.GetSafeHwnd(), m_lblSearchText.GetSafeHwnd(), 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	::SetWindowPos(m_lblResults.GetSafeHwnd(), m_editSearchText.GetSafeHwnd(), 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	::SetWindowPos(m_lstSearchResults.GetSafeHwnd(), m_lblResults.GetSafeHwnd(), 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSearchLocationDlg::SetDocument(CSFRoamerDoc* pDoc)
{
	m_pDoc = pDoc;
}

void CSearchLocationDlg::SearchTextChanged(CString rsText )
{
	std::string sText = CT2A(rsText);
	SVector svResults = m_pDoc->SearchForName(sText);

	//Empty the list box
	m_lstSearchResults.ResetContent();

	//Populate listbox from string vector 
	for (const std::string& str : svResults)
	{
		CString entry;
		for (int i = 0; i < str.length(); i++)
			entry = entry + str[i];
		m_lstSearchResults.AddString( entry );
	}
	if (m_lstSearchResults.GetCount() > 0)
		m_lstSearchResults.SetCurSel(0); (1);
}

int CSearchLocationDlg::GetSelectedSearchResult()
{
	return m_ResultsIndex;
}

void CSearchLocationDlg::OnBnClickedOk()
{
	m_ResultsIndex = m_lstSearchResults.GetCurSel();
	CDialogEx::OnOK();
}
