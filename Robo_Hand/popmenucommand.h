#ifndef POPMENUCOMMAND_H
#define POPMENUCOMMAND_H

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

class PopMenuCommand : public QWidget
{
    Q_OBJECT
public:
    explicit PopMenuCommand(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *pe);

signals:

public slots:

private:
    QGraphicsOpacityEffect *op;
};

#endif // POPMENUCOMMAND_H
