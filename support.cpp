//
//  support.cpp
//  Proj. 4
//
//  Created by Faith Yu on 16/3/2017.
//  Copyright © 2017 Faith Yu. All rights reserved.
//

#include "support.h"

bool operator>(const GeoCoord& gc1, const GeoCoord& gc2)
{
    if(gc1.latitude > gc2.latitude)
        return true;
    else if(gc1.latitude < gc2.latitude)
        return false;
    else
    {
        if(gc1.longitude > gc2.longitude)
            return true;
        else //if (gc1.longitude <= gc2.longitude)
            return false;
    }
}

bool operator<(const GeoCoord& gc1, const GeoCoord& gc2)
{
    if(gc1.latitude < gc2.latitude)
        return true;
    else if(gc1.latitude > gc2.latitude)
        return false;
    else
    {
        if(gc1.longitude < gc2.longitude)
            return true;
        else //if (gc1.longitude >= gc2.longitude)
            return false;
    }
}

bool operator==(const GeoCoord& gc1, const GeoCoord& gc2)
{
    if(gc1.latitude == gc2.latitude && gc1.longitude == gc2.longitude)
        return true;
    else
        return false;
}

bool operator==(const StreetSegment& ss1, const StreetSegment& ss2)
{
    if(ss1.segment.start==ss2.segment.start && ss1.segment.end == ss2.segment.end)
        return true;
    return false;
}

bool operator!=(const StreetSegment& ss1, const StreetSegment& ss2)
{
    if(ss1.segment.start==ss2.segment.start && ss1.segment.end == ss2.segment.end)
        return false;
    return true;
}

std::string getDirection(double angle)
{
    if(angle > 0 && angle <= 22.5)
        return "east";
    else if(angle > 22.5 && angle <= 67.5)
        return "northeast";
    else if(angle > 67.5 && angle <=112.5)
        return "north";
    else if(angle > 112.5 && angle <= 157.5)
        return "northwest";
    else if(angle > 157.5 && angle <= 202.5)
        return "west";
    else if(angle > 202.5 && angle <= 247.5)
        return "southwest";
    else if(angle > 247.5 && angle <= 292.5)
        return "south";
    else if(angle > 292.5 && angle <= 337.5)
        return "southeast";
    else
        return "east";
}

// returns true if nav1 is smaller than nav2
bool Compare::operator() (NavNode nav1, NavNode nav2)
{
    if(nav1.m_fScore > nav2.m_fScore)
        return true;
    else
        return false;
}
