#include "comconnectwidget.h"
#include <QDebug>

ComConnect::ComConnect(QWidget *parent) : QWidget(parent)
{
    this->setMinimumSize(750,60);
    this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    //this->setStyleSheet("background-color: #deffeb");
    CreateWidgets();
    current_port = new QSerialPortInfo();
    connect_but->setEnabled(false);

    connect(available_ports,SIGNAL(currentIndexChanged(QString)),this,SLOT(change_port(QString)));
    connect(connect_but,SIGNAL(clicked(bool)),this,SLOT(push_connect_but()));

    foreach (QSerialPortInfo port_info, current_port->availablePorts()) {
        available_ports->addItem(port_info.portName());
    }
    count_ports = available_ports->count();

    if (count_ports == 0)
    {
        state_port->setStyleSheet("border-image: url(:/img/icons8_portnotan.png) stretch;");
        connect_but->setEnabled(false);
        connection_text->setText("Нет доступных портов");
        PrevBusy = true;
    }
    StateConnect = false;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(TimerUpdate()));
    timer->start(100);

    //available_ports->setEnabled(false);
    //===Для тестов====
    //but = new QPushButton(this);
    //but->setGeometry(50,50,100,50);

}

void ComConnect::CreateWidgets()
{
    layout_widget = new QHBoxLayout();
    layout_widget->setMargin(0);
    layout_widget->setSpacing(0);
    layout_widget_connection = new QHBoxLayout();
    layout_widget_connection->setMargin(0);
    layout_widget_connection->setSpacing(5);


    left_spacer = new QSpacerItem(40,20,QSizePolicy::Fixed,QSizePolicy::Expanding);


    connection_text = new QLabel();
    connection_text->setText("Устройство не подключено");
    QFont font("Century Schoolbook",14);
    connection_text->setFont(font);


    state_port = new QLabel();
    state_port->setMinimumSize(25,25);
    state_port->setMaximumSize(25,25);
    state_port->setStyleSheet("border-image: url(:/img/icons8_portnotan.png) stretch;");

    connect_but = new QPushButton();
    connect_but->setMinimumSize(30,30);
    connect_but->setMaximumSize(30,30);
    connect_but->setStyleSheet("border-image: url(:/img/icons8_disconnect.png) stretch;");

    available_ports = new QComboBox();
    available_ports->setMinimumSize(100,25);
    available_ports->setMaximumSize(130,25);

    midle_spacer = new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Expanding);
    right_spacer = new QSpacerItem(20,20,QSizePolicy::Fixed,QSizePolicy::Expanding);



    down_layout = new QHBoxLayout();
    down_layout->setMargin(0);
    down_layout->setSpacing(0);
    line_down = new QWidget();
    line_down->setMinimumSize(700,2);
    line_down->setMaximumHeight(2);
    line_down->setStyleSheet("background: #000;");
    left_spacer_line = new QSpacerItem(25,1,QSizePolicy::Fixed,QSizePolicy::Fixed);
    right_spacer_line = new QSpacerItem(25,1,QSizePolicy::Fixed,QSizePolicy::Fixed);
    down_layout->addSpacerItem(left_spacer_line);
    down_layout->addWidget(line_down);
    down_layout->addSpacerItem(right_spacer_line);

    layout_widget_connection->addWidget(state_port);
    layout_widget_connection->addWidget(available_ports);
    layout_widget_connection->addWidget(connect_but);


    layout_widget->addSpacerItem(left_spacer);
    layout_widget->addWidget(connection_text);
    layout_widget->addSpacerItem(midle_spacer);
    layout_widget->addLayout(layout_widget_connection);
    layout_widget->addSpacerItem(right_spacer);

    layout_widget_all = new QVBoxLayout();
    layout_widget_all->addLayout(layout_widget);
    layout_widget_all->addLayout(down_layout);
    layout_widget_all->setMargin(0);
    layout_widget_all->setSpacing(0);




    this->setLayout(layout_widget_all);
}


void ComConnect::paintEvent(QPaintEvent *pe) {
    QStyleOption o;
    o.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}


