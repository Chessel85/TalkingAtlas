//PlanetData.h
#pragma once

#include "aliases.h"
#include "Player.h"
#include "Water.h"
#include "Land.h"
#include "IntelliLand.h"
#include "LandStates.h"
#include "Cities.h"
#include "SpeechOut.h"

//Define some enums to help with keystroke processing to indicate what action to take
enum enumKeyProcessing
{
	EKP_NONE = 0,
	EKP_MOVE,
	EKP_CANE_SWEEP,
	EKP_CANE_DISTANCE,
	EKP_CHANGE_CANE_RANGE,
	EKP_ADMIN_LEVEL,
	EKP_CHANGE_STEP_SIZE,
	EKP_CHANGE_LAER
};

enum stateLastAction
{
	slaNone = 0,
	slaMoved,
	slaNearby,
	slaBorders
};

class CPlanetData
{
	//Constructor
public:
	CPlanetData();
	~CPlanetData();

	//Methods
public:
	int InitialiseData();
	void OnKeyDown(unsigned int  nChar, unsigned int nRepCnt, unsigned int nFlags, int KeyState);
	void AnnounceNearbyObjects();
	void SetPlayerPosition(double x, double y);
	void GetPlayerPosition(double& x, double& y);
	void LocateAndAnnouncePlayerPosition();
	SVector SearchForName(std::string rsText);
	void MoveToSearchResult(int index);
	void MoveToCurrentNearbyPoint();
	void MoveToCurrentBorderingResult();


//Private methods related to rendering output
private:
	void SpeakPlayerPosition();
	void SpeakLastLandArea();
	void SpeakMarkers();
	double ChangeStepSize(int change);
	void SpeakFullLandPlaceNames();
	void SpeakLand(double x, double y);
	void SpeakDetailedNearestPoints( int indexChange );
	void SpeakBorderingPolygons();
	void SetAdminLevel(int nLevel);
	void CopyTextToClipboard(const char* pText);
	void LaunchWikiPage();
	void ClearLists();
	bool FindInLastSpoken(std::string vsName);

	//Attributes
private:
	CPlayer m_Player;
	CWater m_Water;
	CLand m_Land;
	CIntelliLand m_OSMLand;
	CLandStates m_LandStates;
	CCities m_Cities;

	//User interface variables 
	int m_AdminLevel;
		CLongLat m_Marker1;
	CLongLat m_Marker2;
	LLVector m_SearchResultPositions;
	NRList m_LastSpokenLocation;
	NRVector  m_CurrentLandDetails;
	int m_CurrentLandDetailsIndex;
	NRVector   m_CurrentBorderingDetails;
	int m_CurrentBorderingDetailsIndex;
	stateLastAction m_LastAction;
	CSpeechOut m_Speaker;
};

