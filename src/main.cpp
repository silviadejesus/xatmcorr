//#include <QtGui/QApplication>
//#include "AtmCorr.h"


#include <QApplication>

#include <iostream>
#include <string>
#include <ctime>
#include <math.h>
#include <fstream>


//#include "gdal.h"

#include "batchcor.h"
#include "dntotoa.h"




int main(int argc, char** argv)
{
    //print(SunEarthDistanceRatio(109));
    bool res=false;
    int continental;
    double heightSeaLevel;
    string visibility,atmMode;
    QApplication a( argc, argv );
    if (argc>1) { //if no parameters were passed, open main window
        if (argc>5) {
            atmMode=argv[2];
            continental=atoi(argv[3]);
            visibility=argv[4];
            heightSeaLevel=atof(argv[5]);
        }
        QString t;
        DnToToa tool1(a.applicationDirPath().toAscii());
        res =tool1.DnToReflectance(argv[1],atmMode,continental,visibility,heightSeaLevel);
        //QApplication app(argc, argv);
        //AtmCorr foo;
        //foo.show();
        //return app.exec();
    } else {
        
        batchCor main(NULL,a.applicationDirPath());
        //a.setMainWidget( &main );
        main.show();
        res=a.exec();
        //res=DnToReflectance("C:\Documents and Settings\vatto\Desktop\AtmCorr\LANDSAT_5_TM_19930724_002_066_L2_BAND2.tif");///home/mauriciodev/Projetos/AtmCorr/LANDSAT_5_TM_19930724_002_066_L2_BAND2.tif");
    }
     
    if (!res) {
        print("Terminado.");
    } else {
        print("Não foi possível completar o processo.");
    }

}
