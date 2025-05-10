 //PlanetData.cpp
#include "pch.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <chrono>
#include <windows.h>
#include <shellapi.h>
#include "PlanetData.h"
#include "SpeechOut.h"

using namespace std;

CPlanetData::CPlanetData()
{
	m_AdminLevel = 1;
	m_Player.SetPosition(23, -30);
	m_CurrentLandDetailsIndex = 0;
	m_CurrentBorderingDetailsIndex  = 0;
	m_LastAction = slaNone;
}


CPlanetData::~CPlanetData()
{
	m_Water.FreeDatabase();
	m_Land.FreeDatabase();
	m_OSMLand.FreeDatabase();	
	m_LandStates.FreeDatabase();
	m_Cities.FreeDatabase();
}

int CPlanetData::InitialiseData()
{
	int status = 0;

	//Set up water polygons
	status = m_Water.InitialiseWaterDatabase();

	//Set up land polygons 
	if (status== 0 )
	{
		status = m_Land.InitialiseLandDatabase();
	}
	if (status == 0)
	{
		status = m_OSMLand.InitialiseOSMLandDatabase();
	}


	//Set up land states polygons 
	if (status == 0)
	{
		status = m_LandStates.InitialiseLandStatesDatabase();
	}

	//Set up cities points 
	if (status == 0)
	{
		status = m_Cities.InitialiseCitiesDatabase();
	}


return status;
}

//Public methods for updating the planet 
void CPlanetData::OnKeyDown(unsigned int  nChar, unsigned int nRepCnt, unsigned int nFlags, int KeyState)
{
	enumKeyProcessing eKP = EKP_NONE;
	int moveX = 0, moveY = 0, nLevel = 0, indexChange = 0;
	std::ostringstream oss;
	std::string s;
	double stepSize = 0;

	//Switch statement to process the key down 
	switch (nChar)
	{
		case VK_LEFT:   //User is moving left 
				moveX = -1;
				eKP = EKP_MOVE;
				break;
	case VK_RIGHT:  //User is moving right 
			moveX = 1;
			eKP = EKP_MOVE;
		break;
	case VK_DOWN:
			moveY = -1;
			eKP = EKP_MOVE;
		break;
	case VK_UP:
			moveY = 1;
			eKP = EKP_MOVE;
		break;
	case 189: //Minus sign on top row to decrease step size 
		ChangeStepSize(-1);
		eKP = EKP_CHANGE_STEP_SIZE;
		break;
	case 187: //plus sign on top row to increase step size or increase cane range 
		ChangeStepSize(1);
		eKP = EKP_CHANGE_STEP_SIZE;
		break;
	case '1':
		nLevel = 1;
		eKP = EKP_ADMIN_LEVEL;
		break;
	case '2':
		nLevel = 2;
		eKP = EKP_ADMIN_LEVEL;
		break;
	case '3':
		nLevel = 3;
		eKP = EKP_ADMIN_LEVEL;
		break;
	case 'X': //Announce position
		m_LastAction = slaNone;
		SpeakPlayerPosition();
		break;
	case 'A': //Announce area of last spoken location 
		m_LastAction = slaNone;
		SpeakLastLandArea();
		break;
	case 'F': //Speak full land place name hierarchy 
		m_LastAction = slaNone;
		SpeakFullLandPlaceNames();
		break;
	case 'S': //Speak nearest points
		if (KeyState == 2) //Shift key is down 
			indexChange = -1;
		else
			indexChange = 1;
		m_LastAction = slaNearby;
		SpeakDetailedNearestPoints( indexChange );
		break;
	case 'B': //Cycle through bordering polygons 
		m_LastAction = slaBorders;
		SpeakBorderingPolygons();
		break;
	case 'M': //Set marker 
		m_LastAction = slaNone;
		m_Marker2 = m_Marker1;
		m_Marker1 = m_Player.GetPosition();
		SpeakMarkers();
		break;
	case 'R': //Repeat last spoken phrase 
		m_Speaker.Repeat();
		break;
	case 'C': //Copy coordinates or last spoken text to to clipboard
		if (KeyState == 2 ) //Shift key down  so copy last spoken text to clipboard 
		{
			oss << m_Speaker.GetLastSpokenText();
		}
		else
		{
			double x, y;
			m_Player.GetPosition(x, y);
			oss << std::fixed << std::setprecision(3) << x << "," << y;
		}
			s = oss.str();
		CopyTextToClipboard( s.c_str());
		m_Speaker.SpeakString("Copied to clipboard");
		break;
	case 'W': //Launch wikipedia page in browser for current item under player
		LaunchWikiPage();
		break;
	case 13: //Enter key hit 
		if (m_LastAction == slaNearby)
		{
			//Move to coordinates in current nearby search result
			MoveToCurrentNearbyPoint();
		}
		else if (m_LastAction == slaBorders)
		{
			//Move to coordinates in current bordering list 
			MoveToCurrentBorderingResult();
		}

		break;
	default:
		eKP = EKP_NONE;
	}

	//Have updated the planet so now render it
	switch (eKP)
	{
	case  EKP_MOVE:
		m_Player.Move(moveX, moveY);
		LocateAndAnnouncePlayerPosition();

		//Clear out info on nearest neighbours  as user has moved so will need rebuilding 
		ClearLists();
		break;
	case EKP_CHANGE_STEP_SIZE:
		stepSize = m_Player.GetStepSize();
		if (stepSize >= 1)
			oss << std::fixed << std::setprecision(0) << stepSize << " kilometres per step";
		else
			oss << std::fixed << std::setprecision(0) << (stepSize * 1000) << " metres per step";
		s = oss.str();
		m_Speaker.SpeakString(s);

		//Clear lists as have changed step size
		ClearLists();

		break;
	case EKP_ADMIN_LEVEL:
		SetAdminLevel(nLevel);
		s = "Level " + std::to_string(nLevel);
		m_Speaker.SpeakString(s);
		break;
	default:
		int k = 0;
	}
}

