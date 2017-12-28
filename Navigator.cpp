#include "provided.h"
#include "support.h"
#include "MyMap.h"
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <queue>
using namespace std;

class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:
    MapLoader m_mapLoader;
    AttractionMapper m_attractionMap;
    SegmentMapper m_segmentMap;
};

NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
    if(!m_mapLoader.load(mapFile))
        return false;
    m_attractionMap.init(m_mapLoader);
    m_segmentMap.init(m_mapLoader);
	return true;
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
    GeoCoord startGeo, endGeo;
    if(!m_attractionMap.getGeoCoord(start, startGeo))//O(log(A))
        return NAV_BAD_SOURCE;
    if(!m_attractionMap.getGeoCoord(end, endGeo))//O(logA)
        return NAV_BAD_DESTINATION;
    

    vector<StreetSegment> ss = m_segmentMap.getSegments(startGeo);
    vector<StreetSegment> es = m_segmentMap.getSegments(endGeo);
    StreetSegment endSegment = es.back();
    StreetSegment startSegment = ss.back();
    
    // when start and end are on the same segment
    if(startSegment == endSegment)
    {
        directions.clear();
        directions.push_back(NavSegment(getDirection(angleOfLine(startSegment.segment)), startSegment.streetName, distanceEarthMiles(startGeo, endGeo), startSegment.segment));
        return NAV_SUCCESS;
    }
    
    // when start and end are on different segments
    list<NavNode> solutionPath;
    priority_queue<NavNode, vector<NavNode>, Compare> OPEN;
    MyMap<GeoCoord, NavNode> CLOSED;
    
    NavNode startNavNode = NavNode(&startSegment, startSegment.streetName, startGeo, startSegment);
    startNavNode.m_gScore = 0;
    startNavNode.m_fScore = distanceEarthMiles(startGeo, endGeo);
    //OPEN.push(startNavNode);
    
    NavNode start1 = NavNode(&startSegment, startSegment.streetName, startSegment.segment.start, startSegment);
    start1.m_fScore = distanceEarthMiles(start1, endGeo);
    OPEN.push(start1);
    
    NavNode start2 = NavNode(&startSegment, startSegment.streetName, startSegment.segment.end, startSegment);
    start2.m_fScore = distanceEarthMiles(start2, endGeo);
    OPEN.push(start2);
    
    // push everything into the list
    while(!OPEN.empty())
    {
        // currNode has the smallest distance to the destination
        NavNode currNode = OPEN.top();
        OPEN.pop();
        cerr << currNode.m_currSeg.streetName << endl;
        
        // if we find the endNode, fill in the directions vector
        if(currNode.m_currSeg == endSegment)
        {
            directions.clear();
            while(!solutionPath.empty())
            {
                NavNode cnn = solutionPath.back();
                solutionPath.pop_back();
                if(cnn.m_currSeg.streetName == cnn.m_cameFrom->streetName)
                {
                    directions.push_back(NavSegment(getDirection(angleOfLine(startSegment.segment)),
                                                    currNode.m_currSeg.streetName,
                                                    distanceEarthMiles(currNode.m_geoCoord,currNode.m_cameFrom->segment.start),
                                                    currNode.m_cameFrom->segment));
                }
                else    // TURN NavSeg
                {
                    directions.push_back(NavSegment(getDirection(angleBetween2Lines(cnn.m_currSeg.segment, cnn.m_cameFrom->segment)), cnn.m_cameFrom->streetName));
                }
            }
            return NAV_SUCCESS;
        }
        
        CLOSED.associate(currNode.m_geoCoord, currNode);
        
        // process the neighbours
        vector<StreetSegment> toBeAdded = m_segmentMap.getSegments(currNode.m_geoCoord);
        while(!toBeAdded.empty())
        {
            StreetSegment ss = toBeAdded.back();
            toBeAdded.pop_back();
            
            // if we already discovered it, continue
            if(CLOSED.find(ss.segment.end) != nullptr && CLOSED.find(ss.segment.start) != nullptr)
                continue;
            
            double temp_gScore1 = currNode.m_gScore + distanceEarthMiles(currNode, ss.segment.start);
            double temp_gScore2 = currNode.m_gScore + distanceEarthMiles(currNode, ss.segment.end);
            if(temp_gScore1 <= temp_gScore2)
            {
                NavNode startNav = NavNode(&currNode.m_currSeg, ss.streetName, ss.segment.start, ss);
                
                // need to check if startNav is in OPEN
                priority_queue<NavNode, vector<NavNode>, Compare> temp(OPEN);
                bool alreadyThere = false;
                while(temp.size()!=0)
                {
                    NavNode tempNav = temp.top();
                    temp.pop();
                    if(tempNav.m_geoCoord == startNav.m_geoCoord)
                        alreadyThere = true;
                }
                if(!alreadyThere)
                    OPEN.push(startNav);
                
                if(temp_gScore1 >= startNav.m_gScore)
                    continue;
                else
                {
                    startNav.m_gScore = temp_gScore1;
                    startNav.m_fScore = startNav.m_gScore + distanceEarthMiles(startNav.m_geoCoord, endGeo);
                    solutionPath.push_back(startNav);
                }
            }
            else
            {
                NavNode endNav = NavNode(&currNode.m_currSeg, ss.streetName, ss.segment.end, ss);
                
                // need to check if endNav is in OPEN
                priority_queue<NavNode, vector<NavNode>, Compare> temp(OPEN);
                bool alreadyThere = false;
                while(temp.size()!=0)
                {
                    NavNode tempNav = temp.top();
                    temp.pop();
                    if(tempNav.m_geoCoord == endNav.m_geoCoord)
                        alreadyThere = true;
                }
                if(!alreadyThere)
                    OPEN.push(endNav);
                
                if(temp_gScore2 >= endNav.m_gScore)
                    continue;
                else
                {
                    endNav.m_gScore = temp_gScore2;
                    endNav.m_fScore = endNav.m_gScore + distanceEarthMiles(endNav.m_geoCoord, endGeo);
                    solutionPath.push_back(endNav);
                }
            }
            
        }
        
    }
    
	return NAV_NO_ROUTE;  // This compiles, but may not be correct
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
