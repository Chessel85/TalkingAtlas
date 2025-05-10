//Water.cpp
#include "pch.h"
#include "Water.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>

using namespace std;

CWater::CWater()
{
	m_WaterDB = nullptr;
}

CWater::~CWater()
{
}

int CWater::InitialiseWaterDatabase()
{
	//Open the water polygon database 
	int rc = sqlite3_open( "Files/water-polygons-split-4326/wp.db", &m_WaterDB);
	if (rc != SQLITE_OK) 
	return -1;

	//Give permission to load extensions
	rc = sqlite3_enable_load_extension(m_WaterDB, 1);
	if (rc != SQLITE_OK)
		return -1;

	//Create a call to spatial library to load it 
	string sql = "SELECT load_extension('mod_spatialite');";
	char* messageError;
	rc = sqlite3_exec(m_WaterDB, sql.c_str(), nullptr, 0, &messageError);
	if (rc != SQLITE_OK)
	{
		cerr << messageError << endl;
		sqlite3_free(messageError);
		return -1;
	}

return 0;
}

void  CWater::FreeDatabase()
{
	sqlite3_close(m_WaterDB);
	}

bool CWater::PointInWater(double x, double y)
{
	bool bInWater = false;
	sqlite3_stmt* stmt = nullptr;
//Get the SQL statement
	std::string sQuery = GetQueryFromScript( "Scripts/SelectSeaNameFromPoint.sql");
	int rc = sqlite3_prepare_v2( m_WaterDB, sQuery.c_str(), -1, &stmt, NULL);
	if (rc != SQLITE_OK) 
	{
		cerr << "Prepare statement failed: " << sqlite3_errmsg(m_WaterDB) << std::endl;
		sqlite3_close(m_WaterDB);
		return 1;
	}

	//Bind values to statement
	sqlite3_bind_double(stmt, 1, x );
	sqlite3_bind_double(stmt, 2, y );
	sqlite3_bind_double(stmt, 3, x);
	sqlite3_bind_double(stmt, 4, y);

	// Execute the query
	rc = sqlite3_step(stmt);
	if (rc == SQLITE_ROW) 
	{
		bInWater = true;
	}
	else if (rc != SQLITE_DONE) 
	{
		std::cerr << "Step failed: " << sqlite3_errmsg(m_WaterDB) << std::endl;
	}

	// Finalize the statement
	sqlite3_finalize(stmt);


	return bInWater;
}


std::string CWater::NameOfWater(double x, double y)
{
	//Default value for function return 
	std::string sSeaName = "Sea";

	//Prepare for the SQLite call 
	sqlite3_stmt* stmt;

	//The query 
	std::string sQuery = GetQueryFromScript("Scripts/SelectSeaNameFromPoint.sql");
	//std::string sQuery = GetQueryFromScript( "Scripts/SelectNearestSeaNameFromPoint.sql");
	int rc = sqlite3_prepare_v2(m_WaterDB, sQuery.c_str(), -1, &stmt, NULL);
	if (rc != SQLITE_OK)
	{
		cerr << "Prepare statement for IHO Seas query failed: " << sqlite3_errmsg(m_WaterDB) << std::endl;
		sqlite3_close(m_WaterDB);
		return sSeaName;
	}

	//Bind values to statement
	sqlite3_bind_double(stmt, 1, x);
	sqlite3_bind_double(stmt, 2, y);
	sqlite3_bind_double(stmt, 3, x);
	sqlite3_bind_double(stmt, 4, y);

	// Execute the query
	rc = sqlite3_step(stmt);
	if (rc == SQLITE_ROW)
	{
		const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		sSeaName = name;
	}
	else if (rc != SQLITE_DONE)
	{
		std::cerr << "Step failed: " << sqlite3_errmsg(m_WaterDB) << std::endl;
	}

	// Finalize the statement
	sqlite3_finalize(stmt);


	return sSeaName;
}

std::string CWater::GetQueryFromScript(std::string rsScriptFilename)
{
	std::ifstream file(rsScriptFilename);
	if (!file.is_open())
	{
		// Handle error if file cannot be opened
		std::cerr << "Error opening file: " << rsScriptFilename << std::endl;
		return "";
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	return buffer.str();

}