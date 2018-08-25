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

    //QString checkusrInfo(QString s);

public slots:
    void slotCreateServer();
    void updateServer(QString , int);
    //void readInfo();
    //QString returnread();

signals:
    //void aleadyread();
    //void datapass(QString);
private slots:
    void on_pushButton_2_clicked();

private:
    Ui::TcpServer *ui;
    int port;
    Server *server;

    QString str;
    QTcpSocket *tcpSocket;

    /*
    QSqlDatabase database;
    bool tableFlag;

    int usr_id;
    QString usr_passwd;
    QString usr_name;

    bool matchFlag;
    QString select_table = "select tbl_name name from sqlite_master where type = 'table'";
    QString create_sql = "create table user (chatid int primary key, passwd varchar(30), name varchar(30))";
    QString select_max_sql = "select max(chatid) from user";
    QString insert_sql = "insert into user values (?, ?, ?)";
    QString select_sql = "select name from user";

    QString select_nameInfo = "selcet * from user where name=";
    */

};

#endif // TCPSERVER_H
