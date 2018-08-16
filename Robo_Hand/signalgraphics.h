#ifndef SIGNALGRAPHICS_H
#define SIGNALGRAPHICS_H

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
#include "comconnectwidget.h"
#include <QThread>
#include <QSerialPort>
#include "comport.h"
#include <QThread>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QTimer>
#include "voicedata.h"
#include <QPolygon>
#include <QPainterPath>
#include <QTextDocument>

struct GraphCoord
{
    int x_start,x_stop; //Начальная и конечная точка по координате x
    int y_high,y_low;   //Верхняя и нижняя точка по координате y
    double step_x, step_y; //Шаг для точек по координатам x и y
};

struct GraphArrowCoord
{
    int ZeroVLineX = 20, ZeroVLineY = 10,
            EndVLineX = 20, EndVLineY,
            ZeroHLineX = 20, ZeroHLineY,
            EndHLineX, EndHLineY;
    int len_height, len_width;
};



class SignalGraphics : public QWidget
{
    Q_OBJECT
public:
    explicit SignalGraphics(int SetTimeDisplay = 1000,int freque = 20000,QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *pe);
    bool event(QEvent * e);

signals:
    void Resize();

public slots:
    void push_graph_start();
    void push_graph_search();
    void WidgetResized();
    void RedrawGraphic();
    void StartDraw();
    void StopDraw();


private:
    void PrintBaseScene();
    void PaintBaseItemInScene();
    void CreateWidget();
    void CalcGraphValue(int x_start, int x_stop, int y_high, int y_low);
    void PrintArrowGraph();
    ComConnect* com;
    QVBoxLayout* layout_widget;
    QSpacerItem* down_spacer;
    QSerialPort* serial_port;
    ComPort* port;  //Виджет ком порта (верхняя часть)
    QThread* thread;
    QTimer* timer;  //Отрисовка за счет таймера
    VoiceData* voice_data;
    QList<uint>* DataList;  //Список для хранения выводимых значений
    int TimeDisplayMs;      //Длительность сигнала выводимое на экран в мс
    int ADCFreque;          //Частота АЦП
    GraphCoord coordinats;
    GraphArrowCoord coordinats_arrow;
    QGraphicsPathItem *item_path;
    bool StatePaintGraph;
    int start_signal_voice, stop_signal_voice;




    //Для вывода графики
    QGraphicsView* graph_view;
    QGraphicsScene* graph_scene;
    QWidget* graph_widget;
    QVBoxLayout* graph_layout;
    QHBoxLayout* graph_layout_down;
    QPushButton* graph_left_arrow;
    QPushButton* graph_right_arrow;
    QSpacerItem* graph_left_spacing;
    QSpacerItem* graph_right_spacing;

    //Элементы графики
    QGraphicsLineItem* VLineGraph;
    QGraphicsLineItem* VLeftArrow;
    QGraphicsLineItem* VRightArrow;
    QGraphicsLineItem* HLineGraph;
    QGraphicsLineItem* HLeftArrow;
    QGraphicsLineItem* HRightArrow;
    QList<QGraphicsLineItem*>* ListLine;



};

#endif // SIGNALGRAPHICS_H
