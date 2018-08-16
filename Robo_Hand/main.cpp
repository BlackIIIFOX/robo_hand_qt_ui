#include "robohand.h"
#include "signalgraphics.h"
#include <QApplication>
#include <QDesktopWidget>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //RoboHand w;
    //w.show();


    SignalGraphics graph(1000,8000);
//    QThread* thread = new QThread();
//    graph.moveToThread(thread);
//    thread->start();
    graph.setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            graph.size(),
            QApplication::desktop()->availableGeometry()
        )
    );
    graph.setMinimumSize(QApplication::desktop()->width()/3,QApplication::desktop()->height()/2);
    graph.show();
    return a.exec();
}
