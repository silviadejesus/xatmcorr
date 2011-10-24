#include "dntotoa.h"

DnToToa::DnToToa() {
    
}
DnToToa::~DnToToa() {
    
}


/*! Outputs a text file that is read by 6S*/
bool DnToToa::write6SFile(SensorParam params, auxTable table, long int npixels, int atmMode, int continental, double visibility, double heightSeaLevel,std::string prefix) {
    std::ofstream outFile;
    outFile.open(prefix.c_str());
    outFile.setf(ios::fixed);
    outFile.precision(2);
    if (outFile.is_open()) {
        outFile << table.geometry<<"                            (Landsat TM geometrical conditions)"<<endl;
        double hour=params.timeStamp.tm_hour+params.timeStamp.tm_min/60. +params.timeStamp.tm_sec/3600;
        outFile <<params.timeStamp.tm_mon <<" "<<  params.timeStamp.tm_mday <<" "<< hour << " "<< params.centerLon << " "<<  params.centerLat <<"    (month,day,hh.ddd,long.,lat.) (hh.ddd=the decimal hour in universal time)"<<endl;
        outFile <<"1                            (tropical atmospheric mode)"<<endl;
        outFile <<"6                            (continental)"<<endl;
        outFile <<"25                           (visibility in km (aerosol model concentration)"<<endl;
        outFile <<"-.130                        (target at  m above sea level)"<<endl;
        outFile <<"-1000                        (sensor on board of satellite)"<<endl;
        outFile << table.id6s <<"                          (band of TM Landsat 5)"<<endl;
        outFile <<"-1                           (TM image with reflectance scalled between 0 and 255)"<<endl;
        outFile << npixels <<"                      (number of pixels of the image=number of bytes)"<<endl;
    }
    outFile.close();
    return true;
}


double DnToToa::d2r(double degree) {
    return degree*pi/180 ;
}

double DnToToa::SunEarthDistanceRatio(int d_n) {
    double t=(2*pi* (d_n - 1) / 365);
    double E_0=(1.000110+ 0.034221*cos(t) + 0.001280*sin(t) + 0.000719*cos(2*t) + 0.000077*sin(2*t));
    return sqrt(1/E_0);
}


/*! converts tm date to yearly date */
int DnToToa::dayOfTheYear(tm date) {
    QDate d1( 1995, 5, 17 );
    QDate d2( 1985, 5, 20 );  // May 20th 1995
    print(d1.daysTo( d2 ));          // returns 3
    int day=floor(toJulian(date.tm_mday,date.tm_mon,date.tm_year)-toJulian(01,01,date.tm_year)+1);
    return day;
}

double DnToToa::coeficient(double esun, double incidence, tm date) {
    int jul=dayOfTheYear(date);
    double d=SunEarthDistanceRatio(jul);
    return pi*d*d/(esun*sin(d2r(incidence)));
}

//Processes both dn to toa radiance and toa radiance to reflectance
bool DnToToa::DnToReflectance(const char* filename, int atmMode, int continental, double visibility, double heightSeaLevel, const char *toaFileName) {
    SensorParam params;
    std::string xmlFilename=filename;
    print("Reading XML metadata.");
    xmlFilename.replace(xmlFilename.find(".tif"),4,".xml");
    params.readFromXml(xmlFilename);
    print("Reading sensor parameters");
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

    print("Opening input and output files.");
    //creating output file
    
    GDALDriver * poDriver=GetGDALDriverManager()->GetDriverByName("ENVI");
    char ** options= NULL;
    //options=CSLSetNameValue(options,"SUFFIX","ADD");
    GDALDataset * poDstDS=poDriver->Create(toaFileName,nXSize,nYsize,1,GDT_Float32, NULL);
    double * geotransform= new double[6];
    poDataset->GetGeoTransform(geotransform);
    poDstDS->SetGeoTransform(geotransform);
    delete geotransform;
    poDstDS->SetProjection(poDataset->GetProjectionRef());
    GDALRasterBand * poOutBand=poDstDS->GetRasterBand(1);
    float newval;
    print("Computing Reflectance Top of Atmosphere.");
    for (int j=0;j<nYsize;j++) {
        poBand->RasterIO( GF_Read, 0, j, nXSize, 1,pafScanline, nXSize, 1, GDT_Float32,0, 0 );
        for (int i=0;i<nXSize;i++) {
            //if ((i==2000) && (j==2000))
            //    cout<<"teste"<<endl;
            newval=c*(a*(pafScanline[i]-auxtable.dnMin) + auxtable.lmin);

            //if (newval<0) newval=0;
            pafScanline[i]=newval*255;
        }
        poOutBand->RasterIO( GF_Write, 0, j, nXSize, 1,pafScanline, nXSize, 1, GDT_Float32,0, 0 );
    }
    GDALClose(poDstDS);
    GDALClose(poDataset);
    print("TOA Reflectance file finished.");
    print("Writing atmospheric correction files.");
    
    
    std::string inpFileName=filename;
    inpFileName.replace(inpFileName.find(".tif"),4,".inp");
    write6SFile(params,auxtable, nXSize*nYsize,atmMode,continental, visibility, heightSeaLevel,inpFileName.c_str());
    print("Done.")
    return true;
    //return outMatrix*c*255;
}

bool DnToToa::Correction6S(const char * filename, const char * inpFileName, const char *surfFileName) {
    //prepare INPFILES.TXT
    std::ofstream inpfiles("INPFILES.TXT");
    
    inpfiles<<inpFileName<<std::endl;
    inpfiles<<"nothing.res"<<std::endl;
    inpfiles<<filename<<std::endl;
    inpfiles<<surfFileName<<std::endl;
    inpfiles.close();
    
    QProcess proc6S;
    proc6S.addArgument("wineconsole");
    proc6S.addArgument("6S/6S_ATMS_CORR.EXE");
    proc6S.start();
    while (proc6S.isRunning ()) {
        //nothing
    }
    return true;
}