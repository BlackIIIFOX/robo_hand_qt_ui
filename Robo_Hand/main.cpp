//#include "robohand.h"
#include "signalgraphics.h"
#include <QApplication>
#include <QDesktopWidget>
#include "handlersignal.h"
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //RoboHand w;
    //w.show();

//    Recognizer rec;
//    QList<double> list;
//    list<<-5<<2<<3<<5<<3;
//    //qDebug()<<list;
//    //rec.RecognizeWord(list);
//    rec.AppendCepstralCoefToTem("Сжать","asd.wav", list);

//    HandlerSignal signal;
//    signal.show();

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
