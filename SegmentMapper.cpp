#include "provided.h"
#include "MyMap.h"
#include "support.h"
#include <vector>
using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
    MyMap<GeoCoord,vector<StreetSegment>>* m_segmentMap;
};

SegmentMapperImpl::SegmentMapperImpl()
{
    m_segmentMap = new MyMap<GeoCoord, vector<StreetSegment>>;
}

SegmentMapperImpl::~SegmentMapperImpl()
{
    delete m_segmentMap;
}

// O((N+A)*log(N+A))
void SegmentMapperImpl::init(const MapLoader& ml)
{
    StreetSegment ss;
    //vector<StreetSegment> vectorOfStreetSegs;
    for(size_t t = 0; t < ml.getNumSegments(); t++) // O(N)
    {
        ml.getSegment(t, ss);
        
        // associate the starting GeoCoord with street segments in the map O(logN)
        vector<StreetSegment>* startPtr = m_segmentMap->find(ss.segment.start);
        if (startPtr == nullptr)
        {
            vector<StreetSegment> vectorOfStreetSegs;
            vectorOfStreetSegs.push_back(ss);
            m_segmentMap->associate(ss.segment.start, vectorOfStreetSegs);
        }
        else
        {
            vector<StreetSegment> vectorOfStreetSegs;
            vectorOfStreetSegs = *startPtr;
            vectorOfStreetSegs.push_back(ss);
            m_segmentMap->associate(ss.segment.start, vectorOfStreetSegs);
        }
        
        // associate the ending GeoCoord with street segments in the map O(logN)
        vector<StreetSegment>* endPtr = m_segmentMap->find(ss.segment.end);
        if (endPtr == nullptr)
        {
            vector<StreetSegment> vectorOfStreetSegs;
            vectorOfStreetSegs.push_back(ss);
            m_segmentMap->associate(ss.segment.end, vectorOfStreetSegs);
        }
        else
        {
            vector<StreetSegment> vectorOfStreetSegs;
            vectorOfStreetSegs = *endPtr;
            vectorOfStreetSegs.push_back(ss);
            m_segmentMap->associate(ss.segment.end, vectorOfStreetSegs);
        }
        
        // associate attraction's GeoCoord with street segments in the map O(logA)
        if(!ss.attractions.empty())
            for(size_t k = 0; k < ss.attractions.size(); k++)
            {
                vector<StreetSegment>* attractionPtr = m_segmentMap->find(ss.attractions[k].geocoordinates);
                if(attractionPtr == nullptr)
                {
                    vector<StreetSegment> vectorOfStreetSegs;
                    vectorOfStreetSegs.push_back(ss);
                    m_segmentMap->associate(ss.attractions[k].geocoordinates, vectorOfStreetSegs);
                }
                else
                {
                    vector<StreetSegment> vectorOfStreetSegs;
                    vectorOfStreetSegs = *attractionPtr;
                    vectorOfStreetSegs.push_back(ss);
                    m_segmentMap->associate(ss.attractions[k].geocoordinates, vectorOfStreetSegs);
                }
            }
    }
}

// O(log(N+A))
vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
    vector<StreetSegment>* p = m_segmentMap->find(gc);
    vector<StreetSegment> segments;
    if(p != nullptr)
        segments = *p;
    return segments;
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
