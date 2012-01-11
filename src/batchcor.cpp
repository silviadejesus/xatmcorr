#include "batchcor.h"

batchCor::batchCor( const char * homePath, QWidget* parent, const char* name, bool modal, WFlags fl ):batchDialog(parent, name, modal, fl) {
    this->homePath=homePath;
}

void batchCor::printText(const char* text) {
    //this->
}

void batchCor::run() {
    int atmMode, continental;
    string visibility;
    double heightSeaLevel;
    //QString atm;//=new QString();
    std::string filename, toaFileName, surfFileName,inpFileName;
    
    //QProcess *proc;
    DnToToa oneImageProcess(this->homePath.c_str());//=new DnToToa();
    int i;
    for (i=0; i<this->table1->numRows();i++) {
        //std::cout<<i<<this->table1->numRows()<<std::endl;
        atmMode=this->table1->text(i,2).toInt();
        continental=this->table1->text(i,3).toInt();
        visibility=this->table1->text(i,4).ascii();
        heightSeaLevel=this->table1->text(i,5).toDouble()*-0.001;
        filename=this->table1->text(i,0).ascii();
        
        
        //moves to the directory where the file is
        QFileInfo fInfo(filename.c_str());
        std::string local=fInfo.dirPath().ascii();
        QDir::setCurrent(local.c_str());
        filename=fInfo.fileName().ascii();
        //transform to reflectance TOA
        toaFileName=filename;
        toaFileName.replace(filename.find("."),4,"-toa");
        
        if (!oneImageProcess.DnToReflectance(filename.c_str(),atmMode,continental,visibility,heightSeaLevel,toaFileName.c_str())) {
            print("Problems converting to Top of Atmosphere reflectance.");
            return;
        }
        surfFileName=filename;
        surfFileName.replace(filename.find("."),5,"-surf");
        inpFileName=filename;
        inpFileName.replace(filename.find("."),4,".inp");
        //transform to reflectance Sup
        oneImageProcess.Correction6S(toaFileName.c_str(),inpFileName.c_str(),surfFileName.c_str());
        
        //convert to tiff and erase files;
        oneImageProcess.CleanUp(local.c_str(), toaFileName.c_str(),inpFileName.c_str(),surfFileName.c_str());
    }
    QMessageBox::information( this, "XML Atmospheric Correction",
    "Process finished.\n"
    "BRASIL!" );

    
}

void batchCor::help() {
    QMessageBox::information( this, "Application name",    "Estamos sem ajuda.");
}

void batchCor::openTable() {
    QString dir =QFileDialog::getOpenFileName(".",tr("Comma-separated values files (*.csv)"),this);
    std::ifstream infile(dir.latin1());
    std::string line;
    char *l;
    QStringList pieces;
    this->table1->setNumRows(0);
    int i=0;
    if (infile.is_open()) {
        while (!infile.eof()) {
            getline(infile,line);
            //std::cout<<line<<std::endl;
            if (line!="") {
                pieces=QStringList::split(",",line);
                this->table1->insertRows(i);
                for (int j=0;j<(pieces.size()); j++) {
                    this->table1->setText(i,j,pieces[j]);
                }
                
                //std::cout<<pieces.size()<<std::endl;
            }
            i++;
        }
    }
    infile.close();
    
}

void batchCor::saveTable() {
    QString dir =QFileDialog::getSaveFileName(".",tr("Comma-separated values files (*.csv)"),this);
    if (dir.find(".csv")<0) dir.append(".csv");
    std::ofstream outfile(dir.latin1());
    for (int i=0;i<table1->numRows();i++) {
        for (int j=0;j<table1->numCols();j++) {
            outfile<< table1->text(i,j).ascii()<< ",";
            //compose a line
        }
        outfile<<std::endl;
    }
}

void batchCor::chooseDir() {
    //QString fileFilters = tr("Comma-separated values files (*.csv)\n");
    if (this->lineEdit1->text()=="") this->lineEdit1->setText(QDir::currentDirPath());
    QString dir =QFileDialog::getExistingDirectory(this->lineEdit1->text(),this);
    //QString fileName = QFileDialog::getOpenFileName(".", fileFilters, this);
    
    if (!dir.isEmpty()) {
        QDir filesDir(dir);
        //list files in the directory
        QStringList tiffFiles = filesDir.entryList ("*.tif");
        lineEdit1->setText(dir);
        
        //fills the table
        table1->setNumRows(tiffFiles.size());
        for ( unsigned int i=0;i<tiffFiles.size();i++ ) {
            table1->setText(i,0,filesDir.absFilePath(tiffFiles[i]));
            QString xmlFile=tiffFiles[i];
            xmlFile.replace(".tif",".xml");
            if (filesDir.exists(xmlFile)) {
                table1->setText(i,1,"Encontrado");
            } else {
                table1->setText(i,1,"Nao encontrado");
            }
        }
    }
}


