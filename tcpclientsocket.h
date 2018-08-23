#include <QTcpSocket>
#include <QObject>

class TcpClientSocket : public QTcpSocket
{
    Q_OBJECT
public:
    TcpClientSocket(QObject *parent = 0);

signals:
    void updateClients(QString, int);
    void disconnected(int);
protected slots:
    void dataReceived();
    void slotDisconnected();
};
