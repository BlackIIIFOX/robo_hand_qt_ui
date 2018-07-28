#ifndef POPMENU_H
#define POPMENU_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QGraphicsOpacityEffect>
#include <QPushButton>
#include <QEvent>
#include <QStyleOption>
#include <QPainter>

class PopMenu:public QWidget
{
Q_OBJECT
public:
    explicit PopMenu(QWidget *parent = 0, QWidget *side = nullptr);
    //explicit PopMenu(QWidget *parent = nullptr, QWidget *side = nullptr);
    void ResizerMenu();
    void CreateAllWidget();

protected:
    void paintEvent(QPaintEvent *pe);

signals:
    void ShownMenuAction();
    void HiddenMenuAction();

public slots:
    void LeftSizeChanged(bool);
    void HideMenuAction();
    void ShowMenuAction();

private:
    QWidget *menu;
    QWidget *left_side;
    QGraphicsOpacityEffect *op;
    bool StateDisplay; //Текущее состояние отображения
};

#endif // POPMENU_H
