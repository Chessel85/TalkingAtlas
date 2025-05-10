//OSMLand.cpp
#include "pch.h"
#include "OSMLand.h"
#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <math.h>

using namespace std;

COSMLand::COSMLand()
{
	m_OSMLandDB = nullptr;
}

COSMLand::~COSMLand()
{
}

int COSMLand::InitialiseOSMLandDatabase()
{
	//Open the OSMLand polygon database 
	int rc = sqlite3_open("Files/OSMLand.db", &m_OSMLandDB);
	if (rc != SQLITE_OK) 
	return -1;

	//Give permission to load extensions
	rc = sqlite3_enable_load_extension(m_OSMLandDB, 1);
	if (rc != SQLITE_OK)
		return -1;

	//Create a call to spatial library to load it 
	string sql = "SELECT load_extension('mod_spatialite');";
	char* messageError;
	rc = sqlite3_exec(m_OSMLandDB, sql.c_str(), nullptr, 0, &messageError);
	if (rc != SQLITE_OK)
	{
		cerr << messageError << endl;
		sqlite3_free(messageError);
		return -1;
	}

return 0;
}

void  COSMLand::FreeDatabase()
{
	sqlite3_close(m_OSMLandDB);
	}

NRList COSMLand::NameOfOSMLand(double x, double y, int adminLevel )
{
	NRList slLand;


	//Prepare for the SQLite call 
	sqlite3_stmt* stmt;

	//The query 
	std::string sQuery = GetQueryFromScript("Scripts/SelectLandNameFromPointAndAdminLevel.sql");
int rc = sqlite3_prepare_v2(m_OSMLandDB, sQuery.c_str(), -1, &stmt, NULL);
	if (rc != SQLITE_OK)
	{
		std::string error = sqlite3_errmsg(m_OSMLandDB);
		cerr << "Prepare statement for IHO Seas query failed: " << sqlite3_errmsg(m_OSMLandDB) << std::endl;
		sqlite3_close(m_OSMLandDB);
		return slLand;
	}

	//Bind values to statement
	rc = sqlite3_bind_double(stmt, 1, x);
	rc = sqlite3_bind_double(stmt, 2, y);
	rc = sqlite3_bind_double(stmt, 3, x);
	rc = sqlite3_bind_double(stmt, 4, y);
	rc = sqlite3_bind_int(stmt, 5, adminLevel * 2-1);
	rc = sqlite3_bind_int(stmt, 6, adminLevel * 2 );

	// Execute the query
	rc = 0;
	while(( rc = sqlite3_step(stmt) ) == SQLITE_ROW)
	{
		std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		int flag = sqlite3_column_int(stmt, 1);
		double area = sqlite3_column_double(stmt, 2);
		int elementType = sqlite3_column_int(stmt, 3);
		int elementID = sqlite3_column_int(stmt, 4);
		nearbyResult nameResult;
		if (flag == 1) //This is an island or islet 
			name = "Island of " + name;
		nameResult.name = name;
		nameResult.area = area;
		nameResult.elementType = elementType;
		nameResult.elementID = elementID;
		slLand.push_back(nameResult );
	}
	if (rc != SQLITE_DONE)
	{
		std::cerr << "Step failed: " << sqlite3_errmsg(m_OSMLandDB) << std::endl;
	}

	// Finalize the statement
	sqlite3_finalize(stmt);

	return slLand;
}

NRList    COSMLand::FullListOfOSMLand(double x, double y)
{
	NRList    slLand;


	//Prepare for the SQLite call 
	sqlite3_stmt* stmt;

	//The query 
	std::string sQuery = GetQueryFromScript("Scripts/SelectLandNameFromPoint.sql");
	int rc = sqlite3_prepare_v2(m_OSMLandDB, sQuery.c_str(), -1, &stmt, NULL);
	if (rc != SQLITE_OK)
	{
		std::string error = sqlite3_errmsg(m_OSMLandDB);
		cerr << "Prepare statement for full land name list query failed: " << sqlite3_errmsg(m_OSMLandDB) << std::endl;
		sqlite3_close(m_OSMLandDB);
		return slLand;
	}

	//Bind values to statement
	rc = sqlite3_bind_double(stmt, 1, x);
	rc = sqlite3_bind_double(stmt, 2, y);
	rc = sqlite3_bind_double(stmt, 3, x);
	rc = sqlite3_bind_double(stmt, 4, y);

	// Execute the query
	rc = 0;
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		double area = sqlite3_column_double(stmt, 1 );
		nearbyResult landResult;
		landResult.name = name;
		landResult.area = area; 

		slLand.push_back(landResult );
	}
	if (rc != SQLITE_DONE)
	{
		std::cerr << "Step failed: " << sqlite3_errmsg(m_OSMLandDB) << std::endl;
	}

	// Finalize the statement
	sqlite3_finalize(stmt);
	return slLand;
}

