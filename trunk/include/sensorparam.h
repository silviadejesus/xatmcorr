#include <string>
#include <ctime>
#include <iostream>

#include "stl.h"

#include "tinyxml.h"
#define print(x) std::cout<<x<<std::endl;

class SensorParam {
    public:
        std::string satellite,sensorName, bandNumber;
        struct std::tm timeStamp; /*{0,0,0,24,5,104} june 24 2004*/
        double incidenceAngle,centerLat, centerLon;

        bool readFromXml(std::string filename);
        
        double getDecTime() {
            return timeStamp.tm_sec/3600.+timeStamp.tm_min/60. +timeStamp.tm_hour;
        }
        void setTimeStamp(std::string timeStamp);
};
