#ifndef LEFTSIDEMENU_H
#define LEFTSIDEMENU_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QEvent>
#include <QString>
#include <QLabel>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QStyleOption>
#include <QPainter>

#include "headleftwidget.h"
#include "widgetmenu.h"

class LeftSideMenu:public QWidget
{
Q_OBJECT
public:
    explicit LeftSideMenu(QWidget *parent = 0);
    WidgetMenu *widg_action;
    WidgetMenu *widg_common;
    WidgetMenu *widg_option;
    bool StateDisplay;

protected:
    void paintEvent(QPaintEvent *pe);

signals:
    void SizeChanged(bool);

public slots:
    void ChandgeDisplay();

private:
    HeadLeftWidget *MenuWidg;
    QVBoxLayout *left_layout;
    QSpacerItem *spacer;


};

#endif // LEFTSIDEMENU_H