void ComConnect::ReScanPorts()
{

    timer->stop();

    int current_count_ports = current_port->availablePorts().count();
    bool state_port_enable = current_port->isValid();
    bool state_port_busy = current_port->isBusy();

    if (current_count_ports != count_ports)
    {
        if (current_count_ports == 0)
        {
            disconnect(available_ports,SIGNAL(currentIndexChanged(QString)),this,SLOT(change_port(QString)));
            state_port->setStyleSheet("border-image: url(:/img/icons8_portnotan.png) stretch;");
            connect_but->setEnabled(false);
            connection_text->setText("Нет доступных портов");
            available_ports->clear();
            PrevBusy = true;
            count_ports = 0;
            delete(current_port);
            current_port = new QSerialPortInfo();
            connect(available_ports,SIGNAL(currentIndexChanged(QString)),this,SLOT(change_port(QString)));
        }
        else
        {
            //Жутко лагучий кусок кода
            //qDebug("Тут");
            if (state_port_enable == false)
            {
                available_ports->removeItem(available_ports->currentIndex());
                //change_port(available_ports->currentText());
            }

            for ( int i =0; i<available_ports->count(); i++)
            {
                if (available_ports->itemText(i) != current_port->portName())
                    available_ports->removeItem(i);
            }

            foreach (QSerialPortInfo port_info, current_port->availablePorts()) {
                //qDebug()<<current_port->portName();
                if (port_info.portName() != current_port->portName())
                    available_ports->addItem(port_info.portName());
            }
            count_ports = current_count_ports;


            if (state_port_busy != PrevBusy)
            {
                if (state_port_busy == false)
                {
                    state_port->setStyleSheet("border-image: url(:/img/icons8_portav.png) stretch;");
                    connect_but->setEnabled(true);
                    connection_text->setText("Устройство не подключено");
                    PrevBusy = false;
                }
                else
                {
                    state_port->setStyleSheet("border-image: url(:/img/icons8_portnotan.png) stretch;");
                    connect_but->setEnabled(false);
                    connection_text->setText("Порт занят, подключение невозможно");
                    PrevBusy = true;
                }
                PrevBusy = state_port_busy;
            }

        }
    }

    timer->start(100);
}



//===============================================================================
//===============================================================================
//====================================Слоты======================================
//===============================================================================
//===============================================================================
void ComConnect::push_connect_but()
{
    StateConnect = !StateConnect;
    if (StateConnect == true)
    {
        timer->stop();
        available_ports->setEnabled(false);
        emit PortConnectReques(current_port->portName());
        connect_but->setStyleSheet("border-image: url(:/img/icons8_connect.png) stretch;");
        state_port->setStyleSheet("border-image: url(:/img/icons8_portnotan.png) stretch;");
        connection_text->setText("Устройство подключено: "+current_port->portName());
    }
    else
    {
        timer->start();
        available_ports->setEnabled(true);
        emit PortDisconncetReques();
        connect_but->setStyleSheet("border-image: url(:/img/icons8_disconnect.png) stretch;");
        state_port->setStyleSheet("border-image: url(:/img/icons8_portav.png) stretch;");
        connection_text->setText("Устройство отключено.");
    }
    //qDebug()<<current_port.isBusy();
}


void ComConnect::TimerUpdate()
{
    ReScanPorts();
}

void ComConnect::change_port(QString port)
{
    foreach (QSerialPortInfo port_info, current_port->availablePorts()) {
        if (port_info.portName() == port)
        {
            *current_port = port_info;
            PrevValide = true;
        }
    }
    if (current_port->isBusy() == false)
    {
        state_port->setStyleSheet("border-image: url(:/img/icons8_portav.png) stretch;");
        connect_but->setEnabled(true);
        connection_text->setText("Устройство не подключено");
        PrevBusy = false;
    }
    else
    {
        state_port->setStyleSheet("border-image: url(:/img/icons8_portnotan.png) stretch;");
        connect_but->setEnabled(false);
        connection_text->setText("Порт занят, подключение невозможно");
        PrevBusy = true;
    }
}
//===============================================================================
//===============================================================================



