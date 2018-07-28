#ifndef MOVEWIDGET_H
#define MOVEWIDGET_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>

class MoveWidget:public QWidget
{
public:
    MoveWidget(QWidget *parent);
    QPoint mpos;
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // MOVEWIDGET_H
