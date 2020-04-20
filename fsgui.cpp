// $Id: soxgui.cpp,v 1.9 2004/08/08 02:44:23 sonntag Exp $
// Fsgui class implementation
//

#include <qapplication.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qinputdialog.h>

#include "fsgui.h"
#include "setupgui.h"

#include <stdlib.h>

#include <iostream>
using namespace std;

Fsgui::Fsgui()
{

  // Get the saved user settings
  gpshost = usettings.readEntry("/astrogeoserver/network/gpshost","localhost");
  gpsport = (Q_UINT16)(usettings.readNumEntry("/astrogeoserver/network/gpsport",
            4052));
  gpscmd = usettings.readEntry("/astrogeoserver/network/gpscmd","gga1hzstream");

  // Get network settings from user
  Setupgui *setup = new Setupgui(&gpshost,&gpsport,&gpscmd,this);
  while (setup->exec() == QDialog::Rejected)
  {
    QMessageBox::warning(this,"Invalid\n","Port number is invalid\n");
  }
  if (!gpscmd.contains('\n'))
    gpscmd.append('\n');

  // Initialize the geoid and topo databases
  initquerygeoid();
  initquerytopo();

  // Build the main menubar and make connections
  mainmenu = new QMenuBar(this);
  filemenu = new QPopupMenu(mainmenu);
  setmenu  = new QPopupMenu(mainmenu);
  QPopupMenu *helpmenu = new QPopupMenu(mainmenu);
  filemenu->insertItem("Quit",qApp,SLOT(quit()));
  helpmenu->insertItem("About",this,SLOT(about()));
  mainmenu->insertItem("File",filemenu);
  mainmenu->insertItem("Settings",setmenu);
  mainmenu->insertItem("Help",helpmenu);

  // Build the main layout for the gui
  QVBox *mainbox  = new QVBox(this);
  QVGroupBox *dtbox = new QVGroupBox("Date and time (UTC)",mainbox);
  dtlab = new QLabel(dtbox);
  dtlab->setFont(QFont("Helvetica",14,QFont::Bold));
  dtlab->setAlignment(AlignHCenter);
  QHBox *llbox = new QHBox(mainbox);
  QVGroupBox *latbox = new QVGroupBox("Latitude (deg)",llbox);
  latlab = new QLabel(latbox);
  latlab->setFont(QFont("Helvetica",14,QFont::Bold));
  latlab->setAlignment(AlignHCenter);
  QVGroupBox *lonbox = new QVGroupBox("Longitude (deg)",llbox);
  lonlab = new QLabel(lonbox);
  lonlab->setFont(QFont("Helvetica",14,QFont::Bold));
  lonlab->setAlignment(AlignHCenter);
  QHBox *sunbox = new QHBox(mainbox);
  QVGroupBox *sunazbox = new QVGroupBox("Sun azimuth (deg)",sunbox);
  sunazlab = new QLabel(sunazbox);
  sunazlab->setFont(QFont("Helvetica",14,QFont::Bold));
  sunazlab->setAlignment(AlignHCenter);
  QVGroupBox *sunelbox = new QVGroupBox("Sun elevation (deg)",sunbox);
  sunellab = new QLabel(sunelbox);
  sunellab->setFont(QFont("Helvetica",14,QFont::Bold));
  sunellab->setAlignment(AlignHCenter);
  QVGroupBox *htellbox = new QVGroupBox("Altitude relative to ellipsoid (ft)",mainbox);
  htelllab = new QLabel(htellbox);
  htelllab->setFont(QFont("Helvetica",14,QFont::Bold));
  htelllab->setAlignment(AlignHCenter);
  QVGroupBox *geoidbox = new QVGroupBox("Geoid height relative to ellipsoid (ft)",mainbox);
  geoidlab = new QLabel(geoidbox);
  geoidlab->setFont(QFont("Helvetica",14,QFont::Bold));
  geoidlab->setAlignment(AlignHCenter);
  QVGroupBox *topobox = new QVGroupBox("Topo height relative to geoid (ft)",mainbox);
  topolab = new QLabel(topobox);
  topolab->setFont(QFont("Helvetica",14,QFont::Bold));
  topolab->setAlignment(AlignHCenter);
  QVGroupBox *htgbox = new QVGroupBox("Altitude relative to geoid (ft)",mainbox);
  htglab = new QLabel(htgbox);
  htglab->setFont(QFont("Helvetica",14,QFont::Bold));
  htglab->setAlignment(AlignHCenter);
  QVGroupBox *aglbox = new QVGroupBox("Altitude AGL (ft)",mainbox);
  agllab = new QLabel(aglbox);
  agllab->setFont(QFont("Helvetica",14,QFont::Bold));
  agllab->setAlignment(AlignHCenter);

  // Build a timer for computing astronomical quantities
  // No need to do this at each position update
  astrotimer = new QTimer(this);
  connect(astrotimer,SIGNAL(timeout()),SLOT(slotAstroUpdate()));
  astrotimer->start(1000); // Once per second

  // Build the network client
  //cout << gpshost << "  " << gpsport << "\n";
  client = new GenericClient(gpshost,gpsport,gpscmd,this);
  connect(client,SIGNAL(gotaline(QString)),this,SLOT(slotNewGPS(QString)));

}


