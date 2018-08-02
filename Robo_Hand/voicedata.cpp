#include "voicedata.h"
#include <QDebug>

VoiceData::VoiceData(uint freq, uint time,QObject *parent) : QObject(parent)
{
    DataList = new QList<uint>;
    FrequeADC = freq;
    RecTime = time;
    MaxCountData = FrequeADC*RecTime;
}

void VoiceData::AddData(uint ADC_Value)
{
    StatusBusy = true;
    DataList->append(ADC_Value);
    if( DataList->length() > MaxCountData )
        DataList->pop_front();
    StatusBusy = false;
}

void VoiceData::GetData()
{

}

bool VoiceData::GetStatusBusy()
{
    return StatusBusy;
}
