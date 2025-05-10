//LongLat.cp

#include "pch.h"
#include "LongLat.h"
#include <math.h>

CLongLat::CLongLat()
{
	m_long = 0.0;
	m_lat = 0.0;
	m_bSet = false;
}

CLongLat::CLongLat(double x, double y)
{
	m_long = x;
	m_lat = y;
	m_bSet = true;
}

double CLongLat ::longitude()
{
	return m_long;
	}


double CLongLat::latitude()
{
	return m_lat;
}

void CLongLat::SetLongitude(double dLongitude)
{
	m_long = dLongitude;
	m_bSet = true;
}

void CLongLat::SetLatitude(double dLatitude)
{
	m_lat = dLatitude;
	m_bSet = true;
}

CLongLat CLongLat::operator-(const CLongLat& rOther) const
{
	return CLongLat(m_long - rOther.m_long , m_lat  - rOther.m_lat );
}

bool CLongLat::operator!=(const CLongLat& rOther) const
{
	bool bSame = false;

	if( m_long  ==  rOther.m_long &&   m_lat  ==  rOther.m_lat)
	bSame = true;

	return !bSame;
}


double CLongLat::DistanceTo(const CLongLat& pLL)
{
	//This is the Haversine approach to calculating distances on earth and uses an approximate ellipsoid 
	double d = 0; //Distance in metres 
	double R = 6371;
	double phi1 = m_lat * CKG_PI / 180;
	double phi2 = pLL.m_lat * CKG_PI / 180;
	double lambda1 = m_long * CKG_PI / 180;
	double lambda2 = pLL.m_long * CKG_PI / 180;
	double deltaPhi = phi2 - phi1;
	double deltaLambda = lambda2 - lambda1;

	//d = 2 * R * asin(sqrt(pow(sin(deltaPhi / 2), 2) + cos(phi1) * cos(phi2) * pow(sin(deltaLambda / 2), 2)));

	double a = pow(sin(deltaPhi / 2), 2) +
		cos(phi1) * cos(phi2) * pow(sin(deltaLambda / 2), 2);

	double c = 2 * asin(sqrt(a));

	d = R * c;  // Distance in kilometers
	return d;
}

bool CLongLat::IsSet()
{
	return m_bSet;
}