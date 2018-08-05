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

struct GraphCoord
{
    int x_start,x_stop; //Начальная и конечная точка по координате x
    int y_high,y_low;   //Верхняя и нижняя точка по координате y
    double step_x, step_y; //Шаг для точек по координатам x и y
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
    void push_graph_left_arrow();
    void push_graph_right_arrow();
    void WidgetResized();
    void RedrawGraphic();


private:
    void PrintBaseScene();
    void PaintBaseItemInScene();
    void CreateWidget();
    void CalcGraphValue(int x_start, int x_stop, int y_high, int y_low);
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
