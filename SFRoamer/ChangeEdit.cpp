#include "pch.h"
#include "ChangeEdit.h"
#include "SearchLocationDlg.h"
//#include "stdafx.h"
#include "SFRoamerDoc.h" // Replace with your actual document class name
#include "SFRoamer.h" // Replace with your actual application class name

BEGIN_MESSAGE_MAP(CChangeEdit, CEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()


void CChangeEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//Call base class before advising parent dialogue something has changed 
	CEdit::OnChar(nChar, nRepCnt, nFlags);

	//Get text of edit box
	CString strText;
	GetWindowText(strText);
	if (strText.GetLength() > 3)
	{
		m_pParentDlg->SearchTextChanged(strText );
		//CSFRoamerApp* pApp = dynamic_cast<CSFRoamerApp*>(AfxGetApp());
		//CSFRoamerView* pView = pApp->GetMainWnd()->Get;
		//CSFRoamerDoc* pDoc = (CSFRoamerDoc*)pApp->Get
	}
}

void CChangeEdit::SetParentDlg(CSearchLocationDlg* pDlg)
{
	m_pParentDlg = pDlg;
}