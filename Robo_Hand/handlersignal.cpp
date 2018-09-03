#include "handlersignal.h"
#include <QDebug>


HandlerSignal::HandlerSignal(QWidget *parent) : QWidget(parent)
{
    CreateWidgets();
    CreateConnectors();
}

HandlerSignal::HandlerSignal(QList<int> list,QWidget *parent) : QWidget(parent)
{
    data_signal = list;
    if ( data_signal.count() !=0 )
        FourierTransform();
    CreateWidgets();
    CreateConnectors();
}

void HandlerSignal::CreateConnectors()
{
    connect(this, SIGNAL(Resize()), this, SLOT(WidgetResized()));
    connect(SaveWavButton, SIGNAL(clicked()), this, SLOT(PushSaveButton()));
    connect(LoadWavButton, SIGNAL(clicked()), this, SLOT(PushLoadButton()));
    connect(CalcWavButton, SIGNAL(clicked()), this, SLOT(PushCalcButton()));
}


bool HandlerSignal::event(QEvent * e)
{
    //qDebug()<<e->type();
    switch(e->type())
    {
    case QEvent::Resize:
        emit Resize();
        return true;
        break;
    case QEvent::Show:
        //WidgetResized();
        emit Resize();
        return true;
        break;
    default:
        break;
    }
    return QWidget::event(e);
}


void HandlerSignal::PushSaveButton()
{
    //QString str = QFileDialog:rgetExistingDirectory(0, "Directory Dialog", "");
    QString str = QFileDialog::getSaveFileName( nullptr , "Сохранить звуковой файл", "", "*.wav");

    QFile WavFile(str); // Связываем объект с файлом fileout.txt


    if(WavFile.open(QIODevice::WriteOnly))
    {
        QByteArray WavData;
        CreateWavData(&WavData);
        WavFile.write(WavData);
        WavFile.close(); // Закрываем файл

        //rec.AppendCepstralCoefToTem("Сжать", str, List_CepstalMelCoof);
        //rec.AppendCepstralCoefToTem("Клавиатура", str, List_CepstalMelCoof);
        //rec.AppendCepstralCoefToTem("Поворот", str, List_CepstalMelCoof);
        rec.AppendCepstralCoefToTem("Окей", str, List_CepstalMelCoof);
    }
}

