#include "robohand.h"
#include "ui_robohand.h"
#include "headleftwidget.h"

RoboHand::RoboHand(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RoboHand)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnBottomHint); //Скрыаем кнопки приложения
    SettingUI = new uiSetting(ui);

    SettingUI->SetStylesUI();

    //Запуск таймера для плавного открытия окна
    StateWindow = 0;
    opacity = 0;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(TimerUpdate()));
    timer->start(20);

    //Создание меню действий
    menu_action = new PopMenu(ui->centralWidget,ui->LeftSide);
    connect(ui->LeftSide->widg_action,SIGNAL(Select()),menu_action,SLOT(ShowMenuAction()));
    connect(ui->LeftSide->widg_action,SIGNAL(UnSelect()),menu_action,SLOT(HideMenuAction()));
    connect(ui->LeftSide,SIGNAL(SizeChanged(bool)),menu_action,SLOT(LeftSizeChanged(bool)));


    //Создание меню команд
    //menu_command = new PopMenuCommand(this);

    //SettingUI->UpdatePopMenu(menu);
}

RoboHand::~RoboHand()
{
    delete ui;
}







//===============================================================================
//===============================================================================
//====================================Слоты======================================
//===============================================================================
//===============================================================================


//============Выход из приложения===================
void RoboHand::on_ExitButt_clicked()
{
    StateWindow = 1;
    opacity = 1;
    timer->start(20);
}



//============Измнение видимости окна===============
void RoboHand::TimerUpdate()
{
    this->setWindowOpacity(opacity);
    switch (StateWindow) {
    case 0:
        opacity = opacity + 0.05;
        if(opacity > 1)
            {
                timer->stop();
                setWindowOpacity(1);
            }
        break;
    case 1:
        opacity = opacity - 0.05;
        if(opacity < 0)
            {
                timer->stop();
                this->close();
            }
        break;
    default:
        break;
    }
}


//================Свернуть окно=======================
void RoboHand::on_TurnBut_clicked()
{
    this->showMinimized();
}



//================Измнение меню=======================

//===============================================================================
//===============================================================================
