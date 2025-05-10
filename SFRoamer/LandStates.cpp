//LandStates.cpp
#include "pch.h"
#include "LandStates.h"
#include <iostream>
#include <string>
#include <sstream>
#include <chrono>

using namespace std;

CLandStates::CLandStates()
{
	m_LandStatesDB = nullptr;
}

CLandStates::~CLandStates()
{
}

int CLandStates::InitialiseLandStatesDatabase()
{
	//Open the LandStates polygon database 
	int rc = sqlite3_open( "Files/ne_10m_admin_1_states_provinces/NaturalEarthStates.db", &m_LandStatesDB);
	if (rc != SQLITE_OK) 
	return -1;

	//Give permission to load extensions
	rc = sqlite3_enable_load_extension(m_LandStatesDB, 1);
	if (rc != SQLITE_OK)
		return -1;

	//Create a call to spatial library to load it 
	string sql = "SELECT load_extension('mod_spatialite');";
	char* messageError;
	rc = sqlite3_exec(m_LandStatesDB, sql.c_str(), nullptr, 0, &messageError);
	if (rc != SQLITE_OK)
	{
		cerr << messageError << endl;
		sqlite3_free(messageError);
		return -1;
	}

return 0;
}

void  CLandStates::FreeDatabase()
{
	sqlite3_close(m_LandStatesDB);
	}

std::string CLandStates::NameOfLandStates(double x, double y)
{
	//Default value for function return 
	std::string sLandStatesName = "Land";

	//Prepare for the SQLite call 
	sqlite3_stmt* stmt;

	//The query 
	const char* sql = "SELECT name  FROM     spt_LPolygons WHERE     ST_Contains( LPolygon, MakePoint(  ?, ?) );";
	int rc = sqlite3_prepare_v2(m_LandStatesDB, sql, -1, &stmt, NULL);
	if (rc != SQLITE_OK)
	{
		cerr << "Prepare statement for IHO Seas query failed: " << sqlite3_errmsg(m_LandStatesDB) << std::endl;
		sqlite3_close(m_LandStatesDB);
		return sLandStatesName;
	}

	//Bind values to statement
	sqlite3_bind_double(stmt, 1, x);
	sqlite3_bind_double(stmt, 2, y);

	// Execute the query
	rc = sqlite3_step(stmt);
	if (rc == SQLITE_ROW)
	{
		const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		sLandStatesName = name;
	}
	else if (rc != SQLITE_DONE)
	{
		std::cerr << "Step failed: " << sqlite3_errmsg(m_LandStatesDB) << std::endl;
	}

	// Finalize the statement
	sqlite3_finalize(stmt);


	return sLandStatesName;
}