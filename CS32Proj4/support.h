#ifndef SUPPORT_INCLUDED
#define SUPPORT_INCLUDED

#include "provided.h"

//Comparison operator overloads
inline
bool operator>(const GeoCoord& lhs, const GeoCoord& rhs)
{
	return lhs.latitude > rhs.latitude || (lhs.latitude == rhs.latitude && lhs.longitude > rhs.longitude);
}

inline
bool operator<(const GeoCoord& lhs, const GeoCoord& rhs)
{
	return lhs.latitude < rhs.latitude || (lhs.latitude == rhs.latitude && lhs.longitude < rhs.longitude);
}

inline
bool operator==(const GeoCoord& lhs, const GeoCoord& rhs)
{
	return lhs.latitude == rhs.latitude && lhs.longitude == rhs.longitude;
}

inline
std::string directionOfLine(const GeoSegment& gs) {
	double angle = angleOfLine(gs);
	if (0 <= angle && angle <= 22.5)
		return "east";
	if (22.5 < angle && angle <= 67.5)
		return "northeast";
	if (67.5 < angle && angle <= 112.5)
		return "north";
	if (112.5 < angle && angle <= 157.5)
		return "northwest";
	if (157.5 < angle && angle <= 202.5)
		return "west";
	if (202.5 < angle && angle <= 247.5)
		return "southwest";
	if (247.5 < angle && angle <= 292.5)
		return "south";
	if (292.5 < angle && angle <= 337.5)
		return "southeast";
	if (337.5 < angle && angle < 360)
		return "east";
	return "ERROR ANGLE OUT OF BOUNDS";
}
#endif //SUPPORT_INCLUDED
