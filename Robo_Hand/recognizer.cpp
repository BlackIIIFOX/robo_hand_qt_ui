#include "recognizer.h"
#include <QDebug>

Recognizer::Recognizer(QObject *parent) : QObject(parent)
{
    path_to_prog = QCoreApplication::applicationDirPath();
    path_to_date = path_to_prog + "/DATA/Recognize/JSON";
    //qDebug()<<GetListFile();
    LoadData();
}




QString Recognizer::RecognizeWord(QList<double> cepstral_coef)
{
    //cepstral_coef
    int lenght_coef = cepstral_coef.count();
    double min_size = 99999;
    QString word;

    foreach (JsonDocument jsondoc, ListJsonDocuments) {

        QJsonDocument tem = jsondoc.document;
        QJsonArray tem_data = tem["data"].toArray();
        //qDebug()<< tem_data;

        for ( int i = 0; i < tem_data.size(); i++ )
        {
            QJsonArray coef_wav = tem_data[i].toObject().value("coef").toArray();

            if ( lenght_coef != coef_wav.size())
                continue;

            QList<double> cepstal_coef_json;
            for ( int j = 0; j < lenght_coef; j++)
            {
                cepstal_coef_json.append(coef_wav[j].toDouble());
                //qDebug()<<cepstral_coef.value(j)<<"  "<<coef_wav[j];
            }

            double distanse = EuclideanDistance(cepstral_coef, cepstal_coef_json);

            if ( min_size > distanse )
            {
                min_size = distanse;
                word = tem["name"].toString();
            }
            //qDebug()<<distanse;
            //qDebug()<<"\n\n";
        }

    }
    //qDebug()<<word;
    return word;
}


double Recognizer::EuclideanDistance(QList<double> a, QList<double> b)
{
    double result;
    for ( int i = 0; i < a.count(); i++ )
    {
        result = result + pow( a.value(i) - b.value(i) , 2 );
    }
    result = sqrt(result);
    return result;
}




//QJsonArray([{"coef":[6,7,8,9,10],"wavname":"asd.wav"},{"coef":[1,2,3,4,5],"wavname":"231.wav"}])
bool Recognizer::AppendCepstralCoefToTem(QString name_tem, QString name_wave_file, QList<double> cepstral_coef)
{
    bool FlagFindeTem = false;
    QJsonDocument doc;

    int IndexDocument;
    // Поиск или создание документа с таким же именем темы
    for ( int i = 0; i < ListJsonDocuments.count(); i++ )
    {
        doc = ListJsonDocuments.value(i).document;
        if ( doc["name"] != name_tem )
            continue;
        else{
            IndexDocument = i;
            FlagFindeTem = true;
            break;
        }
    }

    if ( FlagFindeTem == false )
    {
        //создать новый json файл
    }


    // Создание нового поля данных
    QJsonObject new_coef;
    QJsonArray new_array_cepstral_coef;
    for ( int i = 0; i < cepstral_coef.count(); i++ )
        new_array_cepstral_coef.append( cepstral_coef.value(i) );
    new_coef.insert("wavname", name_wave_file);
    new_coef.insert("coef", new_array_cepstral_coef);

    //qDebug()<<new_coef;

    QJsonArray tem_data = doc["data"].toArray();

    //Ищем wav файл с таким же именем, если нет, то создаем новые данные
    bool FlagFindeWav = false;
    QJsonArray new_data_object;

    for (int i = 0; i < tem_data.size(); ++i) {
        QJsonObject data = tem_data[i].toObject();
        if ( data.value("wavname").toString() == name_wave_file )
        {
            FlagFindeWav = true;
            new_data_object.append(new_coef);
        }
        else
        {
            new_data_object.append(data);
        }
    }

    if ( FlagFindeWav == false)
    {
        new_data_object.append(new_coef);
    }

    //qDebug()<<new_data_object;
    //Пересоздаем json с изменениями (по другому никака)
    QJsonObject new_object;

    QStringList list_keys = doc.object().keys();

    foreach (QString key, list_keys) {
        //qDebug()<<doc[key];
        if (key != "data")
            new_object.insert(key, doc[key]);
        else
            new_object.insert(key, new_data_object);
    }

    //qDebug()<<new_object;
    JsonDocument prev_elem = ListJsonDocuments.value(IndexDocument); // = new_object;
    prev_elem.document = QJsonDocument(new_object);

    ListJsonDocuments.removeAt(IndexDocument);
    ListJsonDocuments.insert(IndexDocument, prev_elem);

    QFile jsonFile(prev_elem.path);
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(prev_elem.document.toJson());
    jsonFile.close();


    return true;
}






QFileInfoList Recognizer::GetListFile()
{
    QDir dir(path_to_date);
    bool ok = dir.exists();
    QFileInfoList list;
    //QList<QString> list_file;

    if (ok)
    {
        dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
        dir.setNameFilters(QStringList()<<"*.json");
        dir.setSorting(QDir::Name);
        list = dir.entryInfoList();
    }
    return list;

}


void Recognizer::LoadData()
{

    QFileInfoList files = GetListFile();

    QFile file;
    QString val;

    foreach (QFileInfo file_info, files) {
        file.setFileName(file_info.absoluteFilePath());
        file.open(QIODevice::ReadOnly | QIODevice::Text);

        val = file.readAll();
        file.close();

        QJsonDocument document = QJsonDocument::fromJson(val.toUtf8());

        JsonDocument new_doc;
        new_doc.path = file_info.absoluteFilePath();
        new_doc.document = document;

        ListJsonDocuments.append(new_doc);
    }
}
