#ifndef MYBUT_H
#define MYBUT_H

#include <QPushButton>
#include <QHoverEvent>
#include <QEvent>
#include <QDebug>

class mybut : public QPushButton
{
    Q_OBJECT
public:
    explicit mybut(QWidget *parent = 0);

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    bool event(QEvent * e);

    void hoverEnter(QHoverEvent * event);
    void hoverLeave(QHoverEvent * event);
    void hoverMove(QHoverEvent * event);

signals:

public slots:

private:
    int m_count;
};

#endif // MYBUT_H
