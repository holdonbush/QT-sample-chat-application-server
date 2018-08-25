#include "tcpclientsocket.h"

TcpClientSocket::TcpClientSocket(QObject *parent)
{
    connect(this,SIGNAL(readyRead()),this,SLOT(dataReceived()));
    connect(this,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));

    //tcpSocket = new QTcpSocket(this);
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
    }
}

void TcpClientSocket::dataReceived()
{
    /*while(bytesAvailable()>0)
    {
        qDebug()<<"接收";
        int length = bytesAvailable();
        char buf[1024];
        read(buf,length);
        QString msg = buf;
        emit updateClients(msg,length);

        if(msg.length() > 15)
        {
            QString pwdinfo = checkusrInfo(msg);
            this->write(pwdinfo.toLatin1(),pwdinfo.length());
        }
        else
        {
            QSqlQuery sql_query(database);
            sql_query.prepare(insert_sql);
            sql_query.addBindValue(msg);
        }
    }*/

    QByteArray buffer;
    buffer = this->readAll();
    QString str = QString::fromLocal8Bit(buffer);
    qDebug()<<str;
    if(str.contains("/",Qt::CaseInsensitive))
    {
        QStringList strlist = str.split("/");
        QString str_1 = strlist[0];
        QString str_2 = strlist[1];
        if(str_2 != "-")
        {
            qDebug()<<str_1;
            qDebug()<<str_2;
            qDebug()<<str_2[0];
            QSqlQuery sql_query(database);
            sql_query.prepare(insert_sql);
            sql_query.addBindValue(str_1);
            sql_query.addBindValue(str_2);
            qDebug()<<"success";
        }
        else
        {
            qDebug()<<"username";
            QString pwdinfo = checkusrInfo(str_1);
            this->write(pwdinfo.toLatin1(),pwdinfo.length());
        }
        //QString str = QString::fromLocal8Bit(tcpSocket->readAll());
        /*int length = str.length();
        if(str.length() > 15)
        {
            QString pwdinfo = checkusrInfo(str);
            this->write(pwdinfo.toLatin1(),pwdinfo.length());
        }
        else
        {
            QSqlQuery sql_query(database);
            sql_query.prepare(insert_sql);
            sql_query.addBindValue(str);
        }*/
        emit updateClients(str,str.length());
    }
    else {
        emit updateClients(str,str.length());
    }

}

void TcpClientSocket::slotDisconnected()
{
    emit disconnected(this->socketDescriptor());
}

QString TcpClientSocket::checkusrInfo(QString s)
{
    qDebug()<<"here";
    //QStringList list = s.split("/");
    //QString str = list[0];
    QSqlQuery sql_query(database);
    QString tempstring = "select * from user where name='"+s+"'";
    qDebug()<<sql_query.lastError();
    if(!sql_query.exec(tempstring))
    {
        qDebug()<<sql_query.lastError();
        //matchFlag = false;
        return NULL;
    }
    else
    {
        while(sql_query.next())
        {
            //usr_id = sql_query.value(0).toInt();
            usr_passwd = sql_query.value(0).toString();
            usr_name = sql_query.value(1).toString();

            qDebug()<<QString("passwd=%1   name=%2").arg(usr_passwd).arg(usr_name);

        }
        if(usr_name==s) return usr_passwd;
        //else               matchFlag=false;
    }
}


