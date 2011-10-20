#include "batchDialog.h"
#include "qfiledialog.h"
#include "qstring.h"
#include "qstringlist.h"
#include "qmessagebox.h"
#include "qlineedit.h"
#include "qdir.h"
#include "qtable.h"
#include "qfileinfo.h"
#include "qprocess.h"



class batchCor: public batchDialog{
    Q_OBJECT
public:
    batchCor(QWidget* parent = 0, const char* name = 0,
                    bool modal = FALSE, WFlags fl = 0 );
private:
    void listDir(QString dir, QStringList &tiffList);
public slots:
    void run();
    void help();
    void saveTable();
    void openTable();
    void chooseDir();
    
};