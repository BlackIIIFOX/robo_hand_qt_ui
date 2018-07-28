#include "popmenu.h"
#include <QDebug>
//#include <QGraphicsOpacityEffect>

PopMenu::PopMenu(QWidget *parent,QWidget *side) : QWidget(parent)
{
    this->setStyleSheet("background: #353333; border-radius: 15px; margin: 7px;");
    this->setGeometry(side->width(),10,170,640);
    this->hide();
    op=new QGraphicsOpacityEffect(this);
    op->setOpacity(0.95);
    this->setGraphicsEffect(op);
    this->setAutoFillBackground(false);

    this->setParent(parent);
    //menu->show();
    left_side = side;
    CreateAllWidget();
    StateDisplay = false;

}


void PopMenu::CreateAllWidget()
{
    int count = 4;

    QVBoxLayout *layout_menu = new QVBoxLayout();
    QHBoxLayout *layout_action;
    layout_menu->setMargin(0);
    layout_menu->setSpacing(0);
    QFont font("Century Schoolbook",14);

    QSpacerItem *spacer = new QSpacerItem(50,25,QSizePolicy::Expanding,QSizePolicy::Fixed);
    layout_menu->addSpacerItem(spacer);


    for (int i = 0;i<count;i++)
    {
        //Новый виджет действия
        QWidget *new_widg = new QWidget();
        new_widg->setMinimumSize(170,50);
        layout_menu->addWidget(new_widg);

        //Добавление элементов в виджет
        layout_action = new QHBoxLayout();

        //Добавлем Lable
        QLabel *new_lable = new QLabel();
        new_lable->setText("Действие "+QString::number(i+1));
        new_lable->setFont(font);
        new_lable->setStyleSheet("color:#fff; font-weight: bold;");

        //Добавляем кнопку изменения действия
        QPushButton *button_change;
        button_change = new QPushButton();
        button_change->setMinimumSize(40,40);
        button_change->setMaximumSize(40,40);
        button_change->setStyleSheet("border-image: url(:/img/ChangeAction.png) stretch;");
        //connect(button_change,);

        //Добавляем кнопку удаления действия
        QPushButton *button_delete;
        button_delete = new QPushButton();
        button_delete->setMinimumSize(40,40);
        button_delete->setMaximumSize(40,40);
        button_delete->setStyleSheet("border-image: url(:/img/ActionDelete.png) stretch;");

        //Добавляем layout
        layout_action->addWidget(new_lable);
        layout_action->addWidget(button_change);
        layout_action->addWidget(button_delete);
        new_widg->setLayout(layout_action);
    }

    spacer = new QSpacerItem(10,10,QSizePolicy::Fixed,QSizePolicy::Expanding);
    layout_menu->addSpacerItem(spacer);

    this->setLayout(layout_menu);
}

void PopMenu::HideMenuAction()
{
    StateDisplay = false;
    this->hide();
    emit HiddenMenuAction();
}

void PopMenu::LeftSizeChanged(bool value)
{
    if(StateDisplay == true)
    {
       if(value == true)
            this->setGeometry(215,10,250,640);
       else
            this->setGeometry(70,10,250,640);
    }
}

void PopMenu::ShowMenuAction()
{
    //Если меню уже открыто, то нужно его закрыть
    if(StateDisplay == false)
    {
        StateDisplay = true;
        ResizerMenu();
        this->show();
        emit ShownMenuAction();
    }
    else
    {
        StateDisplay = false;
        this->hide();
        emit HiddenMenuAction();
    }
}

void PopMenu::ResizerMenu()
{
    this->setGeometry(left_side->width(),10,250,640);
}

void PopMenu::paintEvent(QPaintEvent *pe) {
  QStyleOption o;
  o.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}
