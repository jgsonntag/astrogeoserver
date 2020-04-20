//
// $Id: main.cpp,v 1.4 2004/08/07 13:38:07 sonntag Exp $
//  Main program for FLIGHTSTATUS
//

#include <qapplication.h>
#include "fsgui.h"
#include <string.h>
//#include "soxmap.h"

//extern void options( int argc, char *argv[]);
//extern SETTINGS settings;

int main( int argc, char *argv[] )
{
  //options(argc, argv);
  QApplication app( argc, argv );

  // Transfer control to the GUI
  Fsgui gui;
  app.setMainWidget( &gui );
  gui.show();
  return app.exec();
}

