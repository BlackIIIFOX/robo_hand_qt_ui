#include "headleftwidget.h"
#include <QDebug>


HeadLeftWidget::HeadLeftWidget(QWidget *parent) : QWidget(parent)
{
    this->setParent(parent);
    //this->setStyleSheet("background-color: #f00;");
    this->setMinimumSize(70,120);
    this->setMaximumSize(217,120);
    this->show();
    StateDisplay = true;
    //connect(this,SIGNAL(mouseClicked()),this,SLOT(ChangeSelected()));

    checking = new QLabel();
    label_text = new QLabel();
    push_button = new QPushButton();
    layout_widget = new QHBoxLayout();
    left_spacer = new QSpacerItem(25,25,QSizePolicy::Fixed,QSizePolicy::Expanding);
    midle_spacer = new QSpacerItem(15,25,QSizePolicy::Fixed,QSizePolicy::Expanding);
    right_spacer = new QSpacerItem(5,25,QSizePolicy::Expanding,QSizePolicy::Expanding);
    //Создание виджетов внутри
    //this->setAutoFillBackground(true);
    this->setBackgroundRole(QPalette::Background);
    this->setStyleSheet("background: #e8a032;");
    connect(push_button,SIGNAL(clicked(bool)),this,SLOT(ChandgeDisplay()));
    //Настраиваем линию (виджет активирован)
    checking->setMinimumSize(10,90);
    checking->setMaximumWidth(10);
    checking->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
    //checking->setStyleSheet("background: #f00;");



    //Устанавливаем картинку виджета
    push_button->setMinimumSize(35,35);
    push_button->setMaximumSize(35,35);
    push_button->resize(35,35);
    push_button->setStyleSheet("border-image: url(:/img/icons8-меню-24.png) stretch;");

    //Устанавливаем текст виджета
    QFont font("Century Schoolbook",16);
    label_text->setText("CONTROL");
    label_text->setMinimumSize(125,120);
    label_text->setMaximumWidth(125);
    label_text->resize(125,120);
    label_text->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    label_text->setFont(font);
    label_text->setStyleSheet("color:#fff; font-weight: bold;");

    //Задание расположения
    layout_widget->setMargin(0);
    layout_widget->setSpacing(0);

    layout_widget->addWidget(checking);
    layout_widget->addSpacerItem(left_spacer);
    layout_widget->addWidget(push_button);
    layout_widget->addSpacerItem(midle_spacer);
    layout_widget->addWidget(label_text);
    layout_widget->addSpacerItem(right_spacer);
    this->setLayout(layout_widget);
}

void HeadLeftWidget::paintEvent(QPaintEvent *pe) {
  QStyleOption o;
  o.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}


void HeadLeftWidget::ChandgeDisplay()
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
    emit Convolution();
}
