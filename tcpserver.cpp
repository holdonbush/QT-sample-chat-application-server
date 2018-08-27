#include "tcpserver.h"
#include "ui_tcpserver.h"
#include <QDebug>
#include <QByteArray>

TcpServer::TcpServer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TcpServer)
{
    ui->setupUi(this);
    setWindowTitle("Server");
    setWindowIcon(QPixmap(":/img/server.png"));

    port = 8010;
    tcpclient = new TcpClientSocket;
    ui->lineEdit->setText(QString::number(port));
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(slotCreateServer()));
}

TcpServer::~TcpServer()
{
    delete ui;
}

/*
 * 函数名：slotCreateServer
 * 功能：创建一个TcpServer
 * 参数：NULL
 * 返回值：void
 */
void TcpServer::slotCreateServer()
{
    server = new Server(this, port);
    connect(server,SIGNAL(updateServer(QString,int)),this,SLOT(updateServer(QString,int)));
    ui->pushButton->setEnabled(false);
}

/*
 * 函数名：updateServer
 * 功能：更新内容
 * 参数：QString , int
 * 返回值：void
 */
void TcpServer::updateServer(QString msg, int length)
{
    ui->textBrowser->append(msg);
}

void TcpServer::receive(QString s)
{
    qDebug()<<s;
    ui->textBrowser->append(s);
}