void HandlerSignal::CreateWavData(QByteArray* data)
{
    data_signal.append(3000);
    data_signal.append(4000);
    data_signal.append(128);
    data_signal.append(128);
    QByteArray dataSignalByteArray;

    int len_data_signal = data_signal.count();
    for (int i = 0; i < len_data_signal; i++)
    {
        int16_t value = static_cast<int16_t>(data_signal.value(i));
        dataSignalByteArray.append( static_cast<char>( value & (0x00FF) ) );
        dataSignalByteArray.append(value>>8);
    }
    len_data_signal = len_data_signal * 2;

    //data->append(dataSignalByteArray);
    //return;



    WavFileHeader.chunkId = 0x52494646;                 //Содержит символы “RIFF” в ASCII кодировке
    WavFileHeader.chunkSize = static_cast<uint32_t>(len_data_signal) + 36;     //Это оставшийся размер цепочки, начиная с этой позиции. Иначе говоря, это размер файла – 8, то есть, исключены поля chunkId и chunkSize.
    WavFileHeader.format = 0x57415645 ;                 //Содержит символы “WAVE”
    WavFileHeader.subchunk1Id = 0x666d7420;             //Содержит символы “fmt“
    WavFileHeader.subchunk1Size = 16;                   //16 для формата PCM. Это оставшийся размер подцепочки, начиная с этой позиции.
    WavFileHeader.audioFormat = 1;                      //Аудио формат. Для PCM = 1 (то есть, Линейное квантование). Значения, отличающиеся от 1, обозначают некоторый формат сжатия.
    WavFileHeader.numChannels = 1;                      //Количество каналов. Моно = 1, Стерео = 2 и т.д.
    WavFileHeader.sampleRateWav = 8000;                 //Частота дискретизации. 8000 Гц, 44100 Гц и т.д.
    WavFileHeader.byteRate = 16000;                     //Количество байт, переданных за секунду воспроизведения.
    WavFileHeader.blockAlign = 2;                       //Количество байт для одного сэмпла, включая все каналы.
    WavFileHeader.bitsPerSample = 16;                   //Количество бит в сэмпле. Так называемая “глубина” или точность звучания. 8 бит, 16 бит и т.д.
    WavFileHeader.subchunk2Id = 0x64617461;             //Содержит символы “data”
    WavFileHeader.subchunk2Size = static_cast<uint32_t>(len_data_signal);      //Количество байт в области данных.
    //uint16_t* data;                                   //Непосредственно WAV-данные.

    //Записываем "RIFF"
    for ( int i = 3; i >= 0; i-- )
        data->append( static_cast<char>( ( WavFileHeader.chunkId >> (i * 8) ) & 0xFF ) );

    //Пишем длинну без первых двух байт
    for ( int i = 0; i < 4; i++ )
        data->append( static_cast<char>( ( WavFileHeader.chunkSize >> (i * 8) ) & 0xFF) );

    //Записываем "WAVE"
    for ( int i = 3; i >= 0; i-- )
        data->append( static_cast<char>( ( WavFileHeader.format >> (i * 8) ) & 0xFF ) );

    //Записываем "fmt"
    for ( int i = 3; i >= 0; i-- )
        data->append( static_cast<char>( ( WavFileHeader.subchunk1Id >> (i * 8) ) & 0xFF ) );

    //Пишем 16 (размер Subchunk1)
    for ( int i = 0; i < 4; i++ )
        data->append( static_cast<char>( ( WavFileHeader.subchunk1Size >> (i * 8) ) & 0xFF ) );

    //Пишем 1 (формат PCM)
    for ( int i = 0; i < 2; i++ )
        data->append( static_cast<char>( ( WavFileHeader.audioFormat >> (i * 8) ) & 0xFF ) );

    //Пишем кол-во каналов
    for ( int i = 0; i < 2; i++ )
        data->append( static_cast<char>( ( WavFileHeader.numChannels >> (i * 8) ) & 0xFF ) );

    //Пишем частоту дискретизации
    for ( int i = 0; i < 4; i++ )
        data->append( static_cast<char>( ( WavFileHeader.sampleRateWav >> (i * 8) ) & 0xFF) );

    //Пишем кол-во байт на секунду воспроизведения
    for ( int i = 0; i < 4; i++ )
        data->append( static_cast<char>( ( WavFileHeader.byteRate >> (i * 8) ) & 0xFF) );

    //Пишем кол-во байт для одного семпла
    for ( int i = 0; i < 2; i++ )
        data->append( static_cast<char>( ( WavFileHeader.blockAlign >> (i * 8) ) & 0xFF ) );

    //Пишем кол-во бит в одном семпле
    for ( int i = 0; i < 2; i++ )
        data->append( static_cast<char>( ( WavFileHeader.bitsPerSample >> (i * 8) ) & 0xFF ) );

    //Записываем "data"
    for ( int i = 3; i >= 0; i-- )
        data->append( static_cast<char>( ( WavFileHeader.subchunk2Id >> (i * 8) ) & 0xFF ) );

    //Пишем длинну data
    for ( int i = 0; i < 4; i++ )
        data->append( static_cast<char>( ( WavFileHeader.subchunk2Size >> (i * 8) ) & 0xFF) );


    data->append(dataSignalByteArray);

}

