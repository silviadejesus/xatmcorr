#include "sensorparam.h"

void SensorParam::setTimeStamp(std::string timeStamp)
{


}


bool SensorParam::readFromXml(std::string filename)
{
        TiXmlDocument doc(filename.c_str());
        doc.LoadFile();
        TiXmlHandle docHandle( &doc );
        //reading sensor name

        TiXmlElement* node = docHandle.FirstChild( "prdf" ).FirstChild( "satellite" ).FirstChild( "name" ).ToElement();
        this->satellite=node->GetText();
        
        //delete node;
        node = docHandle.FirstChild( "prdf" ).FirstChild( "satellite" ).FirstChild( "number" ).ToElement();
        this->satellite+=node->GetText();
        
        //reading instrument
        node = docHandle.FirstChild( "prdf" ).FirstChild( "satellite" ).FirstChild( "instrument" ).ToElement();
        this->sensorName=node->GetText();

        //reading band number
        TiXmlHandle handle= docHandle.FirstChild( "prdf" ).FirstChild( "image" );
        node=handle.Child(39).ToElement();
        this->bandNumber=node->Value();
        //
        // .Child()
        //=node->FirstAttribute()->Value();

        //reading timestamp
        node = docHandle.FirstChild( "prdf" ).FirstChild("image").FirstChild( "timeStamp" ).FirstChild( "center" ).ToElement();
        std::string time=node->GetText();
        this->timeStamp=StrToDate(time);
        
        //reading incidence angle
        node = docHandle.FirstChild( "prdf" ).FirstChild("image").FirstChild( "sunPosition" ).FirstChild( "elevation" ).ToElement();
        this->incidenceAngle=atof(node->GetText());
        
    return true;
 /*#incidenceAngletru=sunPosition.elevation
    #timeStamp.center 2007-04-12T14:21:18.662375
    #centerLat=imageData.CT.latitude
    #centerLonimageData.CT.longitude
    #satName=satellite.name + satellite.number
    #satellite.instrument
    #bandNumber=satellite.instrument["channel"]
    return {incidenceAngle: 0, date: 0, timeStamp: 0. ,centerLat:0, centerLon:0,sensor=satName, bandNumber=bandNumber}*/
}