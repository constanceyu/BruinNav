#include "provided.h"
#include "MyMap.h"
#include <string>
#include <queue>
#include <vector>
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
    vector<StreetSegment> vectorOfStreetSegs;
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
    if ( ! infile )		        // Did opening the file fail?
    {
        cerr << "Error: Cannot open data.txt!" << endl;
        return false;
    }
    
    string s;
    queue<string> queueOfStrings;
    // push all the strings into the queue
    // O(N)
    while (getline(infile,s))
        queueOfStrings.push(s);
    
    // constructing items and putting them into MyMap
    //O(N)
    while(!queueOfStrings.empty())
    {
        StreetSegment streetSeg;
        
        // load the name
        streetSeg.streetName = queueOfStrings.front();
        queueOfStrings.pop();
        
        // load the geosegment
        string temp = queueOfStrings.front();
        size_t t1 = temp.find(',');         // the end of the start lat
        string lat1 = temp.substr(0,t1);
        
        size_t t2 = temp.find(' ',t1+2);    // the end of the start long
        string lon1 = temp.substr(t1+2,t2-t1-2);
        
        size_t t3 = temp.find(',',t2);      // the end of the end lat
        string lat2 = temp.substr(t2+1,t3-t2-1);

        string lon2 = temp.substr(t3+1);
        streetSeg.segment = GeoSegment(GeoCoord(lat1, lon1), GeoCoord(lat2,lon2));
        queueOfStrings.pop();
        
        // load the attractions
        int numOfAttractions = stoi(queueOfStrings.front());
        queueOfStrings.pop();
        while (numOfAttractions > 0)
        {
            Attraction attraction;
            
            string temp2 = queueOfStrings.front();
            queueOfStrings.pop();
            
            size_t pos = temp2.find('|');
            attraction.name = temp2.substr(0,pos);
            
            size_t pos1 = temp2.find(',',pos);
            string attractionLat = temp2.substr(pos+1,pos1-pos-1);
            string attractionLon = temp2.substr(pos1+2);
            attraction.geocoordinates = GeoCoord(attractionLat, attractionLon);
            
            streetSeg.attractions.push_back(attraction);
            
            numOfAttractions--;
        }
        
        // loading MyMap
        vectorOfStreetSegs.push_back(streetSeg);
        
    }
    
    return true;
}

size_t MapLoaderImpl::getNumSegments() const
{
	//return m_map->size();
    return vectorOfStreetSegs.size();
}

// O(1)
bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
    if(segNum < 0 || segNum >= getNumSegments())
        return false;
    
    // this should be doing the right thing
    seg = vectorOfStreetSegs[segNum];
    
    return true;

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