void HandlerSignal::PushLoadButton()
{
    QString str = QFileDialog::getOpenFileName( nullptr, "Открыть звуковой файл", "", "*.wav");

    QFile WavFile(str);
    QByteArray WavData;
    if(WavFile.open(QIODevice::ReadOnly))
    {


        WavData = WavFile.read(WavFile.size());
        WavFile.close(); // Закрываем файл


        WavFileHeader = StuctWavFail();
        //Загружаем "RIFF"
        for ( int i = 3; i >= 0; i-- )
            WavFileHeader.chunkId = WavFileHeader.chunkId | static_cast<uint32_t>( ( WavData[i] << (i * 8) ) );

        //Загружаем длинну без первых двух байт
        for ( int i = 0; i < 4; i++ )
            WavFileHeader.chunkSize = WavFileHeader.chunkSize | ( static_cast<uint32_t>( ( WavData[i+4] << (i * 8) ) ) );

        //Загружаем "WAV"
        for ( int i = 3; i >= 0; i-- )
            WavFileHeader.format = WavFileHeader.format | static_cast<uint32_t>( ( WavData[i + 8] << (i * 8) ) );

        //Загружаем "fmt "
        for ( int i = 3; i >= 0; i-- )
            WavFileHeader.subchunk1Id = WavFileHeader.subchunk1Id | static_cast<uint32_t>( ( WavData[i + 12] << (i * 8) ) );


        //Загружаем размер Subchunk1
        for ( int i = 0; i < 4; i++ )
            WavFileHeader.subchunk1Size = WavFileHeader.subchunk1Size | ( static_cast<uint32_t>( ( WavData[i+16] << (i * 8) ) ) );


        //Загружаем AudioFormat
        for ( int i = 0; i < 2; i++ )
            WavFileHeader.audioFormat = WavFileHeader.audioFormat | ( static_cast<uint16_t>( ( WavData[i+20] << (i * 8) ) ) );

        //Загружаем кол-во каналов
        for ( int i = 0; i < 2; i++ )
            WavFileHeader.numChannels = WavFileHeader.numChannels | ( static_cast<uint16_t>( ( WavData[i+22] << (i * 8) ) ) );

        //Загружаем частоту дискретизации
        for ( int i = 0; i < 4; i++ )
            WavFileHeader.sampleRateWav = WavFileHeader.sampleRateWav | ( static_cast<uint32_t>( ( WavData[i+24] << (i * 8) ) ) );

        //Загружаем кол-во байт на секунду воспроизведения
        for ( int i = 0; i < 4; i++ )
            WavFileHeader.byteRate = WavFileHeader.byteRate | ( static_cast<uint32_t>( ( WavData[i+28] << (i * 8) ) ) );


        //Пишем кол-во байт для одного семпла
        for ( int i = 0; i < 2; i++ )
            WavFileHeader.blockAlign = WavFileHeader.blockAlign | ( static_cast<uint16_t>( ( WavData[i+32] << (i * 8) ) ) );


        //Пишем кол-во бит в одном семпле
        for ( int i = 0; i < 2; i++ )
            WavFileHeader.bitsPerSample = WavFileHeader.bitsPerSample | ( static_cast<uint16_t>( ( WavData[i+34] << (i * 8) ) ) );


        //Загружаем "RIFF"
        for ( int i = 3; i >= 0; i-- )
            WavFileHeader.subchunk2Id = WavFileHeader.subchunk2Id | static_cast<uint32_t>( ( WavData[i+36] << (i * 8) ) );


        //Загружаем размер data
        for ( int i = 0; i < 4; i++ )
            WavFileHeader.subchunk2Size = WavFileHeader.subchunk2Size | ( static_cast<uint32_t>( ( WavData[i+40] << (i * 8) ) ) );

        QList<int> list_data;
        uint value1, value2;
        int res;
        for ( int i = 44; i < static_cast<int>( WavFileHeader.subchunk2Size + 44) ; i = i + 2 )
        {
            // Ебаное преобразование типов, только так заработало. Не хотело делать побитовую операцию или над отрицательным числом
            value1 = 0;
            value2 = 0;
            value1 = static_cast<uchar> ( WavData[i] );
            value2 = static_cast<uchar> ( WavData[i + 1] ) << 8;
            res =  ( value2 | value1 );
            //value = static_cast<uint>( WavData[i] );
            //value = value | ( ( static_cast<uint>( WavData[i+1] ) << 8 ));
            list_data.append( res );
        }

        HandlerSignal* new_form = new HandlerSignal(list_data);
        new_form->show();


    }
}

void HandlerSignal::PushCalcButton()
{
    QString result;
    result = rec.RecognizeWord(List_CepstalMelCoof);
    qDebug()<<result;
    //rec.AppendCepstralCoefToTem("Сжать", "", List_CepstalMelCoof);
}


//_________________________________________________________________________________________
//_________________________________________________________________________________________
//
// NAME:          FFT.
// PURPOSE:       Быстрое преобразование Фурье: Комплексный сигнал в комплексный спектр и обратно.
//                В случае действительного сигнала в мнимую часть (Idat) записываются нули.
//                Количество отсчетов - кратно 2**К - т.е. 2, 4, 8, 16, ... (см. комментарии ниже).
//
//
// PARAMETERS:
//
//    float *Rdat    [in, out] - Real part of Input and Output Data (Signal or Spectrum)
//    float *Idat    [in, out] - Imaginary part of Input and Output Data (Signal or Spectrum)
//    int    N       [in]      - Input and Output Data length (Number of samples in arrays)
//    int    LogN    [in]      - Logarithm2(N)
//    int    Ft_Flag [in]      - Ft_Flag = FT_ERR_DIRECT  (i.e. -1) - Direct  FFT  (Signal to Spectrum)
//		                 Ft_Flag = FT_ERR_INVERSE (i.e.  1) - Inverse FFT  (Spectrum to Signal)
//
// RETURN VALUE:  false on parameter error, true on success.
//_________________________________________________________________________________________
//
// NOTE: In this algorithm N and LogN can be only:
//       N    = 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384;
//       LogN = 2, 3,  4,  5,  6,   7,   8,   9,   10,   11,   12,   13,    14;
//_________________________________________________________________________________________
//_________________________________________________________________________________________


