#include "mybut.h"


mybut::mybut(QWidget *parent) :
    QPushButton(parent)
{
    setAttribute(Qt::WA_Hover);
//    this->setMouseTracking(true);
    this->setText(QString::number(m_count));
}

bool mybut::event(QEvent * e)
{
    switch(e->type())
    {
    case QEvent::HoverEnter:
        hoverEnter(static_cast<QHoverEvent*>(e));
        return true;
        break;
    case QEvent::HoverLeave:
        hoverLeave(static_cast<QHoverEvent*>(e));
        return true;
        break;
    /*case QEvent::HoverMove:
        hoverMove(static_cast<QHoverEvent*>(e));
        return true;
        break;*/
    default:
        break;
    }
    return QWidget::event(e);
}

void mybut::enterEvent(QEvent * e)
{
}

void mybut::leaveEvent(QEvent * e)
{
}


void mybut::hoverEnter(QHoverEvent * event)
{
    this->setStyleSheet("\
                   background-color: CornflowerBlue ;\
                   border-radius: 7px;\
                   border: 2px solid black;" );
}

void mybut::hoverLeave(QHoverEvent * event)
{
    this->setStyleSheet("\
                   background-color: white;\
                   border-radius: 7px;\
                   border: 2px solid black;");
}

void mybut::hoverMove(QHoverEvent * event)
{
    //this->setText(QString::number(m_count));
}
