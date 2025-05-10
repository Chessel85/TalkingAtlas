// SFRoamerView.cpp : implementation of the CSFRoamerView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SFRoamer.h"
#endif

#include "SFRoamerDoc.h"
#include "SFRoamerView.h"
#include "GoLocationDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSFRoamerView

IMPLEMENT_DYNCREATE(CSFRoamerView, CView)

BEGIN_MESSAGE_MAP(CSFRoamerView, CView)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_EDIT_GOTOLATITUDE, &CSFRoamerView::OnEditGoToLatLo)
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

// CSFRoamerView construction/destruction

CSFRoamerView::CSFRoamerView() noexcept
{
	// TODO: add construction code here

}

CSFRoamerView::~CSFRoamerView()
{
}

BOOL CSFRoamerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CSFRoamerView drawing

void CSFRoamerView::OnDraw(CDC* /*pDC*/)
{
	CSFRoamerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CSFRoamerView diagnostics

#ifdef _DEBUG
void CSFRoamerView::AssertValid() const
{
	CView::AssertValid();
}

void CSFRoamerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSFRoamerDoc* CSFRoamerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSFRoamerDoc)));
	return (CSFRoamerDoc*)m_pDocument;
}
#endif //_DEBUG


// CSFRoamerView message handlers


void CSFRoamerView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	short nShift = GetKeyState(VK_SHIFT);
	short nControl = GetKeyState(VK_CONTROL);
	int myKeyState = 0;
	if (nShift & 0x8000)
		myKeyState |= 2;
	if (nControl & 0x8000)
		myKeyState |= 1;

	CSFRoamerDoc* pDoc = GetDocument();

	pDoc->OnKeyDown(nChar, nRepCnt, nFlags, myKeyState);


	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CSFRoamerView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CSFRoamerDoc* pDoc = GetDocument();
	pDoc->InitialiseDocument();

}


void CSFRoamerView::OnEditGoToLatLo()
{
	//Display the go to location dialogue box
	CGoLocationDlg dlg;

	//Get current coordinates
	double x, y;
	CSFRoamerDoc* pDoc = (CSFRoamerDoc*)GetDocument();
	pDoc->GetPlayerPosition(x, y);

	//Set the dialogue variables
	dlg.SetLatitude(y);
	dlg.SetLongitude(x);


	// Display the dialog modally (waits for user input)
	int result = dlg.DoModal();

	// Check if the user clicked OK
	if (result == IDOK)
	{
		double latitude = dlg.GetLatitude();
		double longitude = dlg.GetLongitude();
		CSFRoamerDoc* pDoc = GetDocument();
		pDoc->SetPlayerPosition(longitude, latitude);
	}
}


void CSFRoamerView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);

	CSFRoamerDoc* pDoc = GetDocument();

}
