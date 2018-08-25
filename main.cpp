#include "tcpserver.h"
//#include "tcpclientsocket.h"
#include <QApplication>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::addLibraryPath("./");
    QApplication a(argc, argv);
    TcpServer w;
    w.show();


    return a.exec();
}
