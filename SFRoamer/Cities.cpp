//Cities.cpp
#include "pch.h"
#include "Cities.h"
#include <iostream>
#include <string>
#include <sstream>
#include <chrono>

using namespace std;

CCities::CCities()
{
	m_CitiesDB = nullptr;
}

CCities::~CCities()
{
}

int CCities::InitialiseCitiesDatabase()
{
	//Open the Cities points database 
	int rc = sqlite3_open( "Files/Cities.db", &m_CitiesDB);
	if (rc != SQLITE_OK) 
	return -1;

	//Give permission to load extensions
	rc = sqlite3_enable_load_extension(m_CitiesDB, 1);
	if (rc != SQLITE_OK)
		return -1;

	//Create a call to spatial library to load it 
	string sql = "SELECT load_extension('mod_spatialite');";
	char* messageError;
	rc = sqlite3_exec(m_CitiesDB, sql.c_str(), nullptr, 0, &messageError);
	if (rc != SQLITE_OK)
	{
		cerr << messageError << endl;
		sqlite3_free(messageError);
		return -1;
	}

return 0;
}

void  CCities::FreeDatabase()
{
	sqlite3_close(m_CitiesDB);
	}

std::string CCities::GetNearestCity(double x, double y, double radius )
{
	//Default value for function return 
	std::string sCitiesName = "";

	//Prepare for the SQLite call 
	sqlite3_stmt* stmt;

	//The query 
	//const char* sql = "SELECT name, countrycode, capitalcity, population,  ST_Distance( location, MakePoint(?, ?) ) AS distance   FROM spt_cities  WHERE  ST_Intersects( ST_Buffer( MakePoint( ?, ? ),?  ), location )  AND distance < ? ORDER BY distance;";
	const char* sql = "SELECT name, ST_Distance( MakePoint( ?, ? ) , location ) AS distance FROM spt_Cities     WHERE ROWID IN ( SELECT ROWID FROM SpatialIndex WHERE( ( f_table_name = 'spt_Cities') AND( search_frame =  BuildMbr( -?, ?, ?, ?, 4326 ) ) ) ) ORDER BY distance LIMIT 20; ";
	int rc = sqlite3_prepare_v2(m_CitiesDB, sql, -1, &stmt, NULL);
	if (rc != SQLITE_OK)
	{
			cerr << "Prepare statement for cities query failed: " << sqlite3_errmsg(m_CitiesDB) << std::endl;
		sqlite3_close(m_CitiesDB);
		return sCitiesName;
	}

	//Bind values to statement
	sqlite3_bind_double(stmt, 1, x);
	sqlite3_bind_double(stmt, 2, y);
	sqlite3_bind_double(stmt, 3, x-radius);
	sqlite3_bind_double(stmt, 4, y-radius);
	sqlite3_bind_double(stmt, 5, x+radius);
	sqlite3_bind_double(stmt, 6, y+radius);


	// Execute the query
	rc = sqlite3_step(stmt);
	if (rc == SQLITE_ROW)
	{
		const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		sCitiesName = name;
	}
	else if (rc != SQLITE_DONE)
	{
		std::cerr << "Step failed: " << sqlite3_errmsg(m_CitiesDB) << std::endl;
	}

	// Finalize the statement
	sqlite3_finalize(stmt);


	return sCitiesName;
}