Fsgui::~Fsgui()
{

  // Close the geoid and topo databases
  closequerygeoid();
  closequerytopo();

  // Close the network connection
  client->closeConnection();

  // Save user settings
  usettings.writeEntry("/astrogeoserver/network/gpshost",gpshost);
  usettings.writeEntry("/astrogeoserver/network/gpsport",gpsport);
  usettings.writeEntry("/astrogeoserver/network/gpscmd",gpscmd);

  // Delete the objects still out there
  delete mainmenu;

}


void Fsgui::slotAstroUpdate()
{

  // Get sun angle at current position
  getsunangle(lat*DEG2RAD,lon*DEG2RAD,h_ell,year,month,day,hour,min,sec,&sunazac,&sunelac);
  //getsunangle(lat*DEG2RAD,lon*DEG2RAD,0.0,year,month,day,hour,min,sec,&sunazgnd,&sunelgnd);

  // Update the sun az/el fields
  if (year==9999)
  {
    stemp.sprintf("---.--");
    sunazlab->setText(stemp);
    stemp.sprintf("--.--");
    sunellab->setText(stemp);
  }
  else
  {
    stemp.sprintf("%06.2lf",sunazac/DEG2RAD);
    sunazlab->setText(stemp);
    stemp.sprintf("%6.2lf",sunelac/DEG2RAD);
    sunellab->setText(stemp);
  }

}


void Fsgui::slotNewGPS(QString gps)
{

  // Determine the message type and parse as appropriate
  stemp = gps.section(',',0,0);
  if (stemp=="11")
  {
    stemp = gps.section(',',1,1);
    ymd = stemp.toDouble();
    parseymd(ymd,&year,&month,&day);
    stemp = gps.section(',',2,2);
    hms = stemp.toDouble();
    parsehms(hms,&hour,&min,&sec);
    stemp = gps.section(',',3,3);
    lat = stemp.toDouble();
    stemp = gps.section(',',4,4);
    lon = stemp.toDouble();
    stemp = gps.section(',',5,5);
    h_ell = stemp.toDouble();
  }
  else if (stemp=="10")
  {
    stemp = gps.section(',',1,1);
    wks = stemp.toDouble();
    sec2hms(wks,&hour,&min,&sec);
    year = 9999;
    month = 99;
    day = 99;
    stemp = gps.section(',',2,2);
    lat = stemp.toDouble();
    stemp = gps.section(',',3,3);
    lon = stemp.toDouble();
    stemp = gps.section(',',4,4);
    h_ell = stemp.toDouble();
  }
  else
  {
    cout << "Unrecognized network message type - exiting\n";
    exit(-1);
  }

  // Update the date/time field
  if (year==9999)
    stemp.sprintf("----------- %02d:%02d:%04.1lf",hour,min,sec);
  else
    stemp.sprintf("%04d-%02d-%02d   %02d:%02d:%04.1lf",year,month,day,hour,min,sec);
  dtlab->setText(stemp);

  // Update the lat and lon fields
  stemp.sprintf("%8.4lf",lat);
  latlab->setText(stemp);
  stemp.sprintf("%8.4lf",lon);
  lonlab->setText(stemp);

  // Update the ht-ellipsoid field
  stemp.sprintf("%5.0lf",h_ell);
  htelllab->setText(stemp);

  // Update height of geoid relative to height of ellipsoid
  while (lon<=-180.0) lon+=360.0;
  while (lon>180.0) lon-=360.0;
  geoid = querygeoid(lat,lon,geoidid)*3.28;
  stemp.sprintf("%4.0lf\t(%3s)",geoid,geoidid);
  geoidlab->setText(stemp);

  // Update height of topography, relative to the geoid
  topo = querytopo(lat,lon,demid)*3.28;
  if (!strncmp(demid,"G90\0",3)) topo=topo-geoid;
  stemp.sprintf("%5.0lf\t(%3s)",topo,demid);
  topolab->setText(stemp);

  // Update the altitude rel to geoid
  h_geoid = h_ell-geoid;
  stemp.sprintf("%5.0lf",h_geoid);
  htglab->setText(stemp);

  // Update the AGL
  double h_agl = h_geoid-topo;
  stemp.sprintf("%5.0lf",h_agl);
  agllab->setText(stemp);

}


void Fsgui::slotGPSClosed()
{
  stemp.sprintf("Connection to GPS network server closed by the server");
  QMessageBox::critical(this,"GPS Connection Closed",stemp);
}



void Fsgui::slotGPSError(int err)
{
  if (err==QSocket::ErrConnectionRefused)
    stemp.sprintf("Connection to GPS server refused\n");
  else if (err==QSocket::ErrHostNotFound)
    stemp.sprintf("GPS server host not found\n");
  else if (err==QSocket::ErrSocketRead)
    stemp.sprintf("Socket read from GPS server failed");
  QMessageBox::critical(this,"GPS Network Error",stemp);
}


void Fsgui::about()
{
  QMessageBox::about(this,"About ASTROGEOSERVER",
                     "ASTROGEOSERVER\nAuthor: John G. Sonntag\n"
		     "Released 11 November 2014\n"
                     "Last Updated 11 November 2014\n"
		    );
}

