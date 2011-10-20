#include "batchcor.h"

batchCor::batchCor( QWidget* parent, const char* name, bool modal, WFlags fl ):batchDialog(parent, name, modal, fl) {

}



void batchCor::run() {
    QProcess *proc;
    for (int i=0; i<table1->numRows();i++) {
        //transform to reflectance TOA
        
        //transform to reflectance Sup
        proc = new QProcess( this );
        proc->addArgument( "6S.exe" );
        proc->addArgument(table1->text(0,i));
        //connect( proc, SIGNAL(readyReadStdout()),this, SLOT(readFromStdout()) );

        if ( !proc->start() ) {
            //wait
        }
        //convert to tiff
        
        delete proc;
    }
    

    
}

void batchCor::help() {
    QMessageBox::information( this, "Application name",    "Estamos sem ajuda.");
}

void batchCor::openTable() {
    QString dir =QFileDialog::getOpenFileName(".",tr("Comma-separated values files (*.csv)"),this);
}

void batchCor::saveTable() {
    QString dir =QFileDialog::getSaveFileName(".",tr("Comma-separated values files (*.csv)"),this);
    for (int i=0;i<table1->numRows();i++) {
        for (int j=0;j<table1->numCols();j++) {
            //compose a line
        }
    }
}

void batchCor::chooseDir() {
    //QString fileFilters = tr("Comma-separated values files (*.csv)\n");
    QString dir =QFileDialog::getExistingDirectory(".",this);
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