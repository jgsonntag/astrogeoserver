// $Id: setupgui.cpp,v 1.2 2004/05/21 22:21:17 wright Exp $
// Setupgui class implementation
//

//#include <qapplication.h>
#include <qlineedit.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qlabel.h>
#include <qwidget.h>
#include <qstring.h>
#include <qpushbutton.h>
#include <qgrid.h>
#include <qvalidator.h>

#include "setupgui.h"
//#include "fsgui.h"

#include <iostream>
using namespace std;

Setupgui::Setupgui(QString *gpshost,Q_UINT16 *gpsport,QString *gpscmd,
                   QWidget *parent,const char *name)
	: QDialog(parent,name)
{

  // Set the overall layout
  QVBox *all = new QVBox(this);
  all->setMinimumSize(280,150);
  new QLabel("Please examine the values below\n"
             "and adjust if necessary;\n"
             "MESSAGE10/11 format expected\n",all);
  QGrid *grid = new QGrid(2,all);

  // GPS host
  stemp = *gpshost;
  gpshost1 = gpshost;
  gpshostedit = new QLineEdit(grid);
  gpshostedit->setText(stemp);
  new QLabel(" GPS network host",grid);

  // GPS port number
  gpsportval = new QIntValidator(0,9999,this);
  stemp.setNum(*gpsport);
  gpsport1 = gpsport;
  gpsportedit = new QLineEdit(grid);
  gpsportedit->setText(stemp);
  gpsportedit->setValidator(gpsportval);
  new QLabel(" GPS network port",grid);

  // GPS starting command
  stemp = *gpscmd;
  gpscmd1 = gpscmd;
  gpscmdedit = new QLineEdit(grid);
  gpscmdedit->setText(stemp);
  new QLabel(" GPS starting command",grid);

  // Continue button
  QPushButton *cont = new QPushButton("Continue to ASTROGEOSERVER",all);
  connect(cont,SIGNAL(clicked()),SLOT(testInputs()));

}


Setupgui::~Setupgui()
{
}


void Setupgui::testInputs()
{
  int pos;
  QString stemp = gpsportedit->text();
  bool gpisvalid = (gpsportval->validate(stemp,pos)==QValidator::Acceptable);
  if (gpisvalid)
  {
    stemp = gpsportedit->text();
    *gpsport1 = stemp.toInt();
    *gpshost1 = gpshostedit->text();
    *gpscmd1  = gpscmdedit->text();
    accept();
  }
  else
    reject();
}
