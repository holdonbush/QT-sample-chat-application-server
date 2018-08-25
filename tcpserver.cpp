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
    ui->lineEdit->setText(QString::number(port));
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(slotCreateServer()));
    tcpSocket = new QTcpSocket(this);
    /*
    connect(this,SIGNAL(readyRead()),this,SLOT(readInfo()));

    tableFlag = false;

    //创建SQLITE数据库
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("user.db");

    //打开数据库
    if(!database.open()){
        qDebug()<<database.lastError();
        qFatal("failed to connect.") ;
    }
    else
    {
        qDebug()<<"open seccess";
        QSqlQuery sql_query;        //改变量必须在成功打开数据库后定义才有效
        sql_query.prepare(select_table);
        if(!sql_query.exec())
        {
            qDebug()<<sql_query.lastError();
        }
        else
        {
            QString tableName;
            while(sql_query.next())
            {
                tableName = sql_query.value(0).toString();
                qDebug()<<tableName;
                //数据库表不存在
                if(tableName.compare("user"))
                {
                    tableFlag=false;
                    qDebug()<<"table is not exist";
                }
                //数据库表已存在
                else
                {
                    tableFlag=true;
                    qDebug()<<"table is exist";
                }
            }
        }
        if(tableFlag==false)
        {
            //创建名为user的数据库表
            sql_query.prepare(create_sql);
            if(!sql_query.exec())
            {
                qDebug()<<sql_query.lastError();
            }
            else
            {
                qDebug()<<"table created!";
            }
        }
    }*/
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
    ui->listWidget->addItem(msg.left(length));
}
/*
void TcpServer::readInfo()
{

    str = QString::fromLocal8Bit(tcpSocket->readAll());
    if(str.length() > 15)
    {
        QString pwdinfo = checkusrInfo(str);
        tcpSocket->write(pwdinfo.toLatin1(),pwdinfo.length());
    }
    else
    {
        QSqlQuery sql_query(database);
        sql_query.prepare(insert_sql);
        sql_query.addBindValue(str);
    }
    emit aleadyread();
}

QString TcpServer::returnread()
{
    return str;
}

void TcpServer::checkusrInfo(QString s)
{
    QStringList list = s.split("/");
    QString str = list[0];
    QSqlQuery sql_query(database);
    QString tempstring = "select * from user where name='"+name+"'";
    qDebug()<<sql_query.lastError();
    if(!sql_query.exec(tempstring))
    {
        qDebug()<<sql_query.lastError();
        //matchFlag = false;
        return;
    }
    else
    {
        while(sql_query.next())
        {
            usr_id = sql_query.value(0).toInt();
            usr_passwd = sql_query.value(1).toString();
            usr_name = sql_query.value(2).toString();

            qDebug()<<QString("charid = %1   passwd=%2   name=%3").arg(usr_id).arg(usr_passwd).arg(usr_name);

        }
        if(usr_name==name) return usr_passwd;
        //else               matchFlag=false;
    }
}
*/

void TcpServer::on_pushButton_2_clicked()
{
    //tcpSocket = new QTcpSocket(this);
    QString str = ui->textEdit->toPlainText();
    tcpSocket->open(QIODevice::WriteOnly);
    tcpSocket->write(str.toLocal8Bit());
    tcpSocket->flush();
    //tcpSocket->flush();
    //emit datapass(str);
}
