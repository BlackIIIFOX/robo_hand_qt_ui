#include "popmenucommand.h"
#include <QDebug>
#include "robohand.h"

PopMenuCommand::PopMenuCommand(QWidget *parent) : QWidget(parent)
{
    //Делаем виджет прозрачным
    op=new QGraphicsOpacityEffect(this);
    op->setOpacity(0.95);
    this->setGraphicsEffect(op);
    this->setAutoFillBackground(false);

    //Установка размеров и стилей
    this->setStyleSheet("background: #353333; border-radius: 15px; margin: 7px;");
    //qDebug()<<parent->size();
    //qDebug()<<((RoboHand*)parent)->ui->LeftSide->widg_action;
    this->resize(parent->width()-215,parent->height()-10);
    this->move(215,10);
    //this->setGeometry(215,10,170,640);
}

void PopMenuCommand::paintEvent(QPaintEvent *pe) {
  QStyleOption o;
  o.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}
