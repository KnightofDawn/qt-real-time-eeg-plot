#ifndef SERIALTHREAD_H
#define SERIALTHREAD_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QThread>

class serialThread :public QThread
{
    Q_OBJECT
public:
    serialThread();
    void setCom(QString s);
    void openCom();
    void closeCom();
    void sendData(QString str);
    void readData();
    ~serialThread();
private:
    QSerialPort* serial;
    QString com;
    bool stopped;
protected:
    void run();
signals:
    void updateStatusSignal(QString mes);
    void sendMessageSignal(QString mes);
    void solveSignal();
    void solveRxASignal();
};


#endif // SERIALTHREAD_H