NRVector  COSMLand::NearestPointsNameAndDetails(double x, double y, double size )
{
	//Result data structure 
	NRVector  nrLand;

	//Prepare for the SQLite call 
	sqlite3_stmt* stmt;

	//The query 
	std::string sQuery = GetQueryFromScript("Scripts/SelectPointsInMbr.sql");
	int rc = sqlite3_prepare_v2(m_OSMLandDB, sQuery.c_str(), -1, &stmt, NULL);
	if (rc != SQLITE_OK)
	{
		std::string error = sqlite3_errmsg(m_OSMLandDB);
		cerr << "Prepare statement for detailed land query failed: " << sqlite3_errmsg(m_OSMLandDB) << std::endl;
		return nrLand;
	}

	//Calculate the two points to feed into the MBR 
	double x1, x2, y1, y2, size_x, size_y;
	size_x = size / 111;
	size_y = size / ( 111 * cos(y * 3.14159 / 180) );
	x1 = x - size_x;
	y1 = y - size_y;
	x2 = x + size_x;
	y2 =  y + size_y;


	//Bind values to statement
	rc = sqlite3_bind_double(stmt, 1, x);
	rc = sqlite3_bind_double(stmt, 2, y);
	rc = sqlite3_bind_double(stmt, 3, x);
	rc = sqlite3_bind_double(stmt, 4, y);
	rc = sqlite3_bind_double(stmt, 5, x1);
	rc = sqlite3_bind_double(stmt, 6, y1);
	rc = sqlite3_bind_double(stmt, 7, x2);
	rc = sqlite3_bind_double(stmt, 8, y2);

	// Execute the query
	rc = 0;
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		nearbyResult nr;
		const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		nr.name = name;
		nr.longitude= sqlite3_column_double(stmt, 1);
		nr.latitude = sqlite3_column_double(stmt, 2);
		nr.area = sqlite3_column_double(stmt, 3);
		nr.bearing = sqlite3_column_double(stmt, 5);

		//Need to calculate the distance from points
		CLongLat p1(x, y);
		CLongLat p2(nr.longitude, nr.latitude);
		nr.distance = p1.DistanceTo(p2);

		nrLand.push_back(nr);
	}
	if (rc != SQLITE_DONE)
	{
		std::cerr << "Step failed: " << sqlite3_errmsg(m_OSMLandDB) << std::endl;
	}

	// Finalize the statement
	sqlite3_finalize(stmt);
	return nrLand;
}

SVector COSMLand::NameAndDetails(double x, double y)
{
	//Default value for function return 
	std::string sOSMLandName = "Land"; // "OSMLand";
	SVector slLand;


	//Prepare for the SQLite call 
	sqlite3_stmt* stmt;

	//The query 
	std::string sQuery = GetQueryFromScript("Scripts/SelectNearestPolygons.sql");
	int rc = sqlite3_prepare_v2(m_OSMLandDB, sQuery.c_str(), -1, &stmt, NULL);
	if (rc != SQLITE_OK)
	{
		std::string error = sqlite3_errmsg(m_OSMLandDB);
		cerr << "Prepare statement for detailed land query failed: " << sqlite3_errmsg(m_OSMLandDB) << std::endl;
		slLand.push_back(sOSMLandName);
		return slLand;
	}

	//Bind values to statement
	rc = sqlite3_bind_double(stmt, 1, x);
	rc = sqlite3_bind_double(stmt, 2, y);
	rc = sqlite3_bind_double(stmt, 3, x);
	rc = sqlite3_bind_double(stmt, 4, y);
	rc = sqlite3_bind_double(stmt, 5, x);
	rc = sqlite3_bind_double(stmt, 6, y);
	rc = sqlite3_bind_double(stmt, 7, x);
	rc = sqlite3_bind_double(stmt, 8, y);

	// Execute the query
	rc = 0;
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		double area = sqlite3_column_double(stmt, 1);
		double distance  = sqlite3_column_double(stmt, 2);
		double bearing  = sqlite3_column_double(stmt, 3);

		// Create a string stream for formatting the output
		std::stringstream output;

		// Format the name, area, distance, and bearing according to your requirements
		output << std::fixed << std::setprecision(1);
		output << name << " is " << distance << "km away at bearing " << bearing << " degrees";

		// Get the formatted string
		std::string formattedString = output.str();


		slLand.push_back(formattedString);
	}
	if (rc != SQLITE_DONE)
	{
		std::cerr << "Step failed: " << sqlite3_errmsg(m_OSMLandDB) << std::endl;
	}

	// Finalize the statement
	sqlite3_finalize(stmt);

	//Ensure something gets returned 
	if (slLand.size() == 0)
		slLand.push_back(sOSMLandName);

	return slLand;
}

