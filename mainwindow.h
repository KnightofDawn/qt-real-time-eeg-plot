#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qcustomplot.h"
#include <QMainWindow>
#include <QString>
#include <QLabel>
#include <QTimer>

class serialThread;
class dataThread;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void onTimerOut();
    ~MainWindow();

    //设置qcustomplot画图属性，实时
    void setupRealtimeDataDemo(QCustomPlot *customPlot);
    void updateStatus(QString mes);
    void modalmessage(QString mes);

private slots:
    void on_sendButton_clicked();

    void on_closeButton_clicked();

    void on_openButton_clicked();

    void realtimeDataSlot(double point,double point2,double point3,double point4,double point5,double point6,double point7,double point8); //添加实时数据槽

    void on_pushButton_clicked();

    void on_pushButtona_clicked();

    void on_pushButtonb_clicked();

    void on_pushButtonc_clicked();

    void on_pushButtond_clicked();

    void on_fastButton_clicked();

    void on_slowButton_clicked();

    void on_resetButton_clicked();

    void on_saveButton_clicked();

    void on_resetButton2_clicked();

    void on_slowButton2_clicked();

    void on_fastbutton2_clicked();

    void on_resetButton3_clicked();

    void on_slowButton3_clicked();

    void on_fastbutton3_clicked();

    void on_resetButton4_clicked();

    void on_slowButton4_clicked();

    void on_fastbutton4_clicked();

    void on_resetButton5_clicked();

    void on_slowButton5_clicked();

    void on_fastbutton5_clicked();

    void on_resetButton6_clicked();

    void on_slowButton6_clicked();

    void on_fastbutton6_clicked();

    void on_resetButton7_clicked();

    void on_slowButton7_clicked();

    void on_fastbutton7_clicked();

    void on_resetButton8_clicked();

    void on_slowButton8_clicked();

    void on_fastbutton8_clicked();

    void on_pauseButton_clicked();

    void on_rescaleButton_clicked();

private:
    Ui::MainWindow *ui;
    QLabel* tip; //状态栏
    QLabel* rightTip; //状态栏
    QTimer dataTimer; //定时器，周期调用realtimeDataSlot()槽，实现动态数据添加到曲线
    serialThread* thread1;
    dataThread* thread2;
};

#endif // MAINWINDOW_H
