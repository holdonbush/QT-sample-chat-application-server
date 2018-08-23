#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QDialog>
#include "server.h"

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

private:
    Ui::TcpServer *ui;
    int port;
    Server *server;
};

#endif // TCPSERVER_H
