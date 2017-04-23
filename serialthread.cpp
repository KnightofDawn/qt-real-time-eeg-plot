#include "serialthread.h"
#include "datathread.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QThread>
#include <QDataStream>
#include <QByteArray>
#include <QVector>
#include <cmath>
#include <QReadWriteLock>

using namespace std;

class MainWindow;

extern QVector<QString> rawstring;
extern QVector<quint8> rawbyte;
extern QByteArray rawdata;
extern QVector<QByteArray> RxA;
extern QReadWriteLock lock;

//测试
extern QVector<quint8> buffer;

serialThread::serialThread()
{
    serial=new QSerialPort;
    stopped=false;
    connect(serial, &QSerialPort::readyRead,this,&serialThread::readData);
}

//setup serialport
void serialThread::setCom(QString s)
{
    com=s;

    serial->clear();
    serial->setPortName(com);
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
}

//open serialport
void serialThread::openCom()
{
    bool flag=serial->open(QIODevice::ReadWrite);

    if(flag)
    {
        QString mes(tr("串口打开成功"));
        emit updateStatusSignal(mes);
    }
    else
    {
        QString mes=serial->errorString();
        emit updateStatusSignal(mes);
    }
}

//close serialport
void serialThread::closeCom()
{
    if(serial->isOpen())
    {
        serial->close();
        QString mes(tr("串口关闭成功"));
        emit updateStatusSignal(mes);
    }
    else
    {
        QString mes=serial->errorString();
        emit updateStatusSignal(mes);
    }
}

//send data to serialport
void serialThread::sendData(QString str)
{
    const QByteArray text = str.toLocal8Bit();
    serial->write(text);
}

//read data from serialport
void serialThread::readData()
{
    //读取串口缓冲区的数据
    QByteArray Rx;
    Rx=serial->readAll();

    /*QDataStream out(&Rx,QIODevice::ReadWrite); //将字节数组读入
    while(!out.atEnd())
    {
        //每字节填充一次，直到结束
        quint8 outChar = 0;
        out>>outChar;
        rawbyte.push_back(outChar);

        //每字节填充成一个字符串
        /*QString str = QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0')); //十六进制的转换
        str=str.toUpper();
        rawstring.push_back(str);*/

        /*static int flag=0;
        //The first time meet 0xA0
        if(flag==0&&outChar==0xA0)
        {
            buffer.push_back(outChar);
            flag=1;
        }
        else if(flag==1)
        {
            buffer.push_back(outChar);
        }
        else
            ;
        if(buffer.size()==37)
            emit solveSignal();
    }*/
    //rawdata+=Rx;
    lock.lockForWrite();
    RxA.append(Rx);
    lock.unlock();
    emit solveRxASignal();
}

void serialThread::run()
{
    //maybe need to write some code
}

serialThread::~serialThread()
{
    stopped=true;
    delete serial;
}


