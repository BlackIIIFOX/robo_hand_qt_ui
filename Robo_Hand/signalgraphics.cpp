#include "signalgraphics.h"
//#include <QGraphicsLineItem>

#include <QDebug>

SignalGraphics::SignalGraphics(QWidget *parent) : QWidget(parent)
{
    this->setMinimumSize(750,484);
    //this->setStyleSheet("background-color: #0f0");
    com = new ComConnect(this);
    port = new ComPort();

    thread = new QThread();//Создаем поток для порта платы
    port->moveToThread(thread);//помешаем класс  в поток
    port->port->moveToThread(thread);//Помещаем сам порт в поток
    thread->start();

    connect(com,SIGNAL(PortConnectReques(QString)),port,SLOT(PortConnect(QString)));
    connect(com,SIGNAL(PortDisconncetReques()),port,SLOT(PortDisconnect()));

    CreateWidget();

    connect(graph_left_arrow,SIGNAL(clicked(bool)),this,SLOT(push_graph_left_arrow()));
    connect(graph_left_arrow,SIGNAL(clicked(bool)),this,SLOT(push_graph_right_arrow()));
    //PrintBaseScene();
}

void SignalGraphics::CreateWidget()
{
    layout_widget = new QVBoxLayout();
    layout_widget->addWidget(com);
    layout_widget->setMargin(0);
    layout_widget->setSpacing(0);
    PrintBaseScene();
    layout_widget->addWidget(graph_widget);

    //down_spacer = new QSpacerItem(40,40,QSizePolicy::Expanding,QSizePolicy::Expanding);
    //layout_widget->addSpacerItem(down_spacer);


    this->setLayout(layout_widget);
}


void SignalGraphics::PrintBaseScene()
{
    graph_widget = new QWidget();
    graph_widget->setMinimumSize(690,424);
    //graph_widget->setStyleSheet("background-color: #0f0");

    //Создание нижней части виджета
    graph_layout_down = new QHBoxLayout();
    graph_layout_down->setMargin(0);
    graph_layout_down->setSpacing(20);

    graph_left_spacing = new QSpacerItem(20, 55 ,QSizePolicy::Expanding, QSizePolicy::Fixed);
    graph_right_spacing = new QSpacerItem(20, 55 ,QSizePolicy::Expanding, QSizePolicy::Fixed);

    graph_left_arrow = new QPushButton();
    graph_left_arrow->setMinimumSize(50,50);
    graph_left_arrow->setMaximumSize(50,50);
    graph_left_arrow->setStyleSheet("border-image: url(:/img/icons8-сортировать-справа-налево-64.png) stretch;");

    graph_right_arrow = new QPushButton();
    graph_right_arrow->setMinimumSize(50,50);
    graph_right_arrow->setMaximumSize(50,50);
    graph_right_arrow->setStyleSheet("border-image: url(:/img/icons8-сортировать-слева-направо-64.png) stretch;");

    graph_layout_down->addSpacerItem(graph_left_spacing);
    graph_layout_down->addWidget(graph_left_arrow);
    graph_layout_down->addWidget(graph_right_arrow);
    graph_layout_down->addSpacerItem(graph_right_spacing);



    //Создание сцены и ее представления

    graph_view = new QGraphicsView();
    graph_view->setMinimumSize(690,354);
    graph_scene = new QGraphicsScene(QRect(0,0,690,354));
    connect(this,SIGNAL(Resize()),this,SLOT(WidgetResized()));
    graph_view->setScene(graph_scene);
    graph_view->setRenderHint( QPainter::Antialiasing );
    graph_view->setStyleSheet("border-style: none");





    //Создание общего виджета
    graph_layout = new QVBoxLayout();
    //graph_layout->setMargin(25);
    graph_layout->setSpacing(5);
    graph_layout->setContentsMargins(25,15,25,0);
    graph_layout->addWidget(graph_view);
    graph_layout->addLayout(graph_layout_down);
    graph_widget->setLayout(graph_layout);

    PaintBaseItemInScene();
}

bool SignalGraphics::event(QEvent * e)
{
    //qDebug()<<e->type();
    switch(e->type())
    {
    case QEvent::Resize:
        emit Resize();
        return true;
        break;
    case QEvent::Show:
        WidgetResized();
        //qDebug()<<"sadasd";
        return true;
        break;
    default:
        break;
    }
    return QWidget::event(e);
}



void SignalGraphics::PaintBaseItemInScene()
{
    //================================================
    //==========Создание элементов сцены==============
    //================================================

    QPen pen_line(Qt::black,2);
    //Основные линии
    VLineGraph = new QGraphicsLineItem();
    VLineGraph->setPen(pen_line);

    HLineGraph = new QGraphicsLineItem();
    HLineGraph->setPen(pen_line);

    //Линии стрелочек
    VLeftArrow = new QGraphicsLineItem();
    VLeftArrow->setPen(pen_line);

    VRightArrow = new QGraphicsLineItem();
    VRightArrow->setPen(pen_line);

    HLeftArrow = new QGraphicsLineItem();
    HLeftArrow->setPen(pen_line);

    HRightArrow = new QGraphicsLineItem();
    HRightArrow->setPen(pen_line);


    //================================================
    //==========Добавление элементов на сцену=========
    //================================================

    //Добавляем основные линии графика
    graph_scene->addItem(VLineGraph);
    graph_scene->addItem(HLineGraph);

    //Добавляем линии стрелочек
    graph_scene->addItem(VLeftArrow);
    graph_scene->addItem(VRightArrow);
    graph_scene->addItem(HLeftArrow);
    graph_scene->addItem(HRightArrow);





}


void SignalGraphics::paintEvent(QPaintEvent *pe) {
    QStyleOption o;
    o.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}

void SignalGraphics::push_graph_left_arrow()
{

}

void SignalGraphics::push_graph_right_arrow()
{

}


void SignalGraphics::WidgetResized()
{
    int len_height =  graph_view->height();
    int len_width =  graph_view->width();

    //Установка размера самой сцены
    graph_scene->setSceneRect( 0, 0, len_width, len_height );


    //Перерисовака линий и прочего
    int ZeroVLineX = 20, ZeroVLineY = 10,
            EndVLineX = 20, EndVLineY = len_height-20,
            ZeroHLineX = 20, ZeroHLineY = len_height-20,
            EndHLineX = len_width-20, EndHLineY = len_height-20;

    //Перерисовка основных линий графика
    VLineGraph->setLine(QLine(ZeroVLineX, ZeroVLineY, EndVLineX, EndVLineY ));
    HLineGraph->setLine(QLine(ZeroHLineX, ZeroHLineY, EndHLineX, EndHLineY ));

    //Перериросвка линий стрелочек
    VLeftArrow->setLine(QLine(ZeroVLineX, ZeroVLineY, ZeroVLineX-5, ZeroVLineY+(len_height*0.05) ));
    VRightArrow->setLine(QLine(ZeroVLineX, ZeroVLineY, ZeroVLineX+5, ZeroVLineY+(len_height*0.05) ));
    HLeftArrow->setLine(QLine(EndHLineX, EndHLineY, EndHLineX - (len_width*0.03), EndHLineY + 5 ));
    HRightArrow->setLine(QLine(EndHLineX, EndHLineY, EndHLineX - (len_width*0.03), EndHLineY - 5));

}

