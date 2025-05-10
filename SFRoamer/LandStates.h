//LandStates.h
#pragma once

#include <sqlite3.h>
#include <string>

class CLandStates
{
//Constructor
public:
	CLandStates();
	~CLandStates();

	//Methods
public:
	int InitialiseLandStatesDatabase();
	void FreeDatabase();
	std::string NameOfLandStates(double x, double y);


//Member variable
private:
	sqlite3* m_LandStatesDB;
};

