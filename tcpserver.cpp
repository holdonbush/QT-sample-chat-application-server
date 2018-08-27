#include "tcpserver.h"
#include "ui_tcpserver.h"
#include <QDebug>
#include <QByteArray>

TcpServer::TcpServer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TcpServer)
{
    ui->setupUi(this);

    port = 8010;
    tcpclient = new TcpClientSocket;
    ui->lineEdit->setText(QString::number(port));
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(slotCreateServer()));
}

TcpServer::~TcpServer()
{
    delete ui;
}

void TcpServer::slotCreateServer()
{
    server = new Server(this, port);
    connect(server,SIGNAL(updateServer(QString,int)),this,SLOT(updateServer(QString,int)));
    ui->pushButton->setEnabled(false);
}

void TcpServer::updateServer(QString msg, int length)
{
    ui->textBrowser->append(msg);
}

void TcpServer::receive(QString s)
{
    qDebug()<<s;
    ui->textBrowser->append(s);
}
