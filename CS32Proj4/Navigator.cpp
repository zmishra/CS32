#include "provided.h"
#include "support.h"
#include "MyMap.h"
#include <string>
#include <vector>
#include <queue>
#include <iostream>
#include <functional>
using namespace std;

class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:
	SegmentMapper m_sm;
	AttractionMapper m_am;
	vector<NavSegment> convertToDirections(const vector<GeoCoord>& path) const;
	string angleToDirectionP(GeoSegment g) const;
	string angleToDirectionT(GeoSegment g1, GeoSegment g2) const;
	string determineStreet(GeoSegment g) const;
};

NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
	MapLoader ml;
	if (ml.load(mapFile))
	{
		m_sm.init(ml);
		m_am.init(ml);
		cerr << "DONE" << endl;
		return true;
	}
	else
		return false;
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
	GeoCoord g_start, g_end;
	if (!m_am.getGeoCoord(start, g_start))
		return NAV_BAD_SOURCE;
	if (!m_am.getGeoCoord(end, g_end))
		return NAV_BAD_DESTINATION;
	
	priority_queue<pair<double, GeoCoord>, vector<pair<double, GeoCoord>>, greater<pair<double, GeoCoord>>> frontier; //fscore, geocoord pair

	MyMap<GeoCoord, GeoCoord> cameFrom;
	MyMap<GeoCoord, double> gScore;

	gScore.associate(g_start, 0);
	frontier.push(pair<double, GeoCoord>(distanceEarthMiles(g_start, g_end), g_start)); //distanceEarthMiles is the heuristic

	bool pathFound = false;
	while (!frontier.empty())
	{
		GeoCoord current = frontier.top().second;

		if (current == g_end)
		{
			pathFound = true;
			break;
		}

		frontier.pop();
		vector<GeoCoord> neighbors; //all coords current can reach
		vector<StreetSegment> street_neighbors = m_sm.getSegments(current); //all street segments attached to current
		for (int i = 0; i < street_neighbors.size(); i++)
		{
			if (!(street_neighbors[i].segment.start == current)) //don't re-include current
				neighbors.push_back(street_neighbors[i].segment.start);
			if (!(street_neighbors[i].segment.end == current))
				neighbors.push_back(street_neighbors[i].segment.end);
			if (!street_neighbors[i].attractions.empty()) //check if the end attraction is on the street
			{
				for (int j = 0; j < street_neighbors[i].attractions.size(); j++)
					if (street_neighbors[i].attractions[j].geocoordinates == g_end)
						neighbors.push_back(g_end);
			}
		}

		for (int k = 0; k < neighbors.size(); k++) //for each neighbor
		{
			double tentative_gScore = *gScore.find(current) + distanceEarthMiles(current, neighbors[k]); //calculate tentative gscore
			if (gScore.find(neighbors[k]) == nullptr || tentative_gScore < *gScore.find(neighbors[k])) //if the neighbor has never been visited, or the gscore needs to be updated, add it to the priority queue
			{
				gScore.associate(neighbors[k], tentative_gScore);
				frontier.push(pair<double, GeoCoord>(tentative_gScore + distanceEarthMiles(neighbors[k], g_end), neighbors[k]));
				cameFrom.associate(neighbors[k], current);
			}
		}
	}

	if (pathFound)
	{
		vector<GeoCoord> path;
		path.push_back(g_end);
		GeoCoord curr = g_end;
		while (cameFrom.find(curr) != nullptr) //reconstruct path in reverse
		{
			curr = *cameFrom.find(curr);
			path.push_back(curr);
		}
		directions = convertToDirections(path);
		return NAV_SUCCESS;
	}
	else
		return NAV_NO_ROUTE;


	return NAV_NO_ROUTE;  // This compiles, but may not be correct
}

vector<NavSegment> NavigatorImpl::convertToDirections(const vector<GeoCoord>& path) const
{
	vector<NavSegment> result;
	for (int i = path.size() - 1; i > 0; i--)
	{
		NavSegment curr;
		curr.m_geoSegment.start = path[i];
		curr.m_geoSegment.end = path[i - 1];
		curr.m_distance = distanceEarthMiles(path[i], path[i - 1]);
		curr.m_command = NavSegment::PROCEED;
		curr.m_direction = angleToDirectionP(curr.m_geoSegment);
		curr.m_streetName = determineStreet(curr.m_geoSegment);
		if (result.size() > 0 && curr.m_streetName != result[result.size() - 1].m_streetName) //if the streetname changes, need a turn nav segment
		{
			NavSegment turn;
			turn.m_command = NavSegment::TURN;
			turn.m_streetName = curr.m_streetName;
			turn.m_direction = angleToDirectionT(result[result.size() - 1].m_geoSegment, curr.m_geoSegment);
			result.push_back(turn);
		}
		result.push_back(curr);
	}
	return result;
}

string NavigatorImpl::angleToDirectionP(GeoSegment g) const
{
	return directionOfLine(g);
}

string NavigatorImpl::determineStreet(GeoSegment g) const
{
	vector<StreetSegment> startVec = m_sm.getSegments(g.start); //go through the street segments for each end point and find the similarity
	vector<StreetSegment> endVec = m_sm.getSegments(g.end);
	for (int i = 0; i < startVec.size(); i++)
		for (int j = 0; j < endVec.size(); j++)
			if (startVec[i].streetName == endVec[j].streetName)
				return startVec[i].streetName;
	cerr << "ERROR NO STREET";
	return "ERROR";
}

string NavigatorImpl::angleToDirectionT(GeoSegment g1, GeoSegment g2) const
{
	double angle = angleBetween2Lines(g1, g2);
	if (angle < 180)
		return "left";
	else
		return "right";
}

//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
    m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
    delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
    return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
    return m_impl->navigate(start, end, directions);
}
