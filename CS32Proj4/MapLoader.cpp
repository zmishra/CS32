#include "provided.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;
private:
	vector<StreetSegment> m_segments;
};

MapLoaderImpl::MapLoaderImpl()
{
}

MapLoaderImpl::~MapLoaderImpl()
{
}

bool MapLoaderImpl::load(string mapFile)
{
	ifstream infile(mapFile);
	if (!infile)
		return false;

	string s;
	int count = 0;
	StreetSegment seg;
	int numAttractions = 0;
	while (getline(infile, s)) //get each line
	{
		count++;
		switch (count) //count determines how to deal with the line
		{
		case 1:  //first line is the street name
			seg.streetName = s;
			break;
		case 2: //second line is latitudes and longitudes
		{
			int pos = 0;
			int i = 0;
			for (; i < s.size(); i++)
				if (s[i] == ',' || s[i] == ' ') //increment up to the end of the first number
					break;
			string lat1 = s.substr(pos, i); 
			
			for (pos = pos + i; s[pos] == ' ' || s[pos] == ','; pos++); //find the start of the next number
			i = 0;
			for (; i < s.size(); i++)
				if (s[pos + i] == ' ' || s[pos + i] == ',') //repeat
					break;
			string long1 = s.substr(pos, i); 

			for (pos = pos + i; s[pos] == ' ' || s[pos] == ','; pos++);
			i = 0;
			for (; i < s.size(); i++)
				if (s[pos + i] == ' ' || s[pos + i] == ',')
					break;
			string lat2 = s.substr(pos, i); 

			for (pos = pos + i; s[pos] == ' ' || s[pos] == ','; pos++);
			i = 0;
			for (; i < s.size(); i++);
			string long2 = s.substr(pos, i); 

			GeoCoord start = GeoCoord(lat1, long1);
			GeoCoord end = GeoCoord(lat2, long2);
			seg.segment = GeoSegment(start, end);
			break;
		}
		case 3: //third line is the number of attractions
			if (stoi(s) == 0)
			{
				seg.attractions.clear(); //make sure attractions vector is empty
				m_segments.push_back(seg); //push the segment back and rest the count for the next segment
				count = 0;
				break;
			}
			else
				numAttractions = stoi(s);
			break;
		default:
			if (count <= 3 + numAttractions) //Any line after the third is an attraction
			{
				int pos = 0;
				int i = 0;
				for (; i < s.size(); i++) //find the name of the attraction
					if (s[i] == '|')
						break;
				string nm = s.substr(pos, i);

				for (pos = pos + i; s[pos] == ' ' || s[pos] == ',' || s[pos] == '|'; pos++); //find the latitude start and end
				i = 0;
				for (; i < s.size(); i++)
					if (s[pos + i] == ' ' || s[pos + i] == ',')
						break;
				string lat = s.substr(pos, i);

				for (pos = pos + i; s[pos] == ' ' || s[pos] == ','; pos++); //find the longitude start and end
				i = 0;
				for (; i < s.size(); i++);
				string lon = s.substr(pos, i);

				Attraction att;
				att.name = nm;
				att.geocoordinates = GeoCoord(lat, lon);
				seg.attractions.push_back(att);
			}
			if (count == 3 + numAttractions) //if the last attraction has been added
			{
				m_segments.push_back(seg);
				seg.attractions.clear(); //make the attraction vector empty
				count = 0;
			}
			break;
		}
	}
	return true;
}

size_t MapLoaderImpl::getNumSegments() const
{
	return m_segments.size();
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
	if (segNum < 0 || segNum >= getNumSegments())
		return false;
	else
	{
		seg = m_segments[segNum];
		return true;
	}
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
   return m_impl->getSegment(segNum, seg);
}
