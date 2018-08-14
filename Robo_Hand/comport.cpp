#include "comport.h"
#include <QDebug>

ComPort::ComPort(VoiceData* object_voice_data, QObject *parent ) : QObject(parent)
{
    port = new QSerialPort();
    voice_data = object_voice_data;
}


void ComPort::SetVoiceData(VoiceData* object_voice_data)
{
    voice_data = object_voice_data;
}


void ComPort::PortConnect(QString port_name)
{
    //qDebug()<<port_name;
    port->setPortName(port_name);
    port->setBaudRate(921600);
    //serial->setBaudRate(QSerialPort::Baud115200);
    //serial->setBaudRate(9600);
    port->setDataBits(QSerialPort::Data8);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);
    port->setFlowControl(QSerialPort::NoFlowControl);
    connect(port,SIGNAL(readyRead()),this,SLOT(serialReceived()));
    port->open(QIODevice::ReadWrite);
    //port->open(QIODevice::ReadOnly);

    port->write("R",1);
}
int count_data = 0;
void ComPort::PortDisconnect()
{
    //qDebug()<<count_data;
    //qDebug()<<"disk";
    port->write("E",1);
    port->waitForBytesWritten(100);
    while(port->isWritable() == false);
    //qDebug()<<port->isWritable();
    if(port->isOpen()){
        port->close();
    }
}

QByteArray val_adc;
uint16_t data_des;
int i;

//#include <QTime>
//QTime t;


void ComPort::serialReceived()
{
    //qDebug()<<"Пришло";
    //qDebug()<<port->readAll();
    //return true;
    i++;

//    if(count_data == 0)
//    {
//        t.start();
//        t.restart();
//    }
    //qDebug()<<i;
    val_adc = port->readAll();

    for(int i=0;i<val_adc.count();i++)
    {
        uchar data = val_adc[i];
        if((i+1)%2==0)
        {
            data_des = data_des|data;
            while(voice_data->GetStatusBusy());
            voice_data->AddData(data_des);
            //qDebug()<<data_des;
        }
        else
        {
            data_des = data<<8;
        }
        count_data++;
        //data_des
        //uchar data = val_adc[i];
        //qDebug()<<data;
    }



//    if(count_data == 40000)
//    {
//        qDebug("Time elapsed: %d ms", t.elapsed());
//        count_data = 0;
//    }


}

ComPort::~ComPort()
{
    if(port->isOpen()){
        port->close();
    }
}
