#include "signalgraphics.h"
//#include <QGraphicsLineItem>

#include <QDebug>


SignalGraphics::SignalGraphics(int SetTimeDisplay, int freque, QWidget *parent) : QWidget(parent)
{
    //this->setMaximumSize()
    //this->setMinimumSize(750,484);
    //this->setStyleSheet("background-color: #0f0");
    com = new ComConnect();
    port = new ComPort();

    thread = new QThread();//Создаем поток для порта платы
    port->moveToThread(thread);//помешаем класс  в поток
    port->port->moveToThread(thread);//Помещаем сам порт в поток
    thread->start();

    connect(com,SIGNAL(PortConnectReques(QString)),port,SLOT(PortConnect(QString)));
    connect(com,SIGNAL(PortDisconncetReques()),port,SLOT(PortDisconnect()));

    TimeDisplayMs = SetTimeDisplay;
    ADCFreque = freque;
    CreateWidget();

    connect(graph_left_arrow,SIGNAL(clicked(bool)),this,SLOT(push_graph_left_arrow()));
    connect(graph_left_arrow,SIGNAL(clicked(bool)),this,SLOT(push_graph_right_arrow()));

    //voice_data = new VoiceData(20000,10);
    voice_data = new VoiceData(ADCFreque,10);

    thread = new QThread();
    voice_data->moveToThread(thread);
    thread->start();

    DataList = new QList<uint>;
    port->SetVoiceData(voice_data);

    thread = new QThread();
    graph_view->moveToThread(thread);
    //graph_scene->moveToThread(thread);
    thread->start();
//    voice_data->AddData(1800);
//    voice_data->AddData(2200);
//    voice_data->AddData(2350);
//    voice_data->AddData(1650);
//    voice_data->AddData(1900);
//    voice_data->AddData(1950);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(RedrawGraphic()));

    connect(com,SIGNAL(PortConnectReques(QString)),this,SLOT(StartDraw()));
    connect(com,SIGNAL(PortDisconncetReques()),this,SLOT(StopDraw()));

    //timer->start(1000);

    //timer->start(32);
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
    graph_scene->setItemIndexMethod(QGraphicsScene::NoIndex);
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


    //================================================
    //======Созданем список линий для графики=========
    //================================================
    ListLine = new  QList<QGraphicsLineItem*>;
    for (int i = 0; i<ADCFreque*TimeDisplayMs/1000/16; i++)
    {
        QPen pen_line(Qt::black,2);
        QGraphicsLineItem* line = new QGraphicsLineItem();
        line->setPen(pen_line);
        ListLine->append(line);
        graph_scene->addItem(line);
    }

    //ListPoint->append();

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

void SignalGraphics::CalcGraphValue(int x_start, int x_stop, int y_high, int y_low)
{
    coordinats.x_start = x_start;
    coordinats.x_stop = x_stop;
    coordinats.y_high = y_high;
    coordinats.y_low = y_low;
    coordinats.step_x = ((((double)TimeDisplayMs/(double)ADCFreque)*(coordinats.x_stop - coordinats.x_start))/TimeDisplayMs)*((double)ADCFreque/2000.0);
    coordinats.step_y = ((coordinats.y_low - coordinats.y_high)/1500.0);
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

    CalcGraphValue(ZeroHLineX, EndHLineX, ZeroVLineY, EndVLineY);

    //Перерисовка основных линий графика
    VLineGraph->setLine(QLine(ZeroVLineX, ZeroVLineY, EndVLineX, EndVLineY ));
    HLineGraph->setLine(QLine(ZeroHLineX, ZeroHLineY, EndHLineX, EndHLineY ));

    //Перериросвка линий стрелочек
    VLeftArrow->setLine(QLine(ZeroVLineX, ZeroVLineY, ZeroVLineX-5, ZeroVLineY+(len_height*0.05) ));
    VRightArrow->setLine(QLine(ZeroVLineX, ZeroVLineY, ZeroVLineX+5, ZeroVLineY+(len_height*0.05) ));
    HLeftArrow->setLine(QLine(EndHLineX, EndHLineY, EndHLineX - (len_width*0.03), EndHLineY + 5 ));
    HRightArrow->setLine(QLine(EndHLineX, EndHLineY, EndHLineX - (len_width*0.03), EndHLineY - 5));

}

//#include <QTime>
//QTime t;
//int count_data_redraw = 0;

void SignalGraphics::RedrawGraphic()
{

//        if(count_data_redraw == 0)
//        {
//            t.start();
//            t.restart();
//        }
    DataList->clear();
    while(voice_data->GetStatusBusy());
    voice_data->GetData(DataList, TimeDisplayMs);

    int count_list = DataList->count();
    //qDebug()<<count_data_redraw;
    int i = 0;


    for (int j = 0; j < count_list-1; j++)
    {
        if(j%16 != 0) continue;
        //coordinats.step_x;
        double x_1 = coordinats.x_start + i*coordinats.step_x;
        double x_2 = coordinats.x_start + (i+1)*coordinats.step_x;

        uint y_1_value = DataList->value(i);
        if (y_1_value<1500) y_1_value = 1500;
        if (y_1_value>3000) y_1_value = 3000;
        uint y_2_value = DataList->value(i+1);
        if (y_2_value<1500) y_2_value = 1500;
        if (y_2_value>3000) y_2_value = 3000;



        double y_1 = coordinats.y_low - ((y_1_value-1500)*coordinats.step_y);
        double y_2 = coordinats.y_low - ((y_2_value-1500)*coordinats.step_y);

        ListLine->value(i)->setLine(QLineF( x_1, y_1, x_2, y_2 ));
        i++;
        //ListLine->

        //line->setLine(QLineF( x_1, y_1, x_2, y_2 ));
        //graph_scene->addItem(line);

    }

//        if(count_data_redraw == 236)
//            {
//                qDebug("Time elapsed: %d ms", t.elapsed());
//                count_data_redraw = 0;
//                disconnect(timer, SIGNAL(timeout()), this, SLOT(RedrawGraphic()));
//            }

//        count_data_redraw++;
}


void SignalGraphics::StartDraw()
{
    timer->start(16);
}


void SignalGraphics::StopDraw()
{
    timer->stop();
}