void CPlanetData::AnnounceNearbyObjects()
{
	std::string sCity = "";
	double x = 0, y = 0, radius = 0.0;
	m_Player.GetPosition(x, y);
	radius = m_Player.GetStepSize() / 111.0;

	sCity = m_Cities.GetNearestCity(x, y, 0.2);
	if( sCity != "" ) 
		m_Speaker.SpeakString(sCity);
}

void CPlanetData::SetPlayerPosition(double x, double y)
{
	m_Player.SetPosition(x, y);
}

void CPlanetData::GetPlayerPosition(double& x, double& y)
{
	m_Player.GetPosition(x, y);
}

//Private methods for rendering
void CPlanetData::SpeakPlayerPosition()
{
	//Get our variables together 
	double x = 0, y = 0;
	string output;
	m_Player.GetPosition(x, y);
	// Create a stringstream object
	stringstream ss;

	//Construct the string stream 
	ss << "You are at " << std::fixed << setprecision(2) << y << " latitude and " << x << " longitude";

	// Extract the formatted string from the stream
	output = ss.str();

	//Speak the output 
	m_Speaker.SpeakString(output);
}

void CPlanetData::SpeakLastLandArea()
{
	//In case nothing in list 
	if (m_LastSpokenLocation.size() == 0)
	{
		m_Speaker.SpeakString("None");
		return;
	}

	nearbyResult landResult = m_LastSpokenLocation.front();
	std::string name = landResult.name;
	double area = landResult.area;

	//Format output
	stringstream ss;
	ss << std::fixed << std::setprecision(0);
	ss << name << " is " << area << "  square kilometres";
	std::string output = ss.str();

		m_Speaker.SpeakString(output);
	}

void CPlanetData::SpeakMarkers()
{
	if (!m_Marker1.IsSet() || !m_Marker2.IsSet())
	{
		m_Speaker.SpeakString("First marker set");
		return;
	}

	//double difference = m_OSMLand.Distance(m_Marker1, m_Marker2);
	double difference = m_Marker1.DistanceTo(m_Marker2);
	double bearing = m_OSMLand.Bearing(m_Marker2, m_Marker1);

	// Format a string using a stringstream 
	stringstream ss;
	ss << "Distance between markers is " << std::fixed << setprecision(2) << difference << " kilometres on a bearing of " << std::fixed << setprecision(1)  << bearing  << " degrees";
std::string 	output = ss.str();

	//Speak the output 
	m_Speaker.SpeakString(output);
}

void CPlanetData::LocateAndAnnouncePlayerPosition()
{
	auto t_start = std::chrono::high_resolution_clock::now();

	bool inWater = false;
	double x = 0.0, y = 0.0;
	m_Player.GetPosition(x, y);

	inWater = !m_Land.IsOnLand( x, y );

	if (inWater)
	{
		m_Speaker.SpeakString( m_Water.NameOfWater(x,y)  );
	}
	else
	{
		SpeakLand(x, y);
	}

	auto t_end = std::chrono::high_resolution_clock::now();
	double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
	TRACE("The value of myDoubleValue is %f\n", elapsed_time_ms);
	int k = 0;
}

