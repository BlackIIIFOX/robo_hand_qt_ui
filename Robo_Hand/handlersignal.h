#ifndef HANDLERSIGNAL_H
#define HANDLERSIGNAL_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QEvent>
#include <QString>
#include <QLabel>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QPushButton>
#include <QPainter>
#include <QStyleOption>
#include <QThread>
#include <QSerialPort>
#include <QThread>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QTimer>
#include <QPolygon>
#include <QPainterPath>
#include <QTextDocument>
#include <QFile>
#include <QFileDialog>
#include "recognizer.h"


#define LimitValueSpect 10000
#define CountMelBank 30
#define sampleRate 8000

struct GraphCoord
{
    int x_start,x_stop; //Начальная и конечная точка по координате x
    int y_high,y_low;   //Верхняя и нижняя точка по координате y
    double step_x, step_y; //Шаг для точек по координатам x и y
};


struct StuctWavFail
{
    uint32_t chunkId = 0x52494646;        //Содержит символы “RIFF” в ASCII кодировке
    uint32_t chunkSize;                   //Это оставшийся размер цепочки, начиная с этой позиции. Иначе говоря, это размер файла – 8, то есть, исключены поля chunkId и chunkSize.
    uint32_t format = 0x57415645 ;        //Содержит символы “WAVE”
    uint32_t subchunk1Id = 0x666d7420;    //Содержит символы “fmt“
    uint32_t subchunk1Size;               //16 для формата PCM. Это оставшийся размер подцепочки, начиная с этой позиции.
    uint16_t audioFormat = 1;             //Аудио формат. Для PCM = 1 (то есть, Линейное квантование). Значения, отличающиеся от 1, обозначают некоторый формат сжатия.
    uint16_t numChannels = 1;             //Количество каналов. Моно = 1, Стерео = 2 и т.д.
    uint32_t sampleRateWav;               //Частота дискретизации. 8000 Гц, 44100 Гц и т.д.
    uint32_t byteRate;                    //Количество байт, переданных за секунду воспроизведения.
    uint16_t blockAlign;                  //Количество байт для одного сэмпла, включая все каналы.
    uint16_t bitsPerSample;               //Количество бит в сэмпле. Так называемая “глубина” или точность звучания. 8 бит, 16 бит и т.д.
    uint32_t subchunk2Id = 0x64617461;    //Содержит символы “data”
    uint32_t subchunk2Size;               //Количество байт в области данных.
    uint16_t* data;                       //Непосредственно WAV-данные.
};


struct GraphArrowCoord
{
    int ZeroVLineX = 20, ZeroVLineY = 10,
    EndVLineX = 20, EndVLineY,
    ZeroHLineX = 20, ZeroHLineY,
    EndHLineX, EndHLineY;
    int len_height, len_width;
};


class HandlerSignal : public QWidget
{
    Q_OBJECT
public:
    HandlerSignal(QWidget *parent = nullptr);
    HandlerSignal(QList<int> list,QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *pe);
    bool event(QEvent * e);

signals:
    void Resize();

public slots:
    void WidgetResized();
    void PushSaveButton();
    void PushLoadButton();
    void PushCalcButton();

private:
    int min_y_signal_graph, max_y_signal_graph;
    int min_y_mfcc_graph, max_y_mfcc_graph;
    double min_y_spect_graph, max_y_spect_graph;
    void CalcGraphValue(int len_height, int len_width, GraphCoord* coordinats, GraphArrowCoord* coordinats_arrow);
    void CreateConnectors();
    void RedrawGraphicsSignal();
    void RedrawGraphicsSpec();
    void RedrawGraphicsMFCC();
    void RedrawButtons();
    void CalcAndRedrawSignal();
    void CalcAndRedrawSpectr();
    void CalcAndRedrawMFCC();
    void CreateWavData(QByteArray* data);
    //Для вывода графики
    void CreateWidgets();
    void FourierTransform();
    //void SetRecognize();
    StuctWavFail WavFileHeader;
    QGraphicsView* graph_view_signal, *graph_view_spect, *graph_view_mfcc;
    QGraphicsScene* graph_scene_signal, *graph_scene_spect, *graph_scene_mfcc;
    GraphCoord coordinats_view_signal, coordinats_view_spect, coordinats_view_mfcc;
    QPushButton* SaveWavButton;
    QPushButton* LoadWavButton;
    QPushButton* CalcWavButton;

    QList<QList<double>> List_MelFilter;
    QList<double> List_MelCoof;
    QList<double> List_CepstalMelCoof;
    QList<double> List_MelF, List_FreqM;
    QList<int> List_SpecMelCoord;
    QList<int> data_signal;
    QList<double> data_spectr;
    GraphArrowCoord coordinats_arrow_signal, coordinats_arrow_spect, coordinats_arrow_mfcc;
    float* Rdat, *Idat;
    int LenghtDataSpect;
    Recognizer rec;

};

#endif // HANDLERSIGNAL_H
