// SFRoamerDoc.cpp : implementation of the CSFRoamerDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SFRoamer.h"
#endif

#include "SFRoamerDoc.h"
#include "SearchLocationDlg.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSFRoamerDoc

IMPLEMENT_DYNCREATE(CSFRoamerDoc, CDocument)

BEGIN_MESSAGE_MAP(CSFRoamerDoc, CDocument)
	ON_COMMAND(ID_EDIT_SEARCHFORLOCATION, &CSFRoamerDoc::OnEditSearchforlocation)
END_MESSAGE_MAP()


// CSFRoamerDoc construction/destruction

CSFRoamerDoc::CSFRoamerDoc() noexcept
{
	// TODO: add one-time construction code here

}

CSFRoamerDoc::~CSFRoamerDoc()
{
}

BOOL CSFRoamerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CSFRoamerDoc serialization

void CSFRoamerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CSFRoamerDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CSFRoamerDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CSFRoamerDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CSFRoamerDoc diagnostics

#ifdef _DEBUG
void CSFRoamerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSFRoamerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSFRoamerDoc commands
int CSFRoamerDoc::InitialiseDocument()
{
	int status = 0;

status = 	m_PlanetData.InitialiseData();
	
return status;
}
void CSFRoamerDoc::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags, int KeyState)
{
	int k = 0;
	int nnChar = nChar;
	int nnRepsCount = nRepCnt;
	int nnFlags = nFlags;
	m_PlanetData.OnKeyDown(nnChar, nnRepsCount, nnFlags, KeyState);
}

void CSFRoamerDoc::AnnounceNearbyObjects()
{
	m_PlanetData.AnnounceNearbyObjects();
}

void CSFRoamerDoc::SetPlayerPosition(double x, double y)
{
	m_PlanetData.SetPlayerPosition(x, y);
}

void CSFRoamerDoc::GetPlayerPosition(double& x, double& y)
{
	m_PlanetData.GetPlayerPosition(x, y);
}


void CSFRoamerDoc::SpeakPlace()
{
	m_PlanetData.LocateAndAnnouncePlayerPosition();
}

SVector CSFRoamerDoc::SearchForName(std::string rsText)
{
	return m_PlanetData.SearchForName(rsText);
}

void CSFRoamerDoc::MoveToSearchResult(int index)
{
	m_PlanetData.MoveToSearchResult(index);
}

void CSFRoamerDoc::OnEditSearchforlocation()
{
	//Display the search dialog box
	CSearchLocationDlg dlg;
	dlg.SetDocument(this);

	// Display the dialog modally (waits for user input)
	int result = dlg.DoModal();
	if (result == IDOK)
	{
		int index = dlg.GetSelectedSearchResult();
		m_PlanetData.MoveToSearchResult(index);
	}
}
