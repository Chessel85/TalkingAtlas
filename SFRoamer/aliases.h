//aliases.h
#pragma once

#include <string>
#include <list>
#include <vector>
#include "LongLat.h"

struct nearbyResult
{
	std::string name;
	double latitude;
	double longitude;
	double area;
	double distance;
	double bearing;
	int elementType;
	int elementID;
};

using SList = std::list<std::string>;
using SVector = std::vector<std::string>;
using LLVector = std::vector<CLongLat>;
using  NRVector   = std::vector<nearbyResult>;
using  NRList = std::list<nearbyResult>;