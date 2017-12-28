//
//  support.h
//  Proj. 4
//
//  Created by Faith Yu on 14/3/2017.
//  Copyright © 2017 Faith Yu. All rights reserved.
//

#ifndef support_h
#define support_h

#include "provided.h"

bool operator>(const GeoCoord& gc1, const GeoCoord& gc2);

bool operator<(const GeoCoord& gc1, const GeoCoord& gc2);

bool operator==(const GeoCoord& gc1, const GeoCoord& gc2);

bool operator==(const StreetSegment& ss1, const StreetSegment& ss2);

bool operator!=(const StreetSegment& ss1, const StreetSegment& ss2);

std::string getDirection(double angle);

struct NavNode : public GeoCoord
{
    NavNode(StreetSegment* cameFrom, std::string streetName, GeoCoord gc, StreetSegment curr)
    :m_cameFrom(cameFrom),m_geoCoord(gc),m_gScore(__DBL_MAX__),m_fScore(__DBL_MAX__),m_currSeg(curr)
    {}
    
    StreetSegment* m_cameFrom;
    StreetSegment m_currSeg;
    GeoCoord m_geoCoord;
    double m_gScore;    // For each node, the cost of getting from the start node to that node.
    double m_fScore;    // For each node, the total cost of getting from the start node to the goal
                        // by passing by that node. That value is partly known, partly heuristic.
    
};


class Compare
{
public:
    bool operator() (NavNode nav1, NavNode nav2);
};

#endif /* support_h */
