#ifndef DNTOTOA_H
#define DNTOTOA_H

#include "gdal_priv.h"
#include "sensorparam.h"
#include "auxtable.h"
#include "stl.h"

#include <QDateTime>
#include <QProcess>

#include "6s.h"
#include "Transform.h"

class DnToToa {
public:
    DnToToa(const char * homePath);
    ~DnToToa();
    bool DnToReflectance(const char* filename, string atmMode, int continental, string visibility, double heightSeaLevel, const char *toaFileName="");
    bool Correction6S(const char * filename, const char * inpFileName, const char *surfFileName);
    bool CleanUp(const char *path, const char * filename, const char * inpFileName, const char *surfFileName);
    bool WaterCorrection(const char * filename, const char *surfFileName);
private:
    double getSunDeclination(QDate date);
    double solarIncidence(double lat, double lon, QDateTime timeStamp);
    double angularTime(QTime time, double lon);
    double d2r(double degree);
    double SunEarthDistanceRatio(int d_n);
    bool copyHeaders (const char * headerPath, const char * newHeader);
    int dayOfTheYear(tm date);
    double coeficient(double esun, double incidence, QDate date);
    bool write6SFile(SensorParam params, auxTable table, long int npixels, string atmMode, int continental, string visibility, double heightSeaLevel,std::string prefix="processing");
    double rhoSpecular(double incidence, double ee0);
    double waterSpecularCorrection(double sup, double rho_water, double rho_specular);
    std::string homePath;

    auxTable auxtable;
    SensorParam params;
    
    
};

#endif
