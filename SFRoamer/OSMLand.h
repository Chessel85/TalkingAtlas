//OSMLand.h
#pragma once

#include "aliases.h"
#include "LongLat.h"
#include <sqlite3.h>
#include <string>

class COSMLand
{
//Constructor
public:
	COSMLand();
	~COSMLand();

	//Methods
public:
	int InitialiseOSMLandDatabase();
	void FreeDatabase();
	NRList NameOfOSMLand(double x, double y, int adminLevel );
	NRList    FullListOfOSMLand(double x, double y);
	NRVector  NearestPointsNameAndDetails(double x, double y, double size );
	SVector NameAndDetails(double x, double y);
	NRVector   BorderingPolygons(double x, double y, int adminLevel );
	double Distance(CLongLat p1, CLongLat p2);
	double Bearing(CLongLat p1, CLongLat p2);
	SVector SearchForName(std::string rsText,  LLVector& rSearchResultPositions );
	std::string  GetWikipediaValue(int type, int ID);


private:
	std::string GetQueryFromScript(std::string rsScriptFilename);
//Member variable
private:
	sqlite3* m_OSMLandDB;
};

