#include "server.h"

Server::Server(QObject *parent,int port) : QTcpServer(parent)
{
    listen(QHostAddress::Any,port);
}

/*
 * 函数名：incomingConnection
 * 功能：存储新连接的套接字，建立其它connect
 * 参数：int
 * 返回值：void
 */
void Server::incomingConnection(int socketDescriptor)
{
    TcpClientSocket *tcpClientSocket = new TcpClientSocket(this);

    connect(tcpClientSocket,SIGNAL(updateClients(QString,int)),this,SLOT(updateClients(QString,int)));
    connect(tcpClientSocket,SIGNAL(disconnected(int)),this,SLOT(slotDisconnected(int)));
    tcpClientSocket->setSocketDescriptor(socketDescriptor);

    tcpClientSocketList.append(tcpClientSocket);
}

/*
 * 函数名：updateClients
 * 功能：更新内容
 * 参数：QString,int
 * 返回值：void
 */
void Server::updateClients(QString msg, int length)
{
    //传输文件时进行的判断，目的是将文件传输到对应的用户
    if(msg.contains("+"))
    {
        QStringList str = msg.split("+");
        QString str1 = str[0];
        for(int i = 0; i < tcpClientSocketList.count();i++)
        {
            QTcpSocket *item = tcpClientSocketList.at(i);
            if(item->write(str1.toLatin1(),length)!=length)
            {
                continue;
            }
        }
    }
    else
    {
        emit updateServer(msg,length);
        //QString msg1 = msg+"\n";
        for(int i = 0; i < tcpClientSocketList.count();i++)
        {
            QTcpSocket *item = tcpClientSocketList.at(i);
            if(item->write(msg.toLatin1(),length)!=length)
            {
                continue;
            }
        }
    }
}


void Server::slotDisconnected(int descriptor)
{
    for(int i = 0; i < tcpClientSocketList.count();i++)
    {
        QTcpSocket *item = tcpClientSocketList.at(i);
        if(item->socketDescriptor()==descriptor)
        {
            tcpClientSocketList.removeAt(i);
            return;
        }
    }
    return;
}


