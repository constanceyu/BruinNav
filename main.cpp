// The main.cpp you can use for testing will replace this file soon.

#include "provided.h"
#include <iostream>
#include "MyMap.h"

using namespace std;
void foo() {
    MyMap<string,double> nameToGPA; // maps student name to GPA
    // add new items to the binary search tree-based map nameToGPA.associate("Carey", 3.5); // Carey has a 3.5 GPA nameToGPA.associate("David", 3.99); // David beat Carey nameToGPA.associate("Abe", 3.2); // Abe has a 3.2 GPA
    double* davidsGPA = nameToGPA.find("David"); if (davidsGPA != nullptr)
        *davidsGPA = 1.5; // after a re-grade of David’s exam
    nameToGPA.associate("David", 2.5);
    nameToGPA.associate("David", 4.0);
    nameToGPA.associate("Carey", 4.0); // Carey deserves a 4.0 // replaces old 3.5 GPA
    nameToGPA.associate("Linda", 4.0);
    double* lindasGPA = nameToGPA.find("David"); if (lindasGPA == nullptr)
        cout << "Linda is not in the roster!" << endl; else
            cout << "Linda’s GPA is: " << *lindasGPA << endl;
}

void example(const MapLoader& ml) {
    SegmentMapper sm;
    sm.init(ml); // let our object build its internal data structures
    // by iterating thru all segments from the MapLoader object
    GeoCoord lookMeUp("34.0474215", "-118.4901897");
    std::vector<StreetSegment> vecOfAssociatedSegs(sm.getSegments(lookMeUp)); if (vecOfAssociatedSegs.empty())
    {
        cout << "Error - no segments found matching this coordinate\n";
        return;
    }
    cout << "Here are all the segments associated with your coordinate:" << endl;
    for (auto s: vecOfAssociatedSegs)
    {
        cout << "Segment’s street: " << s.streetName << endl;
        cout << "Segment’s start lat/long: " << s.segment.start.latitude << ", " << s.segment.start.longitude << endl;
        cout << "Segment’s end lat/long: " << s.segment.end.latitude << ", " << s.segment.end.longitude << endl;
        cout << "This segment has " << s.attractions.size() << " attractions on it." << endl;
    }
}

 int main(int argc, const char * argv[]) {
 // insert code here...
 //foo();
     
     MapLoader mp;
     mp.load("/Users/Faith/Desktop/mapdata.txt");
     Navigator nav;
     nav.loadMapData("/Users/Faith/Desktop/mapdata.txt");
     vector<NavSegment> result;
     nav.navigate("Hyatt Regency Century Plaza", "2000 Avenue of the Stars", result);
     while(!result.empty())
     {
         NavSegment navSeg = result.back();
         result.pop_back();
         cerr << navSeg.m_distance << endl;
     }
     /*AttractionMapper am;
     am.init(mp);
     GeoCoord fillMe;
     
     string attraction = "ucla Lab School";
     bool found = am.getGeoCoord(attraction, fillMe);
     if ( ! found)
     {
         cout << "No geolocation found for " << attraction << endl;
         return 1;
     }
     cout << "The location of " << attraction << " is " <<
     fillMe.latitudeText << ", " << fillMe.longitudeText << endl;
     
 return 0;*/
     //example(mp);
     
 }
