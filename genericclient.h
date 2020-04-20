// $Id: genericclient.h,v 1.2 2004/05/21 22:21:17 wright Exp $
// The Client class provides a small GUI for the client, establishes a
// connection, and listens for incoming data.
//

#ifndef GENERICCLIENT_H
#define GENERICCLIENT_H

#include <qvbox.h>
#include <qsocket.h>
#include <qtextedit.h>
#include <qtextview.h>


class GenericClient : public QVBox
{

  Q_OBJECT

  public:
    GenericClient( const QString &host, Q_UINT16 port, QString gpscmd,
                   QWidget *parent=0 );  // Constructor
    ~GenericClient();  // Destructor

  signals:
    void gotaline(QString);

  public slots:
    void closeConnection();  // Close connection

  private slots:
    void socketReadyRead();  // Read a line from the socket

  private:
    QSocket *socket;
    QString line;
    QString stemp,stemp0,stemp1,stemp2,stemp3,stemp4,stemp5,stemp9;
    bool message_good;

};

#endif
