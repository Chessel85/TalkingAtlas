
// SFRoamerView.h : interface of the CSFRoamerView class
//

#pragma once


class CSFRoamerView : public CView
{
protected: // create from serialization only
	CSFRoamerView() noexcept;
	DECLARE_DYNCREATE(CSFRoamerView)

// Attributes
public:
	CSFRoamerDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CSFRoamerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnInitialUpdate();
	afx_msg void OnEditGoToLatLo();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};

#ifndef _DEBUG  // debug version in SFRoamerView.cpp
inline CSFRoamerDoc* CSFRoamerView::GetDocument() const
   { return reinterpret_cast<CSFRoamerDoc*>(m_pDocument); }
#endif

