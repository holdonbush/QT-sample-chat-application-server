#include "tcpclientsocket.h"

TcpClientSocket::TcpClientSocket(QObject *parent)
{
    connect(this,SIGNAL(readyRead()),this,SLOT(dataReceived()));
    connect(this,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));

    //tcpSocket = new QTcpSocket(this);
    tableFlag = false;

    //创建SQLITE数据库
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("data.db");

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
                if(tableName.compare("data"))
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
            //创建名为data的数据库表
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
            if(!sql_query.exec())
            {
                qDebug()<<sql_query.lastError();
            }
            else
            {
                qDebug()<<"success";
            }

            QString str = "select * from data";
            sql_query.prepare(str);
            if(!sql_query.exec())
            {
                qDebug()<<sql_query.lastError();
            }
            else
            {
//                if(sql_query.next())
//                {
//                    qDebug()<<"t";
//                }
//                else {
//                    qDebug()<<"f";
//                }
                while(sql_query.next())
                {
                    QString name = sql_query.value(0).toString();
                    QString pwd = sql_query.value(1).toString();
                    qDebug()<<QString("name: %1     pwd:%2").arg(name).arg(pwd);
                }
            }
        }
        else
        {
            qDebug()<<"username";
            bool ok = checkusername(str_1);
            if(ok)
            {
                QString pwdinfo = checkusrInfo(str_1);
                qDebug()<<pwdinfo;
                qDebug()<<str_1;
                this->flush();
                this->write(pwdinfo.toLocal8Bit());
                this->flush();
            }
            else
            {
                QString f =  "***";
                this->flush();
                this->write(f.toLocal8Bit());
                this->flush();
            }
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
    QSqlQuery sql_query(database);
//    QString select = "select * from data";
//    if(!sql_query.exec(select))
//    {
//        qDebug()<<sql_query.lastError();
//    }
//    else
//    {
//        while (sql_query.next()) {
//            QString str = sql_query.value(0).toString();
//            if(str == s)
//            {
//                break;
//            }
//        }
//    }
    QString tempstring = "select * from data where name='"+s+"'";
    qDebug()<<sql_query.lastError();
    qDebug()<<"err0";
    if(!sql_query.exec(tempstring))
    {
        qDebug()<<"error";
        qDebug()<<sql_query.lastError();
        //matchFlag = false;
        return NULL;
    }
    else
    {
        qDebug()<<"err1";
        while(sql_query.next())
        {
            qDebug()<<"continue";
            //usr_id = sql_query.value(0).toInt();
            usr_passwd = sql_query.value(1).toString();
            usr_name = sql_query.value(0).toString();

            qDebug()<<QString("passwd=%1   name=%2").arg(usr_passwd).arg(usr_name);

        }
        if(usr_name==s) return usr_passwd;
        //else               matchFlag=false;


    }
}

bool TcpClientSocket::checkusername(QString s)
{
    QSqlQuery sql_query(database);
    QString select = "select * from data";
       if(!sql_query.exec(select))
       {
           qDebug()<<sql_query.lastError();
       }
       else
       {
           while (sql_query.next()) {
               QString str = sql_query.value(0).toString();
               qDebug()<<str;
               if(str == s)
               {
                   return true;
               }
           }
       }
       return false;
}

