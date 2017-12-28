#include "provided.h"
#include "MyMap.h"
#include <string>
using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
    MyMap<string,GeoCoord>* m_attractionMap;
};

AttractionMapperImpl::AttractionMapperImpl()
{
    m_attractionMap = new MyMap<string, GeoCoord>;
}

AttractionMapperImpl::~AttractionMapperImpl()
{
    delete m_attractionMap;
}

// O(N+A*log(A))
void AttractionMapperImpl::init(const MapLoader& ml)
{
    StreetSegment ss;
    for(size_t t = 0; t < ml.getNumSegments(); t++) // O(N)
    {
        ml.getSegment(t, ss);
        if(!ss.attractions.empty())
        {
            for(size_t k = 0; k < ss.attractions.size(); k++) // O(A)
            {
                string nm = ss.attractions[k].name;
                for(size_t i = 0; i < nm.size(); i++)
                    nm[i] = tolower(nm[i]);
                GeoCoord gc = ss.attractions[k].geocoordinates;
                m_attractionMap->associate(nm, gc); //O(logA)
            }
        }
    }
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{

    string dup = attraction;
    for(size_t t = 0; t < dup.size(); t++)
        dup[t] = tolower(dup[t]);
    GeoCoord* gcPtr = m_attractionMap->find(dup);
    if(gcPtr != nullptr)
    {
        gc = *gcPtr;
        return true;
    }
	return false;
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