#define  NUMBER_IS_2_POW_K(x)   ((!((x)&((x)-1)))&&((x)>1))  // x is pow(2, k), k=1,2, ...
#define  FT_DIRECT        -1    // Direct transform.
#define  FT_INVERSE        1    // Inverse transform.


bool  FFT(float *Rdat, float *Idat, int N, int LogN, int Ft_Flag)
{
    // parameters error check:
    if((Rdat == NULL) || (Idat == NULL))                  return false;
    if((N > 16384) || (N < 1))                            return false;
    if(!NUMBER_IS_2_POW_K(N))                             return false;
    if((LogN < 2) || (LogN > 14))                         return false;
    if((Ft_Flag != FT_DIRECT) && (Ft_Flag != FT_INVERSE)) return false;

    int  i, j, n, k, io, ie, in, nn;
    float         ru, iu, rtp, itp, rtq, itq, rw, iw, sr;

    static const float Rcoef[14] =
    {  -1.0000000000000000F,  0.0000000000000000F,  0.7071067811865475F,
       0.9238795325112867F,  0.9807852804032304F,  0.9951847266721969F,
       0.9987954562051724F,  0.9996988186962042F,  0.9999247018391445F,
       0.9999811752826011F,  0.9999952938095761F,  0.9999988234517018F,
       0.9999997058628822F,  0.9999999264657178F
    };
    static const float Icoef[14] =
    {   0.0000000000000000F, -1.0000000000000000F, -0.7071067811865474F,
        -0.3826834323650897F, -0.1950903220161282F, -0.0980171403295606F,
        -0.0490676743274180F, -0.0245412285229122F, -0.0122715382857199F,
        -0.0061358846491544F, -0.0030679567629659F, -0.0015339801862847F,
        -0.0007669903187427F, -0.0003834951875714F
    };

    nn = N >> 1;
    ie = N;
    for(n=1; n<=LogN; n++)
    {
        rw = Rcoef[LogN - n];
        iw = Icoef[LogN - n];
        if(Ft_Flag == FT_INVERSE) iw = -iw;
        in = ie >> 1;
        ru = 1.0F;
        iu = 0.0F;
        for(j=0; j<in; j++)
        {
            for(i=j; i<N; i+=ie)
            {
                io       = i + in;
                rtp      = Rdat[i]  + Rdat[io];
                itp      = Idat[i]  + Idat[io];
                rtq      = Rdat[i]  - Rdat[io];
                itq      = Idat[i]  - Idat[io];
                Rdat[io] = rtq * ru - itq * iu;
                Idat[io] = itq * ru + rtq * iu;
                Rdat[i]  = rtp;
                Idat[i]  = itp;
            }

            sr = ru;
            ru = ru * rw - iu * iw;
            iu = iu * rw + sr * iw;
        }

        ie >>= 1;
    }

    for(j=i=1; i<N; i++)
    {
        if(i < j)
        {
            io       = i - 1;
            in       = j - 1;
            rtp      = Rdat[in];
            itp      = Idat[in];
            Rdat[in] = Rdat[io];
            Idat[in] = Idat[io];
            Rdat[io] = rtp;
            Idat[io] = itp;
        }

        k = nn;

        while(k < j)
        {
            j   = j - k;
            k >>= 1;
        }

        j = j + k;
    }

    if(Ft_Flag == FT_DIRECT) return true;

    rw = 1.0F / N;

    for(i=0; i<N; i++)
    {
        Rdat[i] *= rw;
        Idat[i] *= rw;
    }

    return true;
}



