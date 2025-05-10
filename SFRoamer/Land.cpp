//Land.cpp
#include "pch.h"
#include "Land.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <chrono>

using namespace std;

CLand::CLand()
{
	m_LandDB = nullptr;
}

CLand::~CLand()
{
}

int CLand::InitialiseLandDatabase()
{
	//Open the Land polygon database 
	int rc = sqlite3_open( "Files/LandPolygons.db", &m_LandDB);
	if (rc != SQLITE_OK) 
	return -1;

	//Give permission to load extensions
	rc = sqlite3_enable_load_extension(m_LandDB, 1);
	if (rc != SQLITE_OK)
		return -1;

	//Create a call to spatial library to load it 
	string sql = "SELECT load_extension('mod_spatialite');";
	char* messageError;
	rc = sqlite3_exec(m_LandDB, sql.c_str(), nullptr, 0, &messageError);
	if (rc != SQLITE_OK)
	{
		cerr << messageError << endl;
		sqlite3_free(messageError);
		return -1;
	}

return 0;
}

void  CLand::FreeDatabase()
{
	sqlite3_close(m_LandDB);
	}

bool CLand::IsOnLand(double x, double y)
{
	//Default value for function return 
	bool bIsOnLand = false;

	//Prepare for the SQLite call 
	sqlite3_stmt* stmt;

	//The query 
	std::string sQuery = GetQueryFromScript( "Scripts/SelectIfOnLand.sql" );
	int rc = sqlite3_prepare_v2(m_LandDB, sQuery.c_str(), -1, &stmt, NULL);
	if (rc != SQLITE_OK)
	{
		cerr << "Prepare statement for testing if on land or not query failed: " << sqlite3_errmsg(m_LandDB) << std::endl;
		sqlite3_close(m_LandDB);
		return false;
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
		int inLand  = sqlite3_column_int( stmt, 0);
		inLand == 1 ? bIsOnLand = 1 : bIsOnLand = false;
	}
	else if (rc != SQLITE_DONE)
	{
		std::cerr << "Step failed: " << sqlite3_errmsg(m_LandDB) << std::endl;
	}

	// Finalize the statement
	sqlite3_finalize(stmt);


	return bIsOnLand;
}

std::string CLand::GetQueryFromScript(std::string rsScriptFilename)
{
	std::ifstream file( rsScriptFilename);
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