SVector CPlanetData::SearchForName(std::string rsText)
{
	m_SearchResultPositions.clear();
	return m_OSMLand.SearchForName(rsText, m_SearchResultPositions );
}

void CPlanetData::MoveToSearchResult(int index)
{
	if (index < m_SearchResultPositions.size())
	{
		CLongLat ll = m_SearchResultPositions[index];
		m_Player.SetPosition(ll.longitude(), ll.latitude());
		ClearLists();
	}
}

void CPlanetData::MoveToCurrentNearbyPoint()
{
	if( m_CurrentLandDetails.size() > 0 )
	{
		nearbyResult nr = m_CurrentLandDetails[m_CurrentLandDetailsIndex];
		m_Player.SetPosition(nr.longitude, nr.latitude);
		ClearLists();
	}
}

void CPlanetData::MoveToCurrentBorderingResult()
{
	if (m_CurrentBorderingDetails.size() > 0)
	{
		nearbyResult nr = m_CurrentBorderingDetails[m_CurrentBorderingDetailsIndex ];
		m_Player.SetPosition(nr.longitude, nr.latitude);
		ClearLists();
	}
}


double CPlanetData::ChangeStepSize(int change)
{
	return m_Player.ChangeStepSize(change);
}

void CPlanetData::SpeakDetailedNearestPoints( int indexChange )
{
	//Refresh info if the current list is empty which indicates player has moved since last request  or changed step size 
	if (m_CurrentLandDetails.size() == 0)
	{
		double x, y;
		m_Player.GetPosition(x, y);
		m_CurrentLandDetails = m_OSMLand.NearestPointsNameAndDetails(x, y, m_Player.GetStepSize() );
		m_CurrentLandDetailsIndex = 0;
	}
	else
	{
		//There was already data so move through the list and wrap the index round to the start if needed 
		m_CurrentLandDetailsIndex += indexChange;
		if (m_CurrentLandDetailsIndex == m_CurrentLandDetails.size())
			m_CurrentLandDetailsIndex = 0;
		else if (m_CurrentLandDetailsIndex == -1)
			m_CurrentLandDetailsIndex = m_CurrentLandDetails.size()-1;
	}

	//Construct a sentence to announce 
	std::string formattedString;
	if (m_CurrentLandDetails.size() == 0)
	{
		formattedString = "Nothing in range.";
	}
	else
	{
		nearbyResult nr = m_CurrentLandDetails[m_CurrentLandDetailsIndex];
		std::stringstream output;

		// Format the name, distance, and bearing 
		output << std::fixed << std::setprecision(1);
		output << nr.name << " is " << nr.distance << "km away at bearing " << nr.bearing << " degrees";

		// Get the formatted string
		formattedString = output.str();
	}

	m_Speaker.SpeakString(formattedString);
}

void CPlanetData::SpeakBorderingPolygons()
{
	//Refresh info if the current list is empty which indicates player has moved since last request 
	if (m_CurrentBorderingDetails.size() == 0)
	{
		double x, y;
		m_Player.GetPosition(x, y);
		m_CurrentBorderingDetails = m_OSMLand.BorderingPolygons(x, y, m_AdminLevel );
		m_CurrentBorderingDetailsIndex = 0;
	}
	else
	{
		//There was already data so move through the list and wrap the index round to the start if needed 
		m_CurrentBorderingDetailsIndex++;
		if (m_CurrentBorderingDetailsIndex == m_CurrentBorderingDetails.size())
			m_CurrentBorderingDetailsIndex = 0;
	}

	//Say the current index 
	std::string sName = m_CurrentBorderingDetails[m_CurrentBorderingDetailsIndex ].name;
	m_Speaker.SpeakString(sName );
}

void CPlanetData::SpeakFullLandPlaceNames()
{
	double x = 0.0, y = 0.0;
	m_Player.GetPosition(x, y);

	NRList slNames = m_OSMLand.FullListOfOSMLand(x, y);

	//Construct what to say from list of strings
	std::string sLand;
	int counter = 0;
	for (NRList::iterator it = slNames.begin(); it != slNames.end(); it++)
	{
		nearbyResult landResult = *it;
		sLand += landResult.name;
		counter++;
		if (counter != slNames.size())
			sLand += ",";
	}

	m_Speaker.SpeakString(sLand);

	//Refresh last spoken cache from temporary copy of what player landed on this time
	m_LastSpokenLocation.clear();
	for (NRList::iterator it = slNames.begin(); it != slNames.end(); it++)
	{
		nearbyResult landResult = *it;
		m_LastSpokenLocation.push_back(landResult);
	}
}