NRVector   COSMLand::BorderingPolygons(double x, double y, int adminLevel )
{
	NRVector   nrvNames;

	//Prepare for the SQLite call 
	sqlite3_stmt* stmt;

	//The query 
	std::string sQuery = GetQueryFromScript("Scripts/SelectBorderingRelationsFromCurrentPosition.sql");
	int rc = sqlite3_prepare_v2(m_OSMLandDB, sQuery.c_str(), -1, &stmt, NULL);
	if (rc != SQLITE_OK)
	{
		std::string error = sqlite3_errmsg(m_OSMLandDB);
		cerr << "Prepare statement for getting bordering polygons query failed: " << sqlite3_errmsg(m_OSMLandDB) << std::endl;
		sqlite3_close(m_OSMLandDB);
		return nrvNames;
	}

	//Bind values to statement
	rc = sqlite3_bind_double(stmt, 1, x);
	rc = sqlite3_bind_double(stmt, 2, y);
	rc = sqlite3_bind_int(stmt, 3, adminLevel * 2 - 1);
	rc = sqlite3_bind_int(stmt, 4, adminLevel * 2);
	rc = sqlite3_bind_double(stmt, 5, x);
	rc = sqlite3_bind_double(stmt, 6, y);
	rc = sqlite3_bind_double(stmt, 7, x);
	rc = sqlite3_bind_double(stmt, 8, y);
	rc = sqlite3_bind_int(stmt, 9, adminLevel * 2 - 1);
	rc = sqlite3_bind_int(stmt, 10, adminLevel * 2);

	// Execute the query
	rc = 0;
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		nearbyResult borderResult;
		const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		double x = sqlite3_column_double( stmt, 1 );
		double y = sqlite3_column_double(stmt, 2);
		double bearing = sqlite3_column_double(stmt, 3);

		borderResult.name = name;
		borderResult.longitude= x;
		borderResult.latitude= y;
		borderResult.bearing = bearing;

		nrvNames.push_back(borderResult );
	}
	if (rc != SQLITE_DONE)
	{
		std::cerr << "Step failed: " << sqlite3_errmsg(m_OSMLandDB) << std::endl;
	}

	// Finalize the statement
	sqlite3_finalize(stmt);

	return nrvNames;
}

double COSMLand::Distance(CLongLat p1, CLongLat p2)
{
	//Temporary distance calculator 
	if (true)
	{
		double diffLon = ( p2.longitude() - p1.longitude() ) * CKG_PI / 180.0;
		double lat1 = p1.latitude() * CKG_PI / 180.0;
		double lon1 = p1.longitude() * CKG_PI / 180.0;
		double lat2 = p2.latitude() * CKG_PI / 180.0;
		double lon2 = p2.longitude() * CKG_PI / 180.0;
		double distance = 6371  * acos( sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2 ) * cos(diffLon) );
		return distance;
	}

	double distance = 0;

	//Prepare for the SQLite call 
	sqlite3_stmt* stmt;

	//The query 
	std::string sQuery = GetQueryFromScript( "Scripts/CalculateDistanceBetweenTwoPoints.sql" );
	int rc = sqlite3_prepare_v2(m_OSMLandDB, sQuery.c_str(), -1, &stmt, NULL);
	if (rc != SQLITE_OK)
	{
		std::string error = sqlite3_errmsg(m_OSMLandDB);
		cerr << "Prepare statement for distance query failed: " << sqlite3_errmsg(m_OSMLandDB) << std::endl;
		sqlite3_close(m_OSMLandDB);
		return distance;
	}

	//Bind values to statement
	rc = sqlite3_bind_double(stmt, 1, p1.longitude() );
 	rc = sqlite3_bind_double(stmt, 2, p1.latitude());
	rc = sqlite3_bind_double(stmt, 3, p2.longitude() );
	rc = sqlite3_bind_double(stmt, 4, p2.latitude());

	// Execute the query
	rc = sqlite3_step(stmt);
	if(rc == SQLITE_ROW)
	{
		distance = sqlite3_column_double(stmt, 0);
	}
	else
	{
		std::cerr << "Step failedin distance calculation: " << sqlite3_errmsg(m_OSMLandDB) << std::endl;
		const char* p = sqlite3_errmsg(m_OSMLandDB);
		int k = 0;
	}

	// Finalize the statement
	sqlite3_finalize(stmt);


	return distance;
}

