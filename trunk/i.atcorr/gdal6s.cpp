#include "6s.h"
#include "Transform.h"
#include <iostream>
#include <string>
using namespace std;
#include "gdal_priv.h"
#include "cpl_conv.h" // for CPLMalloc()
#include "ogrsf_frmts.h"

#define print(x) cout<<x<<endl;

int main(int argc, char* argv[]) {
    if (argc<3) {
        cout <<"This program should be called as: "<< endl << "gdal6s InputImage.tif OutPutImage.tif parameters.inp" <<endl;
        return 0;
    }
    //prearations for 6s transformation
    TransformInput ti;
    init_6S(argv[3]);
    ti=compute();

    //reading and writing the transformation

    //reading  the matrix

    GDALAllRegister();
    GDALDataset  *poDataset = (GDALDataset *) GDALOpen( argv[1], GA_ReadOnly );
    if( poDataset == NULL )  { cout<< "Could not open the image."<<endl; return 0; }
    GDALRasterBand *poBand = poDataset->GetRasterBand( 1 );
    if( poBand == NULL )  { cout<< "Could not open the image."<<endl; return 0; }
    int nXSize = poBand->GetXSize();
    int nYsize = poBand->GetYSize();

    float *pafScanline = (float *) CPLMalloc(sizeof(float)*nXSize);
    float *pafWriteline = (float *) CPLMalloc(sizeof(float)*nXSize);

    print("Opening input and output files.");

    //creating output file
    GDALDriver * poDriver=GetGDALDriverManager()->GetDriverByName("GTiff");
    char ** options= NULL;

    GDALDataset * poDstDS=poDriver->Create(argv[2],nXSize,nYsize,1,GDT_Float32, NULL);

    //copying geographic header
    double * geotransform= new double[6];
    poDataset->GetGeoTransform(geotransform);
    poDstDS->SetGeoTransform(geotransform);
    delete geotransform;
    poDstDS->SetProjection(poDataset->GetProjectionRef());
    GDALRasterBand * poOutBand=poDstDS->GetRasterBand(1);
    float newval,idn,odn;

    //applying 6S transformation
    for (int j=0;j<nYsize;j++) {
        poBand->RasterIO( GF_Read, 0, j, nXSize, 1,pafScanline, nXSize, 1, GDT_Float32,0, 0 );
        for (int i=0;i<nXSize;i++) {
            idn=pafScanline[i]/255.;
            odn=transform(ti,REFLECTANCE,idn)*100;
            pafWriteline[i]=odn;
        }
        poOutBand->RasterIO( GF_Write, 0, j, nXSize, 1,pafWriteline, nXSize, 1, GDT_Float32,0, 0 );
    }
    GDALClose(poDstDS);
    GDALClose(poDataset);

    //odn=transform(ti,REFLECTANCE,idn)*100.;





}
