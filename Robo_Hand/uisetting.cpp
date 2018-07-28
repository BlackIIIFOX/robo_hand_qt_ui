#include "uisetting.h"
#include <QAction>
#include <QDebug>


uiSetting::uiSetting(Ui::RoboHand *new_ui)
{
    ui = new_ui;
}





//===============================================================================
//===============================================================================
//=======================Настройка глобальных стлей==============================
//===============================================================================
//===============================================================================

//================Первоначальная настройка=======================
void uiSetting::SetStylesUI()
{
    SetBaseStyle();
}


void uiSetting::SetMenu()
{
    if(StateMenu == true)
    {
        StateMenu = false;
        HideMenu();
    }
    else
    {
        ShowMenu();
        StateMenu = true;
    }

}




//===============================================================================
//===============================================================================







//===============================================================================
//===============================================================================
//==========================Настройка мелких стилей==============================
//===============================================================================
//===============================================================================

void uiSetting::SetBaseStyle()
{
    //Настройка иконок кнопок
    ui->ExitButt->setStyleSheet("border-image: url(:/img/icons8-удалить-32.png) stretch;");
    ui->TurnBut->setStyleSheet("border-image: url(:/img/icons8-свернуть-32.png) stretch;");
    ui->PicStateHand->setStyleSheet("border-image: url(:/img/icons8-галочка-64.png) stretch;");
    ui->OprionsButt->setStyleSheet("border-image: url(:/img/icons8-сервисы-50.png) stretch;");

}

void uiSetting::HideMenu()
{
    //======Скрытие текста========
    ui->LeftSide->setMinimumWidth(70);
    ui->LeftSide->setMaximumWidth(70);
    ui->LeftSide->resize(70,650);
    //ui->MenuWidg->resize(70,120);
    ui->HeadWidg->resize(930,80);
}

void uiSetting::ShowMenu()
{
    //Выводим скрытый текст
    ui->LeftSide->setMinimumWidth(217);
    ui->LeftSide->setMaximumWidth(217);
    ui->LeftSide->resize(217,650);
    ui->HeadWidg->resize(750,80);
}


//===============================================================================
//===============================================================================


