#ifndef COMPORT_H
#define COMPORT_H

#include <QObject>
#include <QSerialPort>

class ComPort : public QObject
{
    Q_OBJECT
public:
    explicit ComPort(QObject *parent = nullptr);
    QSerialPort* port;
    ~ComPort();

signals:

public slots:
    void PortConnect(QString name_port);
    void PortDisconnect();
    void serialReceived();

};

#endif // COMPORT_H
