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
#include "qapplication.h"
#include "qcheckbox.h"

#include "dntotoa.h"


class batchCor: public batchDialog{
    Q_OBJECT
public:
    batchCor(const char * homePath, QWidget* parent = 0, const char* name = 0,
                    bool modal = FALSE, WFlags fl = 0 );
    
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
