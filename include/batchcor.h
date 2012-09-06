#ifndef BATCHCOR_H
#define BATCHCOR_H


#include "ui_batchDialog.h"

#include <QFileDialog>
#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <QDir>
//#include "qlineedit.h"
#include <QTableWidget>
#include <QFileInfo>
//#include <QProcess>

//#include "qcheckbox.h"

#include "dntotoa.h"


class batchCor: public QDialog, private Ui::batchDialog{
    Q_OBJECT
public:
    batchCor(QWidget *parent = 0,QString homepath="");
    
private:
    std::string homePath;
public slots:
    void run();
    void help();
    void saveTable();
    void openTable();
    void chooseDir();
    void printText(const char* text);
    
};

#endif
