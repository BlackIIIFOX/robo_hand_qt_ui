#ifndef UISETTING_H
#define UISETTING_H

#include "ui_robohand.h"
#include <popmenu.h>

class uiSetting
{
public:
    uiSetting(Ui::RoboHand *new_ui);
    void SetStylesUI();  //Первоначальная установка
    void SetMenu();      //Отображение всего меню
private slots:
    void ShowMenu();     //Отображает кнопки меню
    void HideMenu();     //Скрыает кнопки меню
    void SetBaseStyle();  //Первоначальная установка стиля (задание иконок и прочее)

private:
    Ui::RoboHand *ui;
    bool StateMenu;
};

#endif // UISETTING_H
