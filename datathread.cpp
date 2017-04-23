#include "datathread.h"
#include <QThread>
#include <QVector>
#include <QQueue>
#include <QByteArray>
#include <QDataStream>
#include <cmath>
#include <QReadWriteLock>

#define FACTOR (4.5*pow(10.0,6)/24.0/(pow(2.0,23)-1)) //降幅值系数

double factor=FACTOR;

class MainWindow;

extern QVector<quint8> buffer;
extern QVector<QByteArray> RxA;
extern QReadWriteLock lock;

extern QQueue<double> allData;
extern QQueue<double> allData2;
extern QQueue<double> allData3;
extern QQueue<double> allData4;
extern QQueue<double> allData5;
extern QQueue<double> allData6;
extern QQueue<double> allData7;
extern QQueue<double> allData8;

dataThread::dataThread()
{

}

dataThread::~dataThread()
{

}

void dataThread::solveSlot()
{
    //进行处理的帧是37个字节的，后期要检查帧结构是否符合
    lock.lockForWrite();
    QVector<quint8> buffer_copy=buffer;
    buffer.clear();
    lock.unlock();
    static int cnt=0;
    //检查数据丢失
    static quint8 loss=buffer_copy[6];
    static int flag=0;
    if(buffer_copy[6]!=static_cast<quint8>(loss+1))
    {
        QString str=QString::number(flag);
        emit updateStatusSignal(str+QString(tr(" data loss ")));
        ++flag;
    }
    loss=buffer_copy[6];

    quint8 ch11=buffer_copy[12];
    quint8 ch12=buffer_copy[13];
    quint8 ch13=buffer_copy[14];
    qint32 temp;
    if((ch11 & 0x80) != 0)
    {
        temp = 0xFF;
    }
    else
    {
        temp = 0;
    }
    temp=temp<<8;
    temp=temp|ch11;
    temp=temp<<8;
    temp=temp|ch12;
    temp=temp<<8;
    temp=temp|ch13;
    double temp_1;
    temp_1=temp*factor;
    //allData.push_back(temp_1);

    quint8 ch21=buffer_copy[15];
    quint8 ch22=buffer_copy[16];
    quint8 ch23=buffer_copy[17];
    qint32 temp2;
    if((ch21 & 0x80) != 0)
    {
        temp2 = 0xFF;
    }
    else
    {
        temp2 = 0;
    }
    temp2=temp2<<8;
    temp2=temp2|ch21;
    temp2=temp2<<8;
    temp2=temp2|ch22;
    temp2=temp2<<8;
    temp2=temp2|ch23;
    double temp_2;
    temp_2=temp2*factor;
    //allData2.push_back(temp_2);

    quint8 ch31=buffer_copy[18];
    quint8 ch32=buffer_copy[19];
    quint8 ch33=buffer_copy[20];
    qint32 temp3;
    if((ch31 & 0x80) != 0)
    {
        temp3 = 0xFF;
    }
    else
    {
        temp3 = 0;
    }
    temp3=temp3<<8;
    temp3=temp3|ch31;
    temp3=temp3<<8;
    temp3=temp3|ch32;
    temp3=temp3<<8;
    temp3=temp3|ch33;
    double temp_3;
    temp_3=temp3*factor;
    //allData3.push_back(temp_3);

    quint8 ch41=buffer_copy[21];
    quint8 ch42=buffer_copy[22];
    quint8 ch43=buffer_copy[23];
    qint32 temp4;
    if((ch41 & 0x80) != 0)
    {
        temp4 = 0xFF;
    }
    else
    {
        temp4 = 0;
    }
    temp4=temp4<<8;
    temp4=temp4|ch41;
    temp4=temp4<<8;
    temp4=temp4|ch42;
    temp4=temp4<<8;
    temp4=temp4|ch43;
    double temp_4;
    temp_4=temp4*factor;
    //allData4.push_back(temp_4);

    quint8 ch51=buffer_copy[24];
    quint8 ch52=buffer_copy[25];
    quint8 ch53=buffer_copy[26];
    qint32 temp5;
    if((ch51 & 0x80) != 0)
    {
        temp5 = 0xFF;
    }
    else
    {
        temp5 = 0;
    }
    temp5=temp5<<8;
    temp5=temp5|ch51;
    temp5=temp5<<8;
    temp5=temp5|ch52;
    temp5=temp5<<8;
    temp5=temp5|ch53;
    double temp_5;
    temp_5=temp5*factor;
    //allData5.push_back(temp_5);

    quint8 ch61=buffer_copy[27];
    quint8 ch62=buffer_copy[28];
    quint8 ch63=buffer_copy[29];
    qint32 temp6;
    if((ch61 & 0x80) != 0)
    {
        temp6 = 0xFF;
    }
    else
    {
        temp6 = 0;
    }
    temp6=temp6<<8;
    temp6=temp6|ch61;
    temp6=temp6<<8;
    temp6=temp6|ch62;
    temp6=temp6<<8;
    temp6=temp6|ch63;
    double temp_6;
    temp_6=temp6*factor;
    //allData6.push_back(temp_6);

    quint8 ch71=buffer_copy[30];
    quint8 ch72=buffer_copy[31];
    quint8 ch73=buffer_copy[32];
    qint32 temp7;
    if((ch71 & 0x80) != 0)
    {
        temp7 = 0xFF;
    }
    else
    {
        temp7 = 0;
    }
    temp7=temp7<<8;
    temp7=temp7|ch71;
    temp7=temp7<<8;
    temp7=temp7|ch72;
    temp7=temp7<<8;
    temp7=temp7|ch73;
    double temp_7;
    temp_7=temp7*factor;
    //allData7.push_back(temp_7);

    quint8 ch81=buffer_copy[33];
    quint8 ch82=buffer_copy[34];
    quint8 ch83=buffer_copy[35];
    qint32 temp8;
    if((ch81 & 0x80) != 0)
    {
        temp8 = 0xFF;
    }
    else
    {
        temp8 = 0;
    }
    temp8=temp8<<8;
    temp8=temp8|ch81;
    temp8=temp8<<8;
    temp8=temp8|ch82;
    temp8=temp8<<8;
    temp8=temp8|ch83;
    double temp_8;
    temp_8=temp8*factor;
    //allData8.push_back(temp_8);

    //降低采样率
    if(cnt%8==0)
    {
        emit plotSignal(temp_1,temp_2,temp_3,temp_4,temp_5,temp_6,temp_7,temp_8);
        cnt=0;
    }

}

void dataThread::solveRxASlot()
{

    lock.lockForRead();
    QByteArray cur=RxA.front();
    RxA.pop_front();
    lock.unlock();
    QDataStream out(&cur,QIODevice::ReadWrite); //将字节数组读入
    while(!out.atEnd())
    {
        //每字节填充一次，直到结束
        quint8 outChar = 0;
        out>>outChar;
        //rawbyte.push_back(outChar);

        //每字节填充成一个字符串
        /*QString str = QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0')); //十六进制的转换
        str=str.toUpper();
        rawstring.push_back(str);*/

        static int flag=0;
        //The first time meet 0xA0
        if(flag==0&&outChar==0xA0)
        {
            lock.lockForWrite();
            buffer.push_back(outChar);
            lock.unlock();
            flag=1;
        }
        else if(flag==1)
        {
            lock.lockForWrite();
            buffer.push_back(outChar);
            lock.unlock();
        }
        else
            ;
        if(buffer.size()==37)
        {
            emit solveSignal1();
        }
    }
}

void dataThread::run()
{
    //maybe need to write some code
}
