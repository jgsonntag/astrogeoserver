// $Id: soxgui.h,v 1.5 2004/08/08 02:44:23 sonntag Exp $
// Fsgui class definition
//

#ifndef FSGUI_H
#define FSGUI_H

#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qvbox.h>
#include <qfile.h>
//#include <qtextstream.h>
#include <qsettings.h>
#include <qlabel.h>
#include <qvgroupbox.h>
#include <qtimer.h>
//#include <qlistbox.h>
//#include <qtable.h>
//#include <qpushbutton.h>

#include "genericclient.h"
#include "utility.h"
#include "querytopo.h"
#include "sunangle.h"

#define FT2M (12.0*2.54/100.0)


class Fsgui : public QVBox
{

  Q_OBJECT

  public:
    Fsgui();  // Constructor
    ~Fsgui();  // Destructor

  private:
    double hms,lat,lon,h_ell,sec;
    int year,month,day,hour,min;
    QMenuBar *mainmenu;
    QPopupMenu *filemenu;
    QPopupMenu *setmenu;
    GenericClient *client;
    QString stemp;
    QString gpshost;
    Q_UINT16 gpsport;
    QString gpscmd;
    long int ymd;
    QSettings usettings;
    QLabel *dtlab;
    double wks;
    QLabel *timelab;
    QLabel *latlab;
    QLabel *lonlab;
    QLabel *htelllab;
    QLabel *geoidlab;
    QLabel *topolab;
    QLabel *htglab;
    QLabel *agllab;
    QLabel *sunazlab;
    QLabel *sunellab;
    double geoid,topo;
    double h_geoid,sunazac,sunelac,sunazgnd,sunelgnd;
    char geoidid[10],demid[10];
    QTimer *astrotimer;

  private slots:
    void about();
    void slotNewGPS(QString);
    void slotGPSClosed();
    void slotGPSError(int);
    void slotAstroUpdate();

};

#endif // FSGUI_H
