#include "widgetmenu.h"
#include <QDebug>
#include <QApplication>

WidgetMenu::WidgetMenu(QWidget *parent,QString name_pic,QString text) : QWidget(parent)
{
    this->setParent(parent);
    //this->setStyleSheet("background-color: #f00;");
    this->setMinimumSize(70,90);
    this->setMaximumSize(217,90);
    this->show();
    StateDisplay = true;
    StateSelected = false;
    //connect(this,SIGNAL(mouseClicked()),this,SLOT(ChangeSelected()));
    connect(this,SIGNAL(mouseClicked()),this,SLOT(Selected()));

    checking = new QLabel();
    label_text = new QLabel();
    label_picture = new QLabel();
    layout_widget = new QHBoxLayout();
    left_spacer = new QSpacerItem(25,25,QSizePolicy::Fixed,QSizePolicy::Expanding);
    midle_spacer = new QSpacerItem(15,25,QSizePolicy::Fixed,QSizePolicy::Expanding);
    right_spacer = new QSpacerItem(5,25,QSizePolicy::Expanding,QSizePolicy::Expanding);
    //Создание виджетов внутри


    //Настраиваем линию (виджет активирован)
    checking->setMinimumSize(10,90);
    checking->setMaximumWidth(10);
    checking->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
    //checking->setStyleSheet("background: #f00;");



    //Устанавливаем картинку виджета
    label_picture->setMinimumSize(35,35);
    label_picture->setMaximumSize(35,35);
    label_picture->resize(35,35);
    label_picture->setStyleSheet("border-image: url(:"+name_pic+") stretch;");

    //Устанавливаем текст виджета
    QFont font("Century Schoolbook",16);
    label_text->setText(text);
    label_text->setMinimumSize(125,90);
    label_text->setMaximumWidth(125);
    label_text->resize(125,90);
    label_text->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    label_text->setFont(font);
    label_text->setStyleSheet("color:#fff; font-weight: bold;");

    //Задание расположения
    layout_widget->setMargin(0);
    layout_widget->setSpacing(0);

    layout_widget->addWidget(checking);
    layout_widget->addSpacerItem(left_spacer);
    layout_widget->addWidget(label_picture);
    layout_widget->addSpacerItem(midle_spacer);
    layout_widget->addWidget(label_text);
    layout_widget->addSpacerItem(right_spacer);
    this->setLayout(layout_widget);
}

bool WidgetMenu::event(QEvent * e)
{
    switch(e->type())
    {
    case QEvent::Enter:
        emit mouseEnter();
        return true;
        break;
    case QEvent::Leave:
        emit mouseLeave();
        return true;
        break;
    case QEvent::MouseButtonRelease:
        emit mouseClicked();
        return true;
        break;
    default:
        break;
    }
    return QWidget::event(e);
}

void WidgetMenu::ChandgeDisplay()
{
    StateDisplay = !StateDisplay;
    if(StateDisplay==true)
    {
        left_spacer->changeSize(25,25,QSizePolicy::Fixed,QSizePolicy::Expanding);
        midle_spacer->changeSize(15,25,QSizePolicy::Fixed,QSizePolicy::Expanding);
        label_text->show();
    }
    else
    {
        left_spacer->changeSize(8,25,QSizePolicy::Fixed,QSizePolicy::Expanding);
        midle_spacer->changeSize(0,0,QSizePolicy::Fixed,QSizePolicy::Fixed);
        label_text->hide();
    }
}

void WidgetMenu::ChangeSelected()
{
    StateSelected = !StateSelected;
    if(StateSelected==true)
    {
        checking->setStyleSheet("background: #f00;");
    }
    else
    {
        checking->setStyleSheet("");
    }
}

void WidgetMenu::Selected()
{
    StateSelected = true;
    checking->setStyleSheet("background: #f00;");
    emit Select();
}

void WidgetMenu::UnSelected()
{
    StateSelected = false;
    checking->setStyleSheet("");
    emit UnSelect();
}