void HandlerSignal::FourierTransform()
{
    LenghtDataSpect = data_signal.count();
    int RealLenghtDataSignal = LenghtDataSpect;

    double value = log(LenghtDataSpect)/log(2);
    double dx = 1;


    //Подгоняем размерность под 2^N;
    if (value > 0){
        value = floor((value * dx) +0.5 ) / dx;
    }   else {
        value = ceil((value * dx) - 0.5) / dx;
    }
    LenghtDataSpect = (int)pow(2.0,value);

    Rdat = new float[LenghtDataSpect];
    Idat = new float[LenghtDataSpect];

    for (int i =0; i < LenghtDataSpect; i++)
    {
        if ( i < RealLenghtDataSignal )
            Rdat[i] = data_signal.value(i);
        else
            Rdat[i] = 0;
        Idat[i] = 0;
    }

    //Проводим БПФ над подогнанными данными
    FFT(Rdat, Idat, LenghtDataSpect, (int)value, FT_DIRECT);


    //Фильтр Хэмминга
    float Hi;
    for (int i = 0; i < LenghtDataSpect; i++)
    {
        Hi = (float)(0.54 - 0.46 * cos(2 * M_PI * i/(LenghtDataSpect - 1)));
        Rdat[i] = Rdat[i] * Hi;
    }


    //Отсекаем половину спектра, т.к. он зераклен в действительной части
    for (int i = 0; i < LenghtDataSpect/2; i++)
    {
        data_spectr.append((double)Rdat[i]);
        //Rdat[i] = Rdat[i];
    }


    //Расчет мел параметров
    double MelF;
    double FMin,FMax;
    FMin = 300;
    FMax = 8000;
    //Добавляем минимальные и максимальные мел координаты
    MelF = 1127 * log(1 + FMin/700);
    List_MelF.append(MelF);

    MelF = 1127 * log(1 + FMax/700);
    List_MelF.append(MelF);

    double StepMel;
    StepMel = (List_MelF.value(1) - List_MelF.value(0))/(CountMelBank + 1);

    //Расчитываем координаты треугольников по мел шкале
    for (int i = 0; i < CountMelBank; i++)
    {
        List_MelF.insert(1, List_MelF.value(1) - StepMel );
    }

    //Переводим мел шкалу в частотную область
    for (int i = 0; i < List_MelF.count(); i++)
    {
        List_FreqM.append( 700 * ( exp(List_MelF.value(i)/ 1127) - 1 ));
    }

    //Переводим частотные мел координаты в спектральные(равные реальному кол-ву отсчетов спектра)
    int SpecSize = data_spectr.count();
    for (int i = 0; i < List_FreqM.count(); i++)
    {
        int coord = (int) floor( (SpecSize+1) * List_FreqM.value(i) / sampleRate);
        List_SpecMelCoord.append(coord);
    }


    for (int i = 1; i < List_SpecMelCoord.count() - 1; i++)
    {
        QList<double> MelFilter;

        int PrevMelCoord = List_SpecMelCoord.value(i-1);
        int CurrMelCoord = List_SpecMelCoord.value(i);
        int NextMelCoord = List_SpecMelCoord.value(i+1);


        for (int k = 0; k < SpecSize; k++)
        {
            double value_mel;

            if ( k < PrevMelCoord )
                value_mel = 0;
            if ( PrevMelCoord <= k and k < CurrMelCoord )
                value_mel = ( k - PrevMelCoord ) / ( CurrMelCoord - PrevMelCoord );
            if ( CurrMelCoord <= k and k <= NextMelCoord )
                value_mel = ( NextMelCoord - k ) / ( NextMelCoord - CurrMelCoord );
            if (k > NextMelCoord)
                value_mel = 0;

            MelFilter.append(value_mel);
        }

        List_MelFilter.append(MelFilter);
    }


    for ( int i = 0; i < List_MelFilter.count(); i++)
    {
        double MelCoof = 0;
        QList<double> Hm = List_MelFilter.value(i);

        for ( int j = 0; j <  SpecSize; j++ )
        {
            MelCoof =MelCoof + ( pow(abs(data_spectr.value(j)),2) * Hm.value(j) );
            //List_MelCoof
        }
        MelCoof = log (MelCoof);

        List_MelCoof.append(MelCoof);
    }

    int count_list_MelCoof = List_MelCoof.count();

    for ( int l =0; l < count_list_MelCoof; l++ )
    {
        double CepstalMelCoof = 0;

        for ( int m = 0; m < count_list_MelCoof; m++ )
        {
            CepstalMelCoof = CepstalMelCoof + List_MelCoof.value(m) * cos ( l * M_PI * ( m + 1/2) / count_list_MelCoof );
        }

        List_CepstalMelCoof.append(CepstalMelCoof);
    }


}

