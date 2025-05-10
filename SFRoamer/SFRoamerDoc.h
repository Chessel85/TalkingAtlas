// SFRoamerDoc.h : interface of the CSFRoamerDoc class
//


#pragma once

#include "PlanetData.h"
#include "aliases.h"

class CSFRoamerDoc : public CDocument
{
protected: // create from serialization only
	CSFRoamerDoc() noexcept;
	DECLARE_DYNCREATE(CSFRoamerDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CSFRoamerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

	//Private variable
private:
	CPlanetData m_PlanetData;
	//Public methods
public:
	int InitialiseDocument();
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags, int KeyState);
	void AnnounceNearbyObjects();
	void SetPlayerPosition(double x, double y);
	void GetPlayerPosition(double& x, double& y);
	void SpeakPlace();
	SVector SearchForName(std::string rsText);
	void MoveToSearchResult(int index);


	afx_msg void OnEditSearchforlocation();
};