double COSMLand::Bearing(CLongLat p1, CLongLat p2)
{
	double bearing = 0;

	//Prepare for the SQLite call 
	sqlite3_stmt* stmt;

	//The query 
	std::string sQuery = GetQueryFromScript("Scripts/CalculateBearingBetweenTwoPoints.sql");
	int rc = sqlite3_prepare_v2(m_OSMLandDB, sQuery.c_str(), -1, &stmt, NULL);
	if (rc != SQLITE_OK)
	{
		std::string error = sqlite3_errmsg(m_OSMLandDB);
		cerr << "Prepare statement for bearing query failed: " << sqlite3_errmsg(m_OSMLandDB) << std::endl;
		sqlite3_close(m_OSMLandDB);
		return bearing;
	}

	//Bind values to statement
	rc = sqlite3_bind_double(stmt, 1, p1.longitude());
	rc = sqlite3_bind_double(stmt, 2, p1.latitude());
	rc = sqlite3_bind_double(stmt, 3, p2.longitude());
	rc = sqlite3_bind_double(stmt, 4, p2.latitude());

	// Execute the query
	rc = sqlite3_step(stmt);
	if (rc == SQLITE_ROW)
	{
		bearing = sqlite3_column_double(stmt, 0);
	}
	else
	{
		std::cerr << "Step failedin bearing calculation: " << sqlite3_errmsg(m_OSMLandDB) << std::endl;
		const char* p = sqlite3_errmsg(m_OSMLandDB);
	}

	// Finalize the statement
	sqlite3_finalize(stmt);


	return bearing;
}

SVector COSMLand::SearchForName(std::string rsText, LLVector& rSearchResultPositions)
{
	SVector svResults;

	//Prepare for the SQLite call 
	sqlite3_stmt* stmt;

	//The query 
	std::string sQuery = GetQueryFromScript("Scripts/SelectSearchResults.sql");
	int rc = sqlite3_prepare_v2(m_OSMLandDB, sQuery.c_str(), -1, &stmt, NULL);
	if (rc != SQLITE_OK)
	{
		std::string error = sqlite3_errmsg(m_OSMLandDB);
		cerr << "Prepare statement for search for name query failed: " << sqlite3_errmsg(m_OSMLandDB) << std::endl;
		sqlite3_close(m_OSMLandDB);
		return svResults;
	}

	//Bind values to statement
	rc = sqlite3_bind_text(stmt, 1, rsText.c_str(), rsText.length(), SQLITE_TRANSIENT );
	rc = sqlite3_bind_text(stmt, 2, rsText.c_str(), rsText.length(), SQLITE_TRANSIENT);

	// Execute the query
	rc = 0;
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		svResults.push_back(name);

		//Coordinates of search results 
		double x = sqlite3_column_double(stmt, 1);
		double y = sqlite3_column_double(stmt, 2);
		CLongLat ll(x, y);
		rSearchResultPositions.push_back(ll);
			}
	if (rc != SQLITE_DONE)
	{
		std::cerr << "Step failed: " << sqlite3_errmsg(m_OSMLandDB) << std::endl;
	}

	// Finalize the statement
	sqlite3_finalize(stmt);
	return svResults;
}

std::string COSMLand::GetQueryFromScript(std::string rsScriptFilename)
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
	return buffer.str();

}

std::string COSMLand::GetWikipediaValue(int type, int ID)
{
	std::string sWikiID = "";

	//Prepare for the SQLite call 
	sqlite3_stmt* stmt;

	//The query 
	std::string sQuery = GetQueryFromScript("Scripts/SelectWikipediaValue.sql");
	int rc = sqlite3_prepare_v2(m_OSMLandDB, sQuery.c_str(), -1, &stmt, NULL);
	if (rc != SQLITE_OK)
	{
		std::string error = sqlite3_errmsg(m_OSMLandDB);
		cerr << "Prepare statement for wikipedia query failed: " << sqlite3_errmsg(m_OSMLandDB) << std::endl;
		return sWikiID;
	}

	//Bind values to statement
	rc = sqlite3_bind_int(stmt, 1, type );
	rc = sqlite3_bind_int(stmt, 2, ID );

	// Execute the query
	rc = sqlite3_step(stmt);
	if (rc == SQLITE_ROW)
	{
		sWikiID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
	}
	else
	{
		std::cerr << "Step failed in getting wikipedia entrycalculation: " << sqlite3_errmsg(m_OSMLandDB) << std::endl;
		const char* p = sqlite3_errmsg(m_OSMLandDB);
	}

	// Finalize the statement
	sqlite3_finalize(stmt);


	return sWikiID;
}