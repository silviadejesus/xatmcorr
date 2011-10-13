//#include <QtGui/QApplication>
//#include "AtmCorr.h"

#include <iostream>
#include <string>
#include <ctime>
#include <math.h>

#include "sensorparam.h"
#include "auxtable.h"
#include "gdal_priv.h"
#include "stl.h"
//#include "gdal.h"




//TODO Fix Me
bool write6SFile(SensorParam params, auxTable table, std::string filename) {
    //read id6s from table
    return true;
}

double d2r(double degree) {
    return degree*pi/180 ;
}

double SunEarthDistanceRatio(int d_n) {
    double t=(2*pi* (d_n - 1) / 365);
    double E_0=(1.000110+ 0.034221*cos(t) + 0.001280*sin(t) + 0.000719*cos(2*t) + 0.000077*sin(2*t));
    return sqrt(1/E_0);
}


/*! converts tm date to yearly date */
int dayOfTheYear(tm date) {
    int day=floor(toJulian(date.tm_mday,date.tm_mon,date.tm_year)-toJulian(01,01,date.tm_year)+1);
    return day;
}

double coeficient(double esun, double incidence, tm date) {
    int jul=dayOfTheYear(date);
    double d=SunEarthDistanceRatio(jul);
    return pi*d*d/(esun*sin(d2r(incidence)));
}

//Processes both dn to toa radiance and toa radiance to reflectance
bool DnToReflectance(const char* filename) {
    SensorParam params;
    std::string xmlFilename=filename;
    xmlFilename.replace(xmlFilename.find(".tif"),4,".xml");
    params.readFromXml(xmlFilename);
    auxTable auxtable;
    auxtable.readAuxTable(params.bandNumber.c_str(), params.sensorName.c_str(),params.satellite.c_str(),params.timeStamp);
    //can't the incidence angle be computed also?
    double c=coeficient(auxtable.esun,params.incidenceAngle,params.timeStamp);
    //print(c);
    double a=(auxtable.lmax-auxtable.lmin)/(auxtable.dnMax-auxtable.dnMin);


    //reading  the matrix

    GDALAllRegister();
    GDALDataset  *poDataset = (GDALDataset *) GDALOpen( filename, GA_ReadOnly );
    if( poDataset == NULL ) return false;
    GDALRasterBand *poBand = poDataset->GetRasterBand( 1 );
    if( poBand == NULL ) return false;
    int nXSize = poBand->GetXSize();
    int nYsize = poBand->GetYSize();
    float *pafScanline = (float *) CPLMalloc(sizeof(float)*nXSize);

    //creating output file
    std::string outFileName=filename;
    outFileName.erase(outFileName.find(".tif"),4);//replace(outFileName.find(".tif"),4,".xml");
    outFileName+="-out.tif";
    GDALDataset * poDstDS=poDataset->GetDriver()->Create(outFileName.c_str(),nXSize,nYsize,1,GDT_Float32, NULL);
    double * geotransform= new double[6];
    poDataset->GetGeoTransform(geotransform);
    poDstDS->SetGeoTransform(geotransform);
    delete geotransform;
    poDstDS->SetProjection(poDataset->GetProjectionRef());
    GDALRasterBand * poOutBand=poDstDS->GetRasterBand(1);
    float newval;
    for (int j=0;j<nYsize;j++) {
        poBand->RasterIO( GF_Read, 0, j, nXSize, 1,pafScanline, nXSize, 1, GDT_Float32,0, 0 );
        for (int i=0;i<nXSize;i++) {
            //if ((i==2000) && (j==2000))
            //    cout<<"teste"<<endl;
            newval=c*(a*(pafScanline[i]-auxtable.dnMin) + auxtable.lmin);

            //if (newval<0) newval=0;
            pafScanline[i]=newval*100;
        }
        poOutBand->RasterIO( GF_Write, 0, j, nXSize, 1,pafScanline, nXSize, 1, GDT_Float32,0, 0 );
    }
    GDALClose(poDstDS);
    GDALClose(poDataset);
    return true;
    //return outMatrix*c*255;
}


int main(int argc, char** argv)
{
    //print(SunEarthDistanceRatio(109));
    bool res=false;
    if (argc>1) { //if no parameters were passed, open main window
        res =DnToReflectance(argv[1]);
//QApplication app(argc, argv);
        //AtmCorr foo;
        //foo.show();
        //return app.exec();
    } else {
        res=DnToReflectance("C:\Documents and Settings\vatto\Desktop\AtmCorr\LANDSAT_5_TM_19930724_002_066_L2_BAND2.tif");///home/mauriciodev/Projetos/AtmCorr/LANDSAT_5_TM_19930724_002_066_L2_BAND2.tif");
    }
    
    if (res) {
        print("Finished.");
    } else {
        print("Could not open file.");
    }
}