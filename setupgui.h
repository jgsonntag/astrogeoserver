// $Id: setupgui.h,v 1.2 2004/05/21 22:21:17 wright Exp $
// Setupgui class definition
//

#ifndef SETUPGUI_H
#define SETUPGUI_H

#include <qdialog.h>
#include <qstring.h>
#include <qvalidator.h>
#include <qlineedit.h>
//#include <qlabel.h>

class Setupgui : public QDialog
{

  Q_OBJECT

  public:
    Setupgui(QString *gpshost,Q_UINT16 *gpsport,QString *gpscmd,
             QWidget *parent,const char *name=0);  // Constructor
    ~Setupgui();  // Destructor

  private slots:
    void testInputs();

  private:
    Q_UINT16 *gpsport1;
    QString *gpshost1;
    QIntValidator *gpsportval;
    QString *gpscmd1;
    QString stemp;
    QLineEdit *gpsportedit;
    QLineEdit *gpshostedit;
    QLineEdit *gpscmdedit;

};

#endif // SETUPGUI_H
