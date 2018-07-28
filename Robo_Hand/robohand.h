#ifndef ROBOHAND_H
#define ROBOHAND_H

#include <QMainWindow>
#include "uisetting.h"
#include <QTimer>
#include <QDebug>
#include "movewidget.h"
#include "popmenu.h"
#include "widgetmenu.h"
#include "popmenucommand.h"

namespace Ui {
class RoboHand;
}

class RoboHand : public QMainWindow
{
    Q_OBJECT

public:
    explicit RoboHand(QWidget *parent = 0);
    ~RoboHand();

private slots:
    void on_ExitButt_clicked();
    void TimerUpdate();
    void on_TurnBut_clicked();

private:
    PopMenuCommand *menu_command;
    PopMenu* menu_action;
    Ui::RoboHand *ui;
    uiSetting *SettingUI;
    QTimer *timer;
    float opacity;
    int StateWindow; //0 - запуск, 1 - закрытие
};

#endif // ROBOHAND_H
