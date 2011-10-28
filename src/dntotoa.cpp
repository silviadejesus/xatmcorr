#include "dntotoa.h"

DnToToa::DnToToa(const char * homePath) {
    this->homePath=homePath;
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
        double hour=params.timeStamp.time().hour()+params.timeStamp.time().minute()/60. +params.timeStamp.time().second()/3600;
        outFile <<params.timeStamp.date().month() <<" "<<  params.timeStamp.date().day() <<" "<< hour << " "<< params.centerLon << " "<<  params.centerLat <<"    (month,day,hh.ddd,long.,lat.) (hh.ddd=the decimal hour in universal time)"<<endl;
        outFile <<atmMode<<"                            (tropical atmospheric mode)"<<endl;
        outFile <<continental<<"                            (continental)"<<endl;
        outFile <<visibility<<"                           (visibility in km (aerosol model concentration)"<<endl;
        outFile <<heightSeaLevel<<"                        (target at  m above sea level)"<<endl;
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

/** Sun earth-distance ratio computation. */
double DnToToa::SunEarthDistanceRatio(int d_n) {
    double t=(2*pi* (d_n - 1) / 365);
    double E_0=(1.000110+ 0.034221*cos(t) + 0.001280*sin(t) + 0.000719*cos(2*t) + 0.000077*sin(2*t));
    return sqrt(1/E_0);
}

/** */
double DnToToa::angularTime(QTime time) {
    QTime midday(12,0,0);
    int seconds=midday.secsTo(time);
    double decTime=seconds/3600.;
    
    return (decTime*pi/12);
}

/**Computes the solar incidence in a given latitude at the given time*/
double DnToToa::solarIncidence(double lat, QDateTime timeStamp) {
    double delta=this->getSunDeclination(timeStamp.date());
    double omega=this->angularTime(timeStamp.time());
    lat*=pi/180;
    double zenital=acos(sin(delta)*sin(lat) +cos(delta)*cos(lat)*cos(omega));
    return (pi/2-zenital);
    //return d2r(42.4467);
}
/** Sun's declination computation */
double DnToToa::getSunDeclination(QDate date) {
    int dJul=-1*date.daysTo(QDate(date.year(),1,1));
    double declination=23.45*sin((dJul+284)*360/365*pi/180)*pi/180;
    return declination;
}

/*! converts tm date to yearly date */
int DnToToa::dayOfTheYear(tm date) {
    QDate d1( date.tm_year, date.tm_mon, date.tm_mday );
    return d1.dayOfYear();;
}

double DnToToa::coeficient(double esun, double incidence, QDate date) {
    int jul=date.dayOfYear();//dayOfTheYear(date);
    double d=SunEarthDistanceRatio(jul);
    return pi*d*d/(esun*sin(incidence));
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
    std::string csvPath=this->homePath+"/radiancia.csv";
    print(csvPath);
    auxtable.readAuxTable(params.bandNumber.c_str(), params.sensorName.c_str(),params.satellite.c_str(),params.timeStamp,csvPath.c_str());
    //can't the incidence angle be computed also?
    double incidence=this->solarIncidence(params.centerLat,params.timeStamp);
    print(incidence*180/pi<< " "<< params.incidenceAngle);
    double c=coeficient(auxtable.esun,incidence,params.timeStamp.date());
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
    char *pafWriteline = (char *) CPLMalloc(sizeof(char)*nXSize);

    print("Opening input and output files.");
    //creating output file
    GDALDriver * poDriver=GetGDALDriverManager()->GetDriverByName("ENVI");
    char ** options= NULL;
    //options=CSLSetNameValue(options,"SUFFIX","ADD");
    GDALDataset * poDstDS=poDriver->Create(toaFileName,nXSize,nYsize,1,GDT_Byte, NULL);

    //copying geographic header
    double * geotransform= new double[6];
    poDataset->GetGeoTransform(geotransform);
    poDstDS->SetGeoTransform(geotransform);
    delete geotransform;
    poDstDS->SetProjection(poDataset->GetProjectionRef());
    GDALRasterBand * poOutBand=poDstDS->GetRasterBand(1);
    float newval;
    
    //performing the transformation from DN to Top Of Atmosphere
    print("Computing Reflectance Top of Atmosphere.");
    for (int j=0;j<nYsize;j++) {
        poBand->RasterIO( GF_Read, 0, j, nXSize, 1,pafScanline, nXSize, 1, GDT_Float32,0, 0 );
        for (int i=0;i<nXSize;i++) {
            //if ((i==2000) && (j==2000))
            //    cout<<"teste"<<endl;
            newval=c*(a*(pafScanline[i]-auxtable.dnMin) + auxtable.lmin);

            //if (newval<0) newval=0;
            pafWriteline[i]=char(round(newval*255));
        }
        poOutBand->RasterIO( GF_Write, 0, j, nXSize, 1,pafWriteline, nXSize, 1, GDT_Byte,0, 0 );
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

    
    //path to where the 6S program is
    std::string path6s=this->homePath+QDir::separator();
    path6s+="6S";
    path6s+=QDir::separator();
    path6s+="6S_ATMS_CORR.EXE";
    
    //prepare INPFILES.TXT
    std::ofstream inpfiles("INPFILES.TXT");
    inpfiles<<inpFileName<<std::endl;
    inpfiles<<"nothing.res"<<std::endl;
    inpfiles<<filename<<std::endl;
    inpfiles<<surfFileName<<std::endl;
    inpfiles.close();
    
    QProcess proc6S;
    print("Processing atmospheric correction");
    proc6S.addArgument("wineconsole");
    proc6S.addArgument(path6s.c_str());
    proc6S.start();
    while (proc6S.isRunning ()) {
        //wait
    }
    print("Done.")
    return true;
}

/** text file copy */
bool DnToToa::copyHeaders (const char * headerPath, const char * newHeader) {
    ifstream source(headerPath);
    ofstream dest(newHeader);
    std::string line;
    if (source.is_open() && dest.is_open()) {
        while (! source.eof() ) {
            getline (source,line);
            dest<<line<<std::endl;
        }
    }
    source.close();
    dest.close();
    return true;
}

/*!Copies ENVI header and erases temporary file.*/
bool DnToToa::CleanUp(const char * path, const char * filename, const char * inpFileName, const char *surfFileName) {
    QDir dirPath(path);

    std::string curFile;//="file:";
    curFile+=path;
    curFile+=QDir::separator();
    //erasing .inp
    std::string inpPath=curFile+inpFileName;
    //dirPath.remove(inpPath.c_str());
    
    //erasing INPFILES.TXT
    std::string inpfilesPath=curFile+"INPFILES.TXT";
    dirPath.remove(inpfilesPath.c_str());
    
    //copying the toa's header to surf
    std::string headerPath=curFile;
    std::string newHeader=headerPath;
    headerPath+=filename;
    headerPath+=".hdr";
    newHeader+=surfFileName;
    newHeader+=".hdr";
    print(headerPath<< " e " <<newHeader);
    
    //copying the headers
    this->copyHeaders(headerPath.c_str(),newHeader.c_str());

}