#include "provided.h"
#include "support.h"
#include "MyMap.h"
#include <vector>
#include <iostream>
using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
	MyMap<GeoCoord, vector<StreetSegment>> m_segMap;
	void addToMap(GeoCoord key, StreetSegment ss);
};

SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
	StreetSegment curr;
	for (int i = 0; i < ml.getNumSegments(); i++) //for each segment
	{
		ml.getSegment(i, curr);
		addToMap(curr.segment.start, curr); //add the start and end coords
		addToMap(curr.segment.end, curr);
		if (!curr.attractions.empty()) //if there are attractions
		{
			for (int j = 0; j < curr.attractions.size(); j++) //add each attraction
			{
				addToMap(curr.attractions[j].geocoordinates, curr);
			}
		}
	}
}

void SegmentMapperImpl::addToMap(GeoCoord key, StreetSegment ss)
{
	vector<StreetSegment>* ssVecPtr;
	ssVecPtr = m_segMap.find(key);
	if (ssVecPtr == nullptr) //if the coord is not found
	{
		vector<StreetSegment> ssVec; //create a vector and associate it
		ssVec.push_back(ss);
		m_segMap.associate(key, ssVec);
	}
	else //otherwise add the street segment to the existing vector
	{
		ssVecPtr->push_back(ss);
	}
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
	vector<StreetSegment> segments;
	const vector<StreetSegment>* segmentsPtr;
	segmentsPtr = m_segMap.find(gc);
	if (segmentsPtr == nullptr) //if the coord is not found, return the empty vector
		return segments;
	else
		return *segmentsPtr; //otherwise return the vector
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}
