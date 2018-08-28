#include "tcpclientsocket.h"

TcpClientSocket::TcpClientSocket(QObject *parent)
{
    connect(this,SIGNAL(readyRead()),this,SLOT(dataReceived()));
    connect(this,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
    //server = new Server(this,8010);


    //tcpServer = new TcpServer(this);
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

/*
 * 函数名：dataReceived
 * 功能：处理从客户端收到的各种数据
 * 参数：NULL
 * 返回值：void
 */
void TcpClientSocket::dataReceived()
{
    QByteArray buffer;
    buffer = this->readAll();
    QString str = QString::fromLocal8Bit(buffer);
    qDebug()<<str;

    //用于传输文件时的信息验证
    if(str.contains("+",Qt::CaseInsensitive))
    {
        QStringList strlist = str.split("+");
        QString str1 = strlist[0];
        QString str2 = strlist[1];
        qDebug()<<str2;

        if(str1 == "file")
        {
            qDebug()<<"file";
            QString str3 = str2+str2;
            //emit updateClients(str1,str1.length());
            emit updateClients(str3,str2.length());
        }
        else
        {
            QString str3 = str1+str2;
            //emit showuser(str1);
            //emit updateClients(str1,str2.length());
            emit updateClients(str3,str3.length());
        }
    }

    //此部分用于登陆注册信息的验证
    else if(str.contains("/",Qt::CaseInsensitive))
    {
        QStringList strlist = str.split("/");
        QString str_1 = strlist[0];
        QString str_2 = strlist[1];
        //注册信息写入数据库
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
                while(sql_query.next())
                {
                    QString name = sql_query.value(0).toString();
                    QString pwd = sql_query.value(1).toString();
                    qDebug()<<QString("name: %1     pwd:%2").arg(name).arg(pwd);
                }
            }
        }
        //验证登陆信息
        else
        {
            qDebug()<<"username";
            bool ok = checkusername(str_1);           //验证用户名是否正确
            if(ok)
            {
                QString pwdinfo = checkusrInfo(str_1);  //从数据库中查询获取该用户名对应的密码
                qDebug()<<pwdinfo;
                qDebug()<<str_1;
                this->flush();
                this->write(pwdinfo.toLocal8Bit());
                this->flush();
            }
            else                  //用户名不正确，返回一串字符做标识
            {
                QString f =  "***";
                this->flush();
                this->write(f.toLocal8Bit());
                this->flush();
            }
        }
        emit updateClients(str,str.length());
    }
    //此部分用于更新用户密码
    else if(str.contains("#",Qt::CaseInsensitive))
    {
        QStringList strlist = str.split("#");
        QString str1 = strlist[0];
        QString str2 = strlist[1];
        QSqlQuery sqlquery(database);
        QString update = "update data set passwd = :passwd where name = :name";
        sqlquery.prepare(update);
        sqlquery.bindValue(":passwd",str2);
        sqlquery.bindValue(":name",str1);
        if(!sqlquery.exec())
        {
            qDebug()<<sqlquery.lastError();
        }
        else {
            qDebug()<<"update success";
        }
    }
    //此部分用于更新用户名
    else if(str.contains("@",Qt::CaseInsensitive))
    {
        QStringList strlist = str.split("@");
        QString str1 = strlist[0];
        QString str2 = strlist[1];

        QSqlQuery sqlquery(database);
        QString select = "select * from data";
        QString update = "update data set name = :name where passwd = :passwd";
        if(!sqlquery.exec(select))
        {
            qDebug()<<sqlquery.lastError();
        }
        else
        {
            while (sqlquery.next()) {
                QString name = sqlquery.value(0).toString();
                QString pwd = sqlquery.value(1).toString();
                if(name == str1)
                {
                    QString passwd = pwd;
                    sqlquery.prepare(update);
                    sqlquery.bindValue(":name",str2);
                    sqlquery.bindValue(":passwd",passwd);
                    if(!sqlquery.exec())
                    {
                        qDebug()<<sqlquery.lastError();
                    }
                    else
                    {
                        qDebug()<<"server update success";
                    }
                }
            }



        }
    }    
    else {
        emit updateClients(str,str.length());
    }

}

void TcpClientSocket::slotDisconnected()
{
    emit disconnected(this->socketDescriptor());
}

/*
 * 函数名：checkusrInfo
 * 功能：验证用户信息
 * 参数：QString
 * 返回值：QString
 */
QString TcpClientSocket::checkusrInfo(QString s)
{

    qDebug()<<"here";
    QSqlQuery sql_query(database);
    //数据库语句，执行查询操作
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

/*
 * 函数名：checkusername
 * 功能：验证用户名是否正确
 * 参数：QString
 * 返回值：bool
 */
bool TcpClientSocket::checkusername(QString s)
{
    QSqlQuery sql_query(database);
    //数据库语句，执行查询操作
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