void HandlerSignal::RedrawButtons()
{
    QWidget* parent_widget = (QWidget*)SaveWavButton->parent();
    SaveWavButton->setMaximumSize(parent_widget->width()/4,parent_widget->height()/12);
    SaveWavButton->setMinimumSize(parent_widget->width()/4,parent_widget->height()/12);

    parent_widget = (QWidget*)LoadWavButton->parent();
    LoadWavButton->setMaximumSize(parent_widget->width()/4,parent_widget->height()/12);
    LoadWavButton->setMinimumSize(parent_widget->width()/4,parent_widget->height()/12);

    parent_widget = (QWidget*)CalcWavButton->parent();
    CalcWavButton->setMaximumSize(parent_widget->width()/4,parent_widget->height()/12);
    CalcWavButton->setMinimumSize(parent_widget->width()/4,parent_widget->height()/12);
}


void HandlerSignal::CreateWidgets()
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    graph_view_signal = new QGraphicsView();
    graph_view_spect = new QGraphicsView();
    graph_view_mfcc = new QGraphicsView();
    graph_scene_signal = new QGraphicsScene();
    graph_scene_spect = new QGraphicsScene();
    graph_scene_mfcc = new QGraphicsScene();
    SaveWavButton = new QPushButton();
    LoadWavButton = new QPushButton();
    CalcWavButton = new QPushButton();

    SaveWavButton->setText("Сохранить WAV");
    LoadWavButton->setText("Загрузить WAV");
    CalcWavButton->setText("Выполнить перерасчет WAV");

    QVBoxLayout* button_layout = new QVBoxLayout();
    QHBoxLayout* button_layout_Save = new QHBoxLayout();
    QHBoxLayout* button_layout_Load = new QHBoxLayout();
    QHBoxLayout* button_layout_Calc = new QHBoxLayout();
    QWidget* widget_buttons = new QWidget();

    SaveWavButton->setParent(widget_buttons);
    LoadWavButton->setParent(widget_buttons);
    CalcWavButton->setParent(widget_buttons);

    //    button_layout->addWidget(SaveWavButton);
    //    button_layout->addWidget(LoadWavButton);
    //    button_layout->addWidget(CalcWavButton);

    button_layout_Save->addStretch();
    button_layout_Save->addWidget(SaveWavButton);
    button_layout_Save->addStretch();

    button_layout_Load->addStretch();
    button_layout_Load->addWidget(LoadWavButton);
    button_layout_Load->addStretch();

    button_layout_Calc->addStretch();
    button_layout_Calc->addWidget(CalcWavButton);
    button_layout_Calc->addStretch();

    button_layout->addLayout(button_layout_Save);
    button_layout->addLayout(button_layout_Load);
    button_layout->addLayout(button_layout_Calc);
    button_layout->addStretch();

    widget_buttons->setLayout(button_layout);

    QGridLayout *layout = new QGridLayout();
    layout->addWidget(graph_view_signal,0,0);
    layout->addWidget(graph_view_spect,0,1);
    layout->addWidget(graph_view_mfcc,1,0);
    layout->addWidget(widget_buttons,1,1);
    this->setLayout(layout);

    graph_view_signal->setScene(graph_scene_signal);
    graph_view_spect->setScene(graph_scene_spect);
    graph_view_mfcc->setScene(graph_scene_mfcc);

    graph_view_signal->setRenderHint( QPainter::Antialiasing );
    graph_view_signal->setStyleSheet("border-style: none");
    graph_view_spect->setRenderHint( QPainter::Antialiasing );
    graph_view_spect->setStyleSheet("border-style: none");
    graph_view_mfcc->setRenderHint( QPainter::Antialiasing );
    graph_view_mfcc->setStyleSheet("border-style: none");
}


void HandlerSignal::paintEvent(QPaintEvent *pe) {
    QStyleOption o;
    o.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}


