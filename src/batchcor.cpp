#include "batchcor.h"

batchCor::batchCor( QWidget* parent, const char* name, bool modal, WFlags fl ):batchDialog(parent, name, modal, fl) {

}



void batchCor::run() {
    int atmMode, continental;
    double visibility;
    double heightSeaLevel;
    //QString atm;//=new QString();
    std::string filename, toaFileName, surfFileName,inpFileName;
    
    //QProcess *proc;
    DnToToa oneImageProcess;//=new DnToToa();
    int i;
    for (i=0; i<this->table1->numRows();i++) {
        std::cout<<i<<this->table1->numRows()<<std::endl;
        atmMode=this->table1->text(i,2).toInt();
        continental=this->table1->text(i,3).toInt();
        visibility=this->table1->text(i,4).toDouble();
        heightSeaLevel=this->table1->text(i,5).toDouble();
        filename=this->table1->text(i,0).ascii();
        
        
        //transform to reflectance TOA
        toaFileName=filename;
        toaFileName.replace(filename.find("."),4,"-toa");
        oneImageProcess.DnToReflectance(filename.c_str(),atmMode,continental,visibility,heightSeaLevel,toaFileName.c_str());
        surfFileName=filename;
        surfFileName.replace(filename.find("."),5,"-surf");
        inpFileName=filename;
        inpFileName.replace(filename.find("."),4,".inp");
        //transform to reflectance Sup
        oneImageProcess.Correction6S(toaFileName.c_str(),inpFileName.c_str(),surfFileName.c_str());
        //convert to tiff
        
        //delete proc;
    }
    

    
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
            std::cout<<line<<std::endl;
            if (line!="") {
                pieces=QStringList::split(",",line);
                this->table1->insertRows(i);
                for (int j=0;j<(pieces.size()); j++) {
                    this->table1->setText(i,j,pieces[j]);
                }
                
                std::cout<<pieces.size()<<std::endl;
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
    QString dir =QFileDialog::getExistingDirectory(this->lineEdit1->text(),this);
    //QString fileName = QFileDialog::getOpenFileName(".", fileFilters, this);
    
    if (!dir.isEmpty()) {
        lineEdit1->setText(dir);
        QStringList tiffFiles;
        listDir(dir,tiffFiles);
        table1->setNumRows(tiffFiles.size());
        for ( int i=0;i<tiffFiles.size();i++ ) {
            table1->setText(i,0,tiffFiles[i]);
            QFileInfo file(dir,tiffFiles[i].replace(".tif",".xml"));
            if (file.exists()) {
                table1->setText(i,1,"Ok");
            } else {
                table1->setText(i,1,"Nao encontrado");
            }
        }
    }
}

void batchCor::listDir(QString dir, QStringList &tiffList) {
    QDir myDir(dir);
    tiffList = myDir.entryList ("*.tif");     // filter only c++ files
}

