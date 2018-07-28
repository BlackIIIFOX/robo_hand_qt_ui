#ifndef WIDGETMENU_H
#define WIDGETMENU_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QEvent>
#include <QString>
#include <QLabel>
#include <QHBoxLayout>
#include <QSpacerItem>

class WidgetMenu:public QWidget
{
Q_OBJECT
public:
    explicit WidgetMenu(QWidget *parent = 0,QString name_pic = "",QString s = "");

protected:
    bool event(QEvent * e);


signals:
    void mouseEnter();
    void mouseLeave();
    void mouseClicked();
    void Select();
    void UnSelect();

public slots:
    void ChandgeDisplay();   //Изменить отображение дисплея
    void ChangeSelected();   //Установить виджет, как выбранный
    void Selected();
    void UnSelected();

private:
    QLabel *checking;
    QLabel *label_text;
    QLabel *label_picture;
    QSpacerItem *left_spacer;
    QSpacerItem *midle_spacer;
    QSpacerItem *right_spacer;
    QHBoxLayout *layout_widget;
    bool StateDisplay; //Текущее состояние отображения
    bool StateSelected; //Текущее состояние выбора

};

#endif // WIDGETMENU_H