void HandlerSignal::CalcAndRedrawSignal()
{
    //График сигнала
    int len_height =  graph_view_signal->height();
    int len_width =   graph_view_signal->width();

    graph_scene_signal->setSceneRect( 0, 0, len_width, len_height );
    CalcGraphValue(len_height, len_width, &coordinats_view_signal, &coordinats_arrow_signal);
    coordinats_view_signal.step_x = ((double)(coordinats_view_signal.x_stop - coordinats_view_signal.x_start))/data_signal.count();

    min_y_signal_graph = 99999, max_y_signal_graph = -99999;
    int value;
    for (int i = 0; i < data_signal.count(); i++)
    {
        value = data_signal.value(i);
        if(value<min_y_signal_graph)
            min_y_signal_graph = value;
        if(value>max_y_signal_graph)
            max_y_signal_graph = value;
    }
    coordinats_view_signal.step_y = ((coordinats_view_signal.y_low - coordinats_view_signal.y_high)/(double)(max_y_signal_graph-min_y_signal_graph));
    RedrawGraphicsSignal();
}


void HandlerSignal::CalcAndRedrawSpectr()
{
    //График спектра
    int len_height =  graph_view_spect->height();
    int len_width =   graph_view_spect->width();

    graph_scene_spect->setSceneRect( 0, 0, len_width, len_height );
    CalcGraphValue(len_height, len_width, &coordinats_view_spect, &coordinats_arrow_spect);

    min_y_spect_graph = 99999;
    max_y_spect_graph = -99999;
    double value_spect;
    int LenghtDataSpect = data_spectr.count();

    for (int i = 0; i < LenghtDataSpect; i++)
    {
        value_spect = data_spectr.value(i);
        if (value_spect > LimitValueSpect||value_spect < -LimitValueSpect)
            continue;

        if(value_spect<min_y_spect_graph)
            min_y_spect_graph = value_spect;
        if(value_spect>max_y_spect_graph)
            max_y_spect_graph = value_spect;
    }

    coordinats_view_spect.step_x = ((double)(coordinats_view_spect.x_stop - coordinats_view_spect.x_start))/LenghtDataSpect;
    coordinats_view_spect.step_y = ((coordinats_view_spect.y_low - coordinats_view_spect.y_high)/(double)(max_y_spect_graph-min_y_spect_graph));
    RedrawGraphicsSpec();
}

void HandlerSignal::CalcAndRedrawMFCC()
{
    //График спектра
    int len_height =  graph_view_mfcc->height();
    int len_width =   graph_view_mfcc->width();

    graph_scene_mfcc->setSceneRect( 0, 0, len_width, len_height );
    CalcGraphValue(len_height, len_width, &coordinats_view_mfcc, &coordinats_arrow_mfcc);

    coordinats_view_mfcc.step_x = ((double)(coordinats_view_mfcc.x_stop - coordinats_view_mfcc.x_start))/List_CepstalMelCoof.count();




    min_y_mfcc_graph = 99999;
    max_y_mfcc_graph = -99999;
    double value_mfcc;
    int LenghtDataMfcc = List_CepstalMelCoof.count();

    for (int i = 0; i < LenghtDataMfcc; i++)
    {
        value_mfcc = List_CepstalMelCoof.value(i);

        if(value_mfcc<min_y_mfcc_graph)
            min_y_mfcc_graph = value_mfcc;
        if(value_mfcc>max_y_mfcc_graph)
            max_y_mfcc_graph = value_mfcc;
    }

    coordinats_view_mfcc.step_y = ((coordinats_view_mfcc.y_low - coordinats_view_mfcc.y_high)/(double)(max_y_mfcc_graph-min_y_mfcc_graph));

    RedrawGraphicsMFCC();
}


void HandlerSignal::WidgetResized()
{
    CalcAndRedrawSignal();
    CalcAndRedrawSpectr();
    CalcAndRedrawMFCC();
    RedrawButtons();
}



void HandlerSignal::RedrawGraphicsMFCC()
{

    QPolygonF poly;
    QPainterPath path;

    int Middle = (coordinats_view_mfcc.y_low - coordinats_view_mfcc.y_high)/2;


    for (int i = 0; i < List_CepstalMelCoof.count(); i++)
    {
        double x_1 = coordinats_view_mfcc.x_start + i*coordinats_view_mfcc.step_x;

        double y_1_value = List_CepstalMelCoof.value(i);

        double y_1;
        if (y_1_value>0)
        {
            y_1 = Middle - ((y_1_value)*coordinats_view_mfcc.step_y);
        }
        else
        {
            y_1_value = abs((int)y_1_value);
            y_1 = Middle + ((y_1_value)*coordinats_view_mfcc.step_y);
        }

        poly.append(QPointF(x_1,y_1));

    }

    graph_scene_mfcc->clear();
    //PaintBaseItemInScene();
    //PrintArrowGraph();

    path.addPolygon(poly);

    //graph_view_spect->setStyleSheet();
    graph_scene_mfcc->addLine(QLine(coordinats_arrow_mfcc.ZeroHLineX,Middle,coordinats_arrow_mfcc.EndHLineX,Middle));
    graph_scene_mfcc->addLine(QLine(coordinats_arrow_mfcc.ZeroVLineX,coordinats_arrow_mfcc.ZeroVLineY,coordinats_arrow_mfcc.EndVLineX,coordinats_arrow_mfcc.EndVLineY));

    graph_scene_mfcc->addPath(path);
}





