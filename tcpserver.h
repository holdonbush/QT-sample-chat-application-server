#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QDialog>
#include "server.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

namespace Ui {
class TcpServer;
}

class TcpServer : public QDialog
{
    Q_OBJECT

public:
    explicit TcpServer(QWidget *parent = 0);
    ~TcpServer();


public slots:
    void slotCreateServer();
    void updateServer(QString , int);


private slots:
    void receive(QString s);
private:
    Ui::TcpServer *ui;
    int port;
    Server *server;
    TcpClientSocket *tcpclient;

    QString str;
    QTcpSocket *tcpSocket;

};

#endif // TCPSERVER_H
