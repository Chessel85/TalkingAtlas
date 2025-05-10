//LongLat.h
#pragma once

const double CKG_PI = 3.141592653589793;


class CLongLat
{
//Constructors
public:
	CLongLat();
	CLongLat(double x, double y);

//Methods
public:
	double longitude();
	double latitude();
	void SetLongitude(double dLongitude );
	void SetLatitude(double dLatitude);
	CLongLat operator-(const CLongLat& rOther) const;
	bool operator!=(const CLongLat& rOther) const;
	double DistanceTo(const CLongLat& pLL);
	bool IsSet();

//Attributes
private:
	double m_long;
	double m_lat;
	bool m_bSet;
};