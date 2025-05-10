//Water.h
#pragma once

#include <sqlite3.h>
#include <string>

class CWater
{
//Constructor
public:
	CWater();
	~CWater();

	//Methods
public:
	int InitialiseWaterDatabase();
	void FreeDatabase();
	bool PointInWater(double x, double y );
	std::string NameOfWater(double x, double y);

private:
	std::string GetQueryFromScript(std::string rsScriptFilename);

//Member variable
private:
	sqlite3* m_WaterDB;
};

