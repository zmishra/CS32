#include "provided.h"
#include "MyMap.h"
#include <string>
#include <cctype>
using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
	MyMap<string, GeoCoord> m_attMap;
};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
	StreetSegment curr;
	for (int i = 0; i < ml.getNumSegments(); i++) //for each segment
	{
		ml.getSegment(i, curr);
		if (!curr.attractions.empty()) //if there are attractions
			for (int j = 0; j < curr.attractions.size(); j++) //for each attraction
			{
				string nm = curr.attractions[j].name;
				for (int k = 0; k < nm.size(); k++) //change the attraction name to lower case so case does not matter
					nm[k] = tolower(nm[k]);
				m_attMap.associate(nm, curr.attractions[j].geocoordinates);
			}
	}
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
	for (int k = 0; k < attraction.size(); k++) //change the attraction name to lower case so case does not matter
		attraction[k] = tolower(attraction[k]);
	const GeoCoord* g = m_attMap.find(attraction);
	if (g == nullptr)
		return false;
	else
	{
		gc = *g;
		return true;
	}
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