void HandlerSignal::RedrawGraphicsSpec()
{
    QPolygonF poly;
    QPainterPath path;

    int Middle = (coordinats_view_spect.y_low - coordinats_view_spect.y_high)/2;


    for (int i = 0; i < data_spectr.count(); i++)
    {
        double x_1 = coordinats_view_spect.x_start + i*coordinats_view_spect.step_x;

        double y_1_value = data_spectr.value(i);

        if (y_1_value < -LimitValueSpect)
            y_1_value = min_y_spect_graph;

        if (y_1_value > LimitValueSpect)
            y_1_value = max_y_spect_graph;

        double y_1;
        if (y_1_value>0)
        {
            y_1 = Middle - ((y_1_value)*coordinats_view_spect.step_y);
        }
        else
        {
            y_1_value = abs((int)y_1_value);
            y_1 = Middle + ((y_1_value)*coordinats_view_spect.step_y);
        }

        poly.append(QPointF(x_1,y_1));

    }

    graph_scene_spect->clear();
    //PaintBaseItemInScene();
    //PrintArrowGraph();

    path.addPolygon(poly);

    //graph_view_spect->setStyleSheet();
    graph_scene_spect->addLine(QLine(coordinats_arrow_spect.ZeroHLineX,Middle,coordinats_arrow_spect.EndHLineX,Middle));
    graph_scene_spect->addLine(QLine(coordinats_arrow_spect.ZeroVLineX,coordinats_arrow_spect.ZeroVLineY,coordinats_arrow_spect.EndVLineX,coordinats_arrow_spect.EndVLineY));

    graph_scene_spect->addPath(path);
}



void HandlerSignal::RedrawGraphicsSignal()
{
    QPolygonF poly;
    QPainterPath path;


    for (int i = 0; i < data_signal.count(); i++)
    {
        //if(i%8 != 0) continue;
        double x_1 = coordinats_view_signal.x_start + i*coordinats_view_signal.step_x;

        double y_1_value = data_signal.value(i);

        double y_1 = coordinats_view_signal.y_low - (((int)y_1_value-min_y_signal_graph)*coordinats_view_signal.step_y);

        poly.append(QPointF(x_1,y_1));

    }

    graph_scene_signal->clear();
    //PaintBaseItemInScene();
    //PrintArrowGraph();

    path.addPolygon(poly);
    graph_scene_signal->addPath(path);
}



void HandlerSignal::CalcGraphValue(int len_height, int len_width, GraphCoord* coordinats, GraphArrowCoord* coordinats_arrow)
{
    //Перерисовака линий и прочего
    int ZeroVLineX = 20, ZeroVLineY = 10,
            EndVLineX = 20, EndVLineY = len_height-20,
            ZeroHLineX = 20, ZeroHLineY = len_height-20,
            EndHLineX = len_width-20, EndHLineY = len_height-20;

    coordinats_arrow->EndVLineY = EndVLineY;
    coordinats_arrow->ZeroHLineY = ZeroHLineY;
    coordinats_arrow->EndHLineX = EndHLineX;
    coordinats_arrow->EndHLineY = EndHLineY;

    coordinats_arrow->len_height = len_height;
    coordinats_arrow->len_width = len_width;

    coordinats->x_start = ZeroHLineX;
    coordinats->x_stop = EndHLineX;
    coordinats->y_high = ZeroVLineY;
    coordinats->y_low = EndVLineY;

    //coordinats->step_x = ((double)(coordinats->x_stop - coordinats->x_start))/(((double)TimeDisplayMs/1000.0)*(double)ADCFreque);
    //coordinats->step_y = ((coordinats->y_low - coordinats->y_high)/700.0);
}