void CPlanetData::SpeakLand(double x, double y)
{
	//Purpose is to get names of relations under current position and filter admin relations that were spoken previously to reduce verbosity 
	//Aim to keep islands always announced 
	NRList slLand = m_OSMLand.NameOfOSMLand(x, y, m_AdminLevel);

	//Make a temporary copy to refresh list after processing 
	NRList tempLand;
	std::copy(slLand.begin(), slLand.end(), std::back_inserter(tempLand));

	//Remove slLand entries if they were returned last time 
	for (NRList::iterator it = slLand.begin(); it != slLand.end();)
	{
		if (FindInLastSpoken(it->name)
			&& it->name.substr(0, 9) != "Island of")
		{
			it = slLand.erase(it);
		}
		else
		{
			++it;
		}
	}

	//In case the lists were identical and so it is now empty push the first entry onto our list for speaking 
	if (slLand.size() == 0)
		slLand.push_back(tempLand.front());

	//Construct what to say from list of strings
	std::string sLand;
	nearbyResult landResult;
	int counter = 0;
	for (NRList::iterator it = slLand.begin(); it != slLand.end(); it++)
	{
		landResult = *it;
		sLand += landResult.name;
		counter++;
		if (counter != slLand.size())
			sLand += ",";
	}

	m_Speaker.SpeakString(sLand);

	//Refresh last spoken cache from temporary copy of what player landed on this time
	m_LastSpokenLocation.clear();
	for (NRList::iterator it = tempLand.begin(); it != tempLand.end(); it++)
	{
		nearbyResult landResult = *it;
		m_LastSpokenLocation.push_back(landResult);
	}
}

void CPlanetData::SetAdminLevel(int nLevel)
{
	m_AdminLevel = nLevel;
	ClearLists();
}

void CPlanetData::CopyTextToClipboard( const char* sText )
{
// Open the clipboard.
HWND hWnd = GetDesktopWindow();
if (OpenClipboard(hWnd)) 
{
	// Empty the clipboard
	EmptyClipboard();

	// Set the clipboard data.
	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, strlen(sText) + 1);
	memcpy(GlobalLock(hMem), sText, strlen(sText) + 1);
	GlobalUnlock(hMem);
	SetClipboardData(CF_TEXT, hMem);

	// Close the clipboard
	CloseClipboard();
}
}



void CPlanetData::ClearLists()
{
	m_CurrentLandDetails.clear();
	m_CurrentLandDetailsIndex = 0;
	m_CurrentBorderingDetails.clear();
	m_CurrentBorderingDetailsIndex = 0;
	}

bool CPlanetData::FindInLastSpoken(std::string vsName)
{
	bool bFound = false;

	for (NRList::iterator it = m_LastSpokenLocation.begin(); it != m_LastSpokenLocation.end(); it++ )
	{
		std::string name = it->name;
		if (name == vsName)
			bFound = true;
	}

	return bFound;
}

void CPlanetData::LaunchWikiPage()
{
	//In case nothing in list 
	if (m_LastSpokenLocation.size() == 0)
	{
		m_Speaker.SpeakString("None");
		return;
	}

	nearbyResult landResult = m_LastSpokenLocation.front();
	std::string sWikiID = m_OSMLand.GetWikipediaValue(landResult.elementType, landResult.elementID);

	//Abort if no wikipedia tag 
	if (sWikiID == "")
	{
		m_Speaker.SpeakString("No wikipedia entry for " + landResult.name);
		return;
	}

	//Convert any spaces to underscores 
	for (size_t i = 0; i < sWikiID.size(); ++i)
	{
		if (sWikiID[i] == ' ')
			sWikiID[i] = '_';
	}

	//Construct the URL 
	std::string sURL = "https://en.wikipedia.org/wiki/" + sWikiID;

	//Now the magic to launch this URL in default browser 
	int wchars_needed = MultiByteToWideChar(CP_UTF8, 0, sURL.c_str(), -1, NULL, 0);
	std::wstring wURL(wchars_needed, 0); // Allocate space for wide characters
	MultiByteToWideChar(CP_UTF8, 0, sURL.c_str(), -1, &wURL[0], wchars_needed);

	SHELLEXECUTEINFO sei = { 0 };
	sei.cbSize = sizeof(sei);
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	sei.lpVerb = L"open";
	sei.lpFile = wURL.c_str(); // Now a wide character string
	sei.nShow = SW_SHOWNORMAL;

	ShellExecuteEx(&sei);
}