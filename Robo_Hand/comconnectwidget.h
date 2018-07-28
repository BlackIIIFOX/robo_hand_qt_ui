#ifndef COMCONNECT_H
#define COMCONNECT_H

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
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QComboBox>
#include <QTimer>
#include <QVBoxLayout>


class ComConnect : public QWidget
{
    Q_OBJECT
public:
    explicit ComConnect(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *pe);

signals:
    void PortConnectReques(QString value);
    void PortDisconncetReques();

public slots:
    void push_connect_but();         //Установка соединения
    void change_port(QString port);  //Был выбран другой порт
    void TimerUpdate();              //Узнать текущее состояние портов по таймеру

private:
    void CreateWidgets(); //Создание основных виджетов
    void ReScanPorts();
    int count_ports;
    bool PrevBusy;
    bool PrevValide;

    //QPushButton* but;   //Для отладки
    //QSerialPortInfo ports;   //Информация о состоянии портов
    QSerialPortInfo* current_port;
    QTimer* timer;

    QVBoxLayout* layout_widget_all;    //Общий layout
    QHBoxLayout *layout_widget;  //Основной layout для виджета соединения
    QHBoxLayout *layout_widget_connection; //layout для виджета соединения
    QSpacerItem *left_spacer;   //Левый пробел
    QSpacerItem *midle_spacer;  //Центральный пробел
    QSpacerItem *right_spacer;  //Правый пробел
    QLabel *connection_text;    //Строка для вывода подключения
    QLabel *state_port;         //Label для картики состояния порта
    QComboBox *available_ports; //Для вывода всех доступных портов
    QPushButton *connect_but;   //Кнопка для подключения к порту
    QHBoxLayout* down_layout;   //Нижний layout для линии
    QWidget* line_down;         //Нижняя линия
    QSpacerItem *left_spacer_line;   //Левый пробел для линии
    QSpacerItem *right_spacer_line;  //Правый пробел для линии

    bool StateConnect; //Текущее состояние отображения

};

#endif // COMCONNECT_H
