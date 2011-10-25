#ifndef SENSORPARAM_H
#define SENSORPARAM_H

#include <string>
#include <ctime>
#include <iostream>
#include "qdatetime.h"

#include "stl.h"

#include "tinyxml.h"
#define print(x) std::cout<<x<<std::endl;

class SensorParam {
    public:
        std::string satellite,sensorName, bandNumber;
        //struct std::tm timeStamp; /*{0,0,0,24,5,104} june 24 2004*/
        QDateTime timeStamp;
        double incidenceAngle,centerLat, centerLon;

        bool readFromXml(std::string filename);
        
        double getDecTime() {
            QTime time;
            return time.second()/3600.+time.minute()/60. +time.hour();
        }
        void setTimeStamp(std::string timeStamp);
};

#endif