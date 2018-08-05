#ifndef COMPORT_H
#define COMPORT_H

#include <QObject>
#include <QSerialPort>
#include "voicedata.h"

class ComPort : public QObject
{
    Q_OBJECT
public:
    explicit ComPort(VoiceData* object_voice_data = nullptr, QObject *parent = nullptr);
    QSerialPort* port;
    void SetVoiceData(VoiceData* object_voice_data);
    ~ComPort();

signals:

public slots:
    void PortConnect(QString name_port);
    void PortDisconnect();
    void serialReceived();

private:
    VoiceData* voice_data;
};

#endif // COMPORT_H
