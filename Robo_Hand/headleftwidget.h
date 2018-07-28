#ifndef HEADLEFTWIDGET_H
#define HEADLEFTWIDGET_H

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

class HeadLeftWidget:public QWidget
{
Q_OBJECT
public:
    explicit HeadLeftWidget(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *pe);

signals:
    void Convolution();      //Кнопка свертки/развертки нажата и размер изменен

public slots:
    void ChandgeDisplay();   //Изменить отображение дисплея

private:
    QLabel *checking;
    QLabel *label_text;
    QPushButton *push_button;
    QSpacerItem *left_spacer;
    QSpacerItem *midle_spacer;
    QSpacerItem *right_spacer;
    QHBoxLayout *layout_widget;
    bool StateDisplay; //Текущее состояние отображения

};

#endif // HEADLEFTWIDGET_H
