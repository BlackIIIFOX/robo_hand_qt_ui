#include "voicedata.h"
#include <QDebug>

VoiceData::VoiceData(uint freq, uint time,QObject *parent) : QObject(parent)
{
    DataList = new QList<uint>;
    FrequeADC = freq;
    RecTime = time;
    MaxCountData = FrequeADC*RecTime;
    StatusBusy = false;
}

void VoiceData::AddData(uint ADC_Value)
{
    StatusBusy = true;
    DataList->append(ADC_Value);
    if( DataList->length() > (int)MaxCountData )
        DataList->pop_front();
    StatusBusy = false;
}


void VoiceData::GetData(QList<uint>* InputList, uint start_time_ms, uint stop_time_ms)
{
    /*
     * Функция расчитывает начальный и конечный индекс списка на основании частоты. На входе время в милисекундах.
     * На основании индексов происходит добавление значений в выходной список.
    */
    StatusBusy = true;

    //Предварительные проверки для избежания ошибок
    uint RealRecTime;
    RealRecTime = (DataList->count()/(float)FrequeADC)*1000;
    if(RealRecTime<stop_time_ms)
        stop_time_ms = RealRecTime;


    //Основные расчеты индексов
    double koef_freq_start;
    koef_freq_start = start_time_ms/1000.0;
    int index_start,index_stop;
    index_start =  FrequeADC * koef_freq_start;
    index_stop = (((stop_time_ms - start_time_ms)*FrequeADC)/1000) + index_start;

    //Заполнение входного списка за счет расчитанных индексов
    for (int i = index_start; i < index_stop; i++)
    {
        InputList->append(DataList->value(i));
    }

    StatusBusy = false;
}


void VoiceData::GetData(QList<uint>* InputList, uint start_time_ms)
{
    StatusBusy = true;

    int LengthDataList = DataList->count();

    //Предварительные проверки для избежания ошибок
    uint RealRecTime;
    RealRecTime = (LengthDataList/(float)FrequeADC)*1000;
    if(RealRecTime<start_time_ms)
        start_time_ms = RealRecTime;

    //Основные расчеты индексов
    int index_start;
    index_start = LengthDataList - ((start_time_ms*FrequeADC)/1000);
    //qDebug()<<index_start << " "<<LengthDataList;
    //if(index_start<0)
    //    index_start = 0;

    //Заполнение входного списка за счет расчитанных индексов
    for (int i = index_start; i < LengthDataList; i++)
    {
        InputList->append(DataList->value(i));
    }

    StatusBusy = false;
}

bool VoiceData::GetStatusBusy()
{
    return StatusBusy;
}
