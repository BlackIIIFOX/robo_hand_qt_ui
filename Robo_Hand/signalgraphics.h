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

class SignalGraphics : public QWidget
{
    Q_OBJECT
public:
    explicit SignalGraphics(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *pe);
    bool event(QEvent * e);

signals:
    void Resize();

public slots:
    void push_graph_left_arrow();
    void push_graph_right_arrow();
    void WidgetResized();


private:
    void PrintBaseScene();
    void PaintBaseItemInScene();
    void CreateWidget();
    ComConnect* com;
    QVBoxLayout* layout_widget;
    QSpacerItem* down_spacer;
    QSerialPort* serial_port;
    ComPort* port;
    QThread* thread;

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


};

#endif // SIGNALGRAPHICS_H
