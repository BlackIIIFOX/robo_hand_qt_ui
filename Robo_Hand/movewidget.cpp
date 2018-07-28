#include "movewidget.h"
#include <QMouseEvent>

MoveWidget::MoveWidget(QWidget *parent):QWidget(parent)
{

}

void MoveWidget::mouseMoveEvent(QMouseEvent *event){
    if(event->buttons()&&Qt::LeftButton){
        int x,y;
        x=event->pos().x()-mpos.x()+this->parentWidget()->parentWidget()->parentWidget()->geometry().left()-this->geometry().left();
        y=event->pos().y()-mpos.y()+this->parentWidget()->parentWidget()->parentWidget()->geometry().top()-this->geometry().top();

        this->parentWidget()->parentWidget()->parentWidget()->move(x,y);
    }

}

void MoveWidget::mousePressEvent(QMouseEvent *event){
    mpos = event->pos();
}
