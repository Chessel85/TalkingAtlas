#pragma once
#include <afxwin.h>

class CSearchLocationDlg;


class CChangeEdit : public CEdit
{
//Methods 
public:
    void SetParentDlg(CSearchLocationDlg* pDlg);
    //Attributes
private:
    CSearchLocationDlg* m_pParentDlg;

    DECLARE_MESSAGE_MAP()
        afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};

