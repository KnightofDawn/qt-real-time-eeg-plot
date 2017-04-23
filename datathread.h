#ifndef DATATHREAD_H
#define DATATHREAD_H

#include <QThread>
class dataThread :public QThread
{
    Q_OBJECT
public:
    dataThread();
    ~dataThread();
public slots:
    void solveSlot();
    void solveRxASlot();
signals:
    void updateStatusSignal(QString mes);
    void plotSignal(double point,double point2,double point3,double point4,double point5,double point6,double point7,double point8);
    void solveSignal1();
protected:
    void run();
};

#endif

