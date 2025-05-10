//IntelliLand.h
#pragma once
#include "OSMLand.h"


class CIntelliLand
{
//Constructor
public:
	CIntelliLand();
	~CIntelliLand();

	//Methods
public:
	int InitialiseOSMLandDatabase();
	void FreeDatabase();
	NRList   NameOfOSMLand(double x, double y, int adminLevel);
	NRList   FullListOfOSMLand(double x, double y);
	NRVector  NearestPointsNameAndDetails(double x, double y, double size);
	SVector NameAndDetails(double x, double y);
	NRVector   BorderingPolygons(double x, double y, int adminLevel);
	double Distance(CLongLat p1, CLongLat p2);
	double Bearing(CLongLat p1, CLongLat p2);
	SVector SearchForName(std::string rsText, LLVector& rSearchResultPositions);
	std::string  GetWikipediaValue(int type, int ID);


//Attributes
private:
	COSMLand m_OSMLand;
};

