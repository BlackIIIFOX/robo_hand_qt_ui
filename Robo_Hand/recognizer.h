#ifndef RECOGNIZER_H
#define RECOGNIZER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QApplication>
#include <QDir>
#include <QFileInfo>


struct JsonDocument
{
    QString path;
    QJsonDocument document;
};



class Recognizer : public QObject
{
    Q_OBJECT
public:
    explicit Recognizer(QObject *parent = nullptr);
    QString RecognizeWord(QList<double> cepstral_coef);
    bool AppendCepstralCoefToTem(QString name_tem, QString name_wave_file, QList<double> cepstral_coef);

signals:

public slots:



private:
    void CreateModels();
    void LoadData();
    double EuclideanDistance(QList<double> a, QList<double> b);

    QFileInfoList GetListFile();

    QString path_to_prog;
    QString path_to_date;
    QList<JsonDocument> ListJsonDocuments;
};


#endif // RECOGNIZER_H
