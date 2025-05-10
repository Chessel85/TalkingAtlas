//Cities.h
#pragma once

#include <sqlite3.h>
#include <string>

class CCities
{
//Constructor
public:
	CCities();
	~CCities();

	//Methods
public:
	int InitialiseCitiesDatabase();
	void FreeDatabase();
	std::string GetNearestCity(double x, double y, double radius );


//Member variable
private:
	sqlite3* m_CitiesDB;
};

