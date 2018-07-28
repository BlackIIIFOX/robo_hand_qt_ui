#include "leftsidemenu.h"
#include <QDebug>


LeftSideMenu::LeftSideMenu(QWidget *parent) : QWidget(parent)
{
    left_layout = new QVBoxLayout();
    this->setMinimumSize(70,650);
    this->resize(70,650);
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    left_layout->setMargin(0);
    left_layout->setSpacing(0);
    //left_layout->addWidget(ui->MenuWidg);
    //ui->MenuWidg->hide();
    StateDisplay = true;
    //Создание объектов
    MenuWidg = new HeadLeftWidget(this);
    widg_common = new WidgetMenu(this,"/img/icons8-главная-filled-50.png","Общее");
    widg_action = new WidgetMenu(this,"/img/icons8-горизонтальный-микшер-настроек-filled-50.png","Дейсвтия");
    widg_option = new WidgetMenu(this,"/img/icons8-сервисы-filled-50.png","Настройки");
    spacer = new QSpacerItem(50,50,QSizePolicy::Expanding,QSizePolicy::Expanding);

    //Задание конектов
    connect(MenuWidg,SIGNAL(Convolution()),this,SLOT(ChandgeDisplay()));
    connect(MenuWidg,SIGNAL(Convolution()),widg_common,SLOT(ChandgeDisplay()));
    connect(MenuWidg,SIGNAL(Convolution()),widg_action,SLOT(ChandgeDisplay()));
    connect(MenuWidg,SIGNAL(Convolution()),widg_option,SLOT(ChandgeDisplay()));
    connect(widg_common,SIGNAL(mouseClicked()),widg_action,SLOT(UnSelected()));
    connect(widg_common,SIGNAL(mouseClicked()),widg_option,SLOT(UnSelected()));
    connect(widg_action,SIGNAL(mouseClicked()),widg_common,SLOT(UnSelected()));
    connect(widg_action,SIGNAL(mouseClicked()),widg_option,SLOT(UnSelected()));
    connect(widg_option,SIGNAL(mouseClicked()),widg_common,SLOT(UnSelected()));
    connect(widg_option,SIGNAL(mouseClicked()),widg_action,SLOT(UnSelected()));

    widg_common->Selected();
    //Задание расположения
    left_layout->addWidget(MenuWidg);
    left_layout->addWidget(widg_common);
    left_layout->addWidget(widg_action);
    left_layout->addWidget(widg_option);
    left_layout->addSpacerItem(spacer);
    this->setLayout(left_layout);

    //Удаление слоя
    /*QLayout* layout = ui->LeftSide->layout ();
    if (layout != 0)
    {
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != 0)
        layout->removeItem (item);
    delete layout;
    }*/
}

void LeftSideMenu::ChandgeDisplay()
{
    StateDisplay = !StateDisplay;
    //qDebug()<<StateDisplay;
    emit SizeChanged(StateDisplay);
}

void LeftSideMenu::paintEvent(QPaintEvent *pe) {
  QStyleOption o;
  o.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}
