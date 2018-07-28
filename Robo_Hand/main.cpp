#include "robohand.h"
#include "signalgraphics.h"
#include <QApplication>
#include <QDesktopWidget>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //RoboHand w;
    //w.show();


    SignalGraphics graph;
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
    graph.show();
    return a.exec();
}
