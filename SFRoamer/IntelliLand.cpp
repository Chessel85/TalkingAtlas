//IntelliLand.cpp
#include "pch.h"
#include "IntelliLand.h"

CIntelliLand::CIntelliLand()
{
}

CIntelliLand::~CIntelliLand()
{
}

int CIntelliLand::InitialiseOSMLandDatabase()
{
	return m_OSMLand.InitialiseOSMLandDatabase();
}

void CIntelliLand::FreeDatabase()
{
	m_OSMLand.FreeDatabase();
}

NRList CIntelliLand::NameOfOSMLand(double x, double y, int adminLevel)
{
	nearbyResult nrDefault;
	nrDefault.name = "Land";
	NRList landResults;

	while( landResults.size() == 0 && adminLevel > 0 )
		landResults = m_OSMLand.NameOfOSMLand(x, y, adminLevel-- );

	//Return something
	if (landResults.size() == 0)
		landResults.push_back(nrDefault);

	return landResults;
}

NRList    CIntelliLand::FullListOfOSMLand(double x, double y)
{
	nearbyResult nrDefault;
	nrDefault.name = "Land";
	NRList landResults;

	landResults = m_OSMLand.FullListOfOSMLand(x, y);

	//Return something
	if (landResults.size() == 0)
		landResults.push_back(nrDefault);

	return  landResults;
}

NRVector  CIntelliLand::NearestPointsNameAndDetails(double x, double y, double size)
{
	return m_OSMLand.NearestPointsNameAndDetails(x, y, size);
}

SVector CIntelliLand::NameAndDetails(double x, double y)
{
	return m_OSMLand.NameAndDetails(x, y);
}

NRVector   CIntelliLand::BorderingPolygons(double x, double y, int adminLevel)
{
	NRVector nrvBorders;

	nrvBorders = m_OSMLand.BorderingPolygons(x, y, adminLevel);
	if (nrvBorders.size() == 0)
	{
		nearbyResult borderResult;
		borderResult.name = "None";
		nrvBorders.push_back(borderResult);
	}

	return nrvBorders;
}

double CIntelliLand::Distance(CLongLat p1, CLongLat p2)
{
	return m_OSMLand.Distance(p1, p2);
}

double CIntelliLand::Bearing(CLongLat p1, CLongLat p2)
{
	return m_OSMLand.Bearing(p1, p2);
}

SVector CIntelliLand::SearchForName(std::string rsText, LLVector& rSearchResultPositions)
{
	return m_OSMLand.SearchForName(rsText, rSearchResultPositions);
}


std::string CIntelliLand::GetWikipediaValue(int type, int ID)
{
	return m_OSMLand.GetWikipediaValue(type, ID);
}