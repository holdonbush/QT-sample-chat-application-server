#include <QTcpSocket>
#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

class TcpClientSocket : public QTcpSocket
{
    Q_OBJECT
public:
    TcpClientSocket(QObject *parent = 0);

    QString checkusrInfo(QString s);

signals:
    void updateClients(QString, int);
    void disconnected(int);
protected slots:
    void dataReceived();
    void slotDisconnected();
    //void receivestr(QString s);
private:
    QSqlDatabase database;
    //QTcpSocket *tcpSocket;
    bool tableFlag;

    int usr_id;
    QString usr_passwd;
    QString usr_name;

    bool matchFlag;
    QString select_table = "select tbl_name name from sqlite_master where type = 'table'";
    QString create_sql = "create table user ( passwd varchar(30), name varchar(30))";
    QString select_max_sql = "select max(chatid) from user";
    QString insert_sql = "insert into user values (?, ?)";
    QString select_sql = "select name from user";

    QString select_nameInfo = "selcet * from user where name=";
};
