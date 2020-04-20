// $Id: genericclient.cpp,v 1.2 2004/05/21 22:21:17 wright Exp $
#include <qapplication.h>

#include "genericclient.h"

#include <iostream>
using namespace std;


GenericClient::GenericClient(const QString &host,Q_UINT16 port,QString gpscmd,
                             QWidget *parent)
{

  // Create the socket and connect its signals
  socket = new QSocket( this );
  //socket->flush();
  connect(socket,SIGNAL(connectionClosed()),parent,SLOT(slotGPSClosed()));
  connect(socket,SIGNAL(readyRead()),this,SLOT(socketReadyRead()));
  connect(socket,SIGNAL(error(int)),parent,SLOT(slotGPSError(int)));

  // Connect to the server
  socket->connectToHost(host,port);

  // Send the starting string
  socket->writeBlock(gpscmd,13);

}


GenericClient::~GenericClient()
{
}


void GenericClient::closeConnection()
{
  socket->close();
}


void GenericClient::socketReadyRead()
{

  // Read a line from the server and send the most recent one
  // We only emit a $GPGGA or message10 format line; ignore all others
  message_good = false;
  while ( socket->canReadLine() )
  {
    stemp = socket->readLine();
    stemp0 = stemp.section(',',0,0);
    stemp1 = stemp.section(',',1,1);
    stemp2 = stemp.section(',',2,2);
    stemp3 = stemp.section(',',3,3);
    stemp4 = stemp.section(',',4,4);
    stemp5 = stemp.section(',',5,5);
    stemp9 = stemp.section(',',9,9);
    if (stemp0=="$GPGGA" && !stemp1.isEmpty() && !stemp2.isEmpty() &&
        !stemp3.isEmpty() && !stemp4.isEmpty() && !stemp5.isEmpty() &&
        !stemp9.isEmpty())
    {
      line = stemp;
      message_good = true;
    }
    else if (stemp0=="10")
    {
      line = stemp;
      message_good = true;
    }
    else if (stemp0=="11")
    {
      line = stemp;
      message_good = true;
    }
  }

  // If we got a valid GGA or message10 record, make it available
  if (message_good)
  {
    emit gotaline(line);
  }

}
