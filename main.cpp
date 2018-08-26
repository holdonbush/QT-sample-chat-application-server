#include "tcpserver.h"
//#include "tcpclientsocket.h"
#include <QApplication>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::addLibraryPath("./");
    QApplication a(argc, argv);
    TcpServer w;
    TcpClientSocket t;
    QObject::connect(&t,SIGNAL(showuser(QString)),&w,SLOT(receive(QString)));
    w.show();


    return a.exec();
}
