#include "batchcor.h"

batchCor::batchCor( QWidget *parent, QString homepath ): QDialog(parent) {
    setupUi(this);
    this->homePath=homepath.toStdString();
}

void batchCor::printText(const char* text) {
    //this->
}

void batchCor::run() {
    int continental;
    string visibility,atmMode;
    double heightSeaLevel;
    //QString atm;//=new QString();
    std::string filename, toaFileName, surfFileName,inpFileName, waterFileName;
    
    //QProcess *proc;
    DnToToa oneImageProcess(this->homePath.c_str());//=new DnToToa();
    int i;
    for (i=0; i<this->tableWidget->rowCount();i++) {
        //std::cout<<i<<this->table1->numRows()<<std::endl;
        atmMode= this->tableWidget->itemAt(i,2)->text().toStdString();
        continental= this->tableWidget->itemAt(i,3)->text().toInt();
        visibility=this->tableWidget->itemAt(i,4)->text().toStdString();
        heightSeaLevel=this->tableWidget->itemAt(i,5)->text().toDouble()*-0.001;

        filename=QDir(this->homePath.c_str()).filePath(this->tableWidget->itemAt(i,0)->text()).toStdString();
        
        
        //moves to the directory where the file is
        QFileInfo fInfo(filename.c_str());

        //std::string local=fInfo.absoluteDir();
        //QDir::setCurrent(local.c_str());
        //filename=fInfo.fileName().toStdString();
        //transform to reflectance TOA
        toaFileName=filename;
        toaFileName.replace(filename.find(".tif"),4,"-toa.tif");
        
        if (!oneImageProcess.DnToReflectance(filename.c_str(),atmMode,continental,visibility,heightSeaLevel,toaFileName.c_str())) {
            print("Problems converting to Top of Atmosphere reflectance.");
            return;
        }
        surfFileName=filename;
        surfFileName.replace(filename.find("."),9,"-surf.tif");
        inpFileName=filename;
        inpFileName.replace(filename.find("."),4,".inp");
        //transform to reflectance Sup
        oneImageProcess.Correction6S(toaFileName.c_str(),inpFileName.c_str(),surfFileName.c_str());
        if (this->waterCheckBox->isChecked()) {
            waterFileName=filename;
            waterFileName.replace(filename.find("."),10,"-water.tif");
            oneImageProcess.WaterCorrection(waterFileName.c_str(),surfFileName.c_str());
        }
        //convert to tiff and erase files;
        //oneImageProcess.CleanUp(local.c_str(), toaFileName.c_str(),inpFileName.c_str(),surfFileName.c_str());
    }
    QMessageBox::information( this, "XML Atmospheric Correction",
    "Process finished.\n"
    "BRASIL!" );

    
}

void batchCor::help() {
    QMessageBox::information( this, "Application name",    "Estamos sem ajuda.");
}

void batchCor::openTable() {
    QString dir =QFileDialog::getOpenFileName(this,"Abrir tabela",".",tr("Comma-separated values files (*.csv)"));
    std::ifstream infile(dir.toAscii());
    std::string line;
    char *l;
    QStringList pieces;
    this->tableWidget->setRowCount(0);
    int i=0;
    if (infile.is_open()) {
        while (!infile.eof()) {
            getline(infile,line);
            //std::cout<<line<<std::endl;
            if (line!="") {
                QString s=line.c_str();
                pieces=s.split(",");
                this->tableWidget->insertRow(i);
                for (int j=0;j<(pieces.size()); j++) {
                    QTableWidgetItem *newItem=new QTableWidgetItem(pieces[j]);
                    this->tableWidget->setItem(i,j,newItem);
                }
                
                //std::cout<<pieces.size()<<std::endl;
            }
            i++;
        }
    }
    infile.close();
    
}

void batchCor::saveTable() {
    QString dir =QFileDialog::getSaveFileName(this,"Salvar tabela como... ",".",tr("Comma-separated values files (*.csv)"));
    if (dir.indexOf(".csv")<0) dir.append(".csv");
    std::ofstream outfile(dir.toAscii());
    for (int i=0;i<tableWidget->rowCount();i++) {
        for (int j=0;j<tableWidget->columnCount();j++) {
            outfile<< tableWidget->item(i,j)->text().toStdString()<< ",";
            //cout<<tableWidget->item(i,j)->text().toStdString()<<endl;
            //compose a line
        }
        outfile<<std::endl;
    }
}

void batchCor::chooseDir() {
    //QString fileFilters = tr("Comma-separated values files (*.csv)\n");
    if (this->lineEdit1->text()=="") this->lineEdit1->setText(QDir::currentPath());

    QString dir =QFileDialog::getExistingDirectory(this,"Abrir diretorio", this->lineEdit1->text());
    //QString fileName = QFileDialog::getOpenFileName(".", fileFilters, this);
    
    if (!dir.isEmpty()) {
        QDir filesDir(dir);
        //list files in the directory
        QDir::Filter f=QDir::Files;
        QStringList tiffFiles = filesDir.entryList (QStringList("*.tif"),f);
        lineEdit1->setText(dir);
        
        //fills the table
        tableWidget->setRowCount(tiffFiles.size());
        for ( unsigned int i=0;i<tiffFiles.size();i++ ) {
            QTableWidgetItem *newItem = new QTableWidgetItem(filesDir.absoluteFilePath(tiffFiles[i]));
            tableWidget->setItem(i,0,newItem);
            QString xmlFile=tiffFiles[i];
            xmlFile.replace(".tif",".xml");
            QTableWidgetItem *newItem2;
            if (filesDir.exists(xmlFile)) {
                newItem2= new QTableWidgetItem("Encontrado");
            } else {
                newItem2= new QTableWidgetItem("Nao encontrado");
            }
            tableWidget->setItem(i,1,newItem2);
        }
    }
}


