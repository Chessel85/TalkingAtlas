//Land.h
#pragma once

#include <sqlite3.h>
#include <string>

class CLand
{
//Constructor
public:
	CLand();
	~CLand();

	//Methods
public:
	int InitialiseLandDatabase();
	void FreeDatabase();
	bool IsOnLand(double x, double y);

private:
	std::string GetQueryFromScript(std::string rsScriptFilename);


//Member variable
private:
	sqlite3* m_LandDB;
};

