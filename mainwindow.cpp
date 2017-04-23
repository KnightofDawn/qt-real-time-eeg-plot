#include "ui_mainwindow.h"
#include "mainwindow.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QMessageBox>
#include "serialthread.h"
#include "datathread.h"
#include "qcustomplot.h"
#include <QByteArray>
#include <QPalette>
#include <QVector>
#include <QQueue>
#include <string>
#include <fstream>
#include <QTimer>
#include <QLabel>
#include <QTime>
#include <QIcon>
#include <cmath>
#include <QPainter>
#include <QReadWriteLock>
#include <QtOpenGL/QtOpenGL>
#include <QtGui>

#define TIMESCALE 25

//全局变量
QByteArray rawdata; //原始字节流
QVector<QString> rawstring; //每个字节保存成字符串
QVector<quint8> rawbyte; //每个字节保存成unsigned int
QVector<quint8> buffer; //帧结构缓冲区
QVector<QByteArray> RxA;
QReadWriteLock lock;

QQueue<double> allData;
QQueue<double> allData2;
QQueue<double> allData3;
QQueue<double> allData4;
QQueue<double> allData5;
QQueue<double> allData6;
QQueue<double> allData7;
QQueue<double> allData8;

std::ofstream out;

int timescale=TIMESCALE; //时间尺度默认为TIMESCALE
int lasttimescale=timescale; //暂停之前的时间尺度

int timescale2=TIMESCALE;
int lasttimescale2=timescale2;

int timescale3=TIMESCALE;
int lasttimescale3=timescale3;

int timescale4=TIMESCALE;
int lasttimescale4=timescale4;

int timescale5=TIMESCALE;
int lasttimescale5=timescale5;

int timescale6=TIMESCALE;
int lasttimescale6=timescale6;

int timescale7=TIMESCALE;
int lasttimescale7=timescale7;

int timescale8=TIMESCALE;
int lasttimescale8=timescale8;

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->sendtextEdit->hide();
    ui->sendButton->hide();
    ui->pushButtonc->hide();
    ui->pushButtond->hide();
    ui->pushButton->hide();

    ui->closeButton->setEnabled(false);
    ui->saveButton->setEnabled(false);

    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/new/prefix1/backgrounduais")));
    this->setPalette(palette);
    this->setAutoFillBackground(true);

    //ui->customPlot->setOpenGl(true);
    if(ui->customPlot->openGl()==true)
        QMessageBox::information(this, tr("Information"), QString(tr("通道1 opengl设置成功!")));
    setupRealtimeDataDemo(ui->customPlot);
    ui->customPlot->replot();
    setupRealtimeDataDemo(ui->customPlot2);
    ui->customPlot2->replot();
    setupRealtimeDataDemo(ui->customPlot3);
    ui->customPlot3->replot();
    setupRealtimeDataDemo(ui->customPlot4);
    ui->customPlot4->replot();
    setupRealtimeDataDemo(ui->customPlot5);
    ui->customPlot5->replot();
    setupRealtimeDataDemo(ui->customPlot6);
    ui->customPlot6->replot();
    setupRealtimeDataDemo(ui->customPlot7);
    ui->customPlot7->replot();
    setupRealtimeDataDemo(ui->customPlot8);
    ui->customPlot8->replot();

    setWindowIcon(QIcon(":/new/prefix1/icon"));
    setWindowTitle(tr("8 leads eeg wave by lovesunmoonlight"));

    //枚举串口号
    QList<QSerialPortInfo> serilList=QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo& r:serilList)
    {
        ui->comboBox->addItem(r.portName());
    }

    //左侧状态栏
    tip=new QLabel;
    tip->setFont(QFont("Times",15));
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::white);
    tip->setPalette(pe);
    statusBar()->addWidget(tip);

    //右侧状态栏
    rightTip=new QLabel;
    rightTip->setFrameStyle(QFrame::Box | QFrame::Sunken);
    rightTip->setText(
      tr("<a href=\"http://uais.lzu.edu.cn\">uais.lzu.edu.cn</a>"));
    rightTip->setTextFormat(Qt::RichText);
    rightTip->setOpenExternalLinks(true);
    statusBar()->addPermanentWidget(rightTip);



    thread1=new serialThread; //pointer to sub thread
    thread2=new dataThread; //pointer to sub thread

    //下面这两句是否应该写在子线程里有待商榷
    connect(thread1,&serialThread::updateStatusSignal,this,&MainWindow::updateStatus);
    connect(thread2,&dataThread::updateStatusSignal,this,&MainWindow::updateStatus);

    //子进程间通信
    //connect(thread1,&serialThread::solveSignal,thread2,&dataThread::solveSlot);
    connect(thread1,&serialThread::solveRxASignal,thread2,&dataThread::solveRxASlot);
    connect(thread2,&dataThread::solveSignal1,thread2,&dataThread::solveSlot);

    //绘制信号
    connect(thread2,&dataThread::plotSignal,this,&MainWindow::realtimeDataSlot);

    //调试用
    connect(thread1,&serialThread::sendMessageSignal,this,&MainWindow::modalmessage);

}

MainWindow::~MainWindow()
{
    thread1->closeCom();
    delete thread1;
    delete thread2;
    delete ui;
}

//click "send"
void MainWindow::on_sendButton_clicked()
{
    QString str=ui->sendtextEdit->toPlainText();
    thread1->sendData(str);
}

//click "close"
void MainWindow::on_closeButton_clicked()
{
    thread1->closeCom();
    ui->closeButton->setEnabled(false);
    ui->openButton->setEnabled(true);

    //原始字节流
    out.open("rawdata.txt");
    QByteArray temp=rawdata.toHex().toUpper();
    for(int i=0;i<temp.size();i++)
        out<<temp[i];
    out.close();

    //每个字节转换成一个字符串
    out.open("rawstring.txt");
    for(int i=0;i<rawstring.size();i++)
    {
        QString tempp=rawstring[i];
        string temp1=tempp.toStdString();
        out<<temp1<<" ";
    }
    out.close();

    //每个字节转换成一个unsigned char
    out.open("rawbyte.txt");
    for(int i=0;i<rawbyte.size();i++)
        out<<(int)rawbyte[i]<<" ";
    out.close();

    //波形数据
    out.open("allData.txt");
    for(int i=0;i<allData.size();i++)
        out<<allData[i]<<" ";
    out.close();
}

//click "open"
void MainWindow::on_openButton_clicked()
{
    ui->openButton->setEnabled(false);
    ui->closeButton->setEnabled(true);
    QString com=ui->comboBox->currentText();
    thread1->setCom(com);
    thread1->openCom();
    thread1->start();
}

//绘图初始化
void MainWindow::setupRealtimeDataDemo(QCustomPlot *customPlot)
{
  //demoName = "Real Time Data Demo";

  // include this section to fully disable antialiasing for higher performance:
  // 下面的一部分代码用于禁止抗锯齿
  /*customPlot->setNotAntialiasedElements(QCP::aeAll);
  QFont font;
  font.setStyleStrategy(QFont::NoAntialias);
  customPlot->xAxis->setTickLabelFont(font);
  customPlot->yAxis->setTickLabelFont(font);
  customPlot->legend->setFont(font);*/

  //opengl加速
  /*customPlot->setOpenGl(true);
  if(customPlot->openGl()==true)
  {
      QMessageBox::information(this, tr("Information"), tr("Opengl acceleration ok"));
  }*/

  customPlot->addGraph(); // blue line
  customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));

  QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
  timeTicker->setTimeFormat("%h:%m:%s");
  //customPlot->xAxis->setLabel("time");

  if(customPlot==ui->customPlot)
      customPlot->yAxis->setLabel("l1");
  else if(customPlot==ui->customPlot2)
      customPlot->yAxis->setLabel("l2");
  else if(customPlot==ui->customPlot3)
      customPlot->yAxis->setLabel("l3");
  else if(customPlot==ui->customPlot4)
      customPlot->yAxis->setLabel("l4");
  else if(customPlot==ui->customPlot5)
      customPlot->yAxis->setLabel("l5");
  else if(customPlot==ui->customPlot6)
      customPlot->yAxis->setLabel("l6");
  else if(customPlot==ui->customPlot7)
      customPlot->yAxis->setLabel("l7");
  else
      customPlot->yAxis->setLabel("l8");

  customPlot->xAxis->setTicker(timeTicker);
  customPlot->axisRect()->setupFullAxesBox();
  customPlot->yAxis->setRange(2300, 2500);
  customPlot->setInteraction(QCP::iRangeDrag, true); //允许拖拽
  customPlot->setInteraction(QCP::iRangeZoom, true); //允许放缩
  customPlot->axisRect()->setRangeZoom(Qt::Vertical); //只允许竖直方向的放缩

  //这两句没懂啥意思
  // make left and bottom axes transfer their ranges to right and top axes:
  //connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
  //connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

  // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
  //connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
  //dataTimer.start(4); // Interval 0 means to refresh as fast as possible
}

/*2017.4.17 add
  坐标轴改为手动适应
  放缩可以一直进行
*/

void MainWindow::realtimeDataSlot(double point,double point2,double point3,double point4,double point5,double point6,double point7,double point8)
{
    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    //static double lastPointKey = 0;
    //static int pos=0;
    //if (key-lastPointKey > 0.01) // at most add point every 2 ms
    //{
      // add data to lines:
        /*if(!allData.empty())
        {

            ui->customPlot->graph(0)->addData(key,allData.front());
            allData.pop_front();

        }*/

      //ui->customPlot->graph(0)->addData(key, qSin(key)+qrand()/(double)RAND_MAX*1*qSin(key/0.3843));
      //ui->customPlot->graph(1)->addData(key, qCos(key)+qrand()/(double)RAND_MAX*0.5*qSin(key/0.4364));
      // rescale value (vertical) axis to fit the current data:
      //ui->customPlot->rescaleAxes();
      //lastPointKey = key;
    //}
    // make key axis range scroll with the data (at a constant range size of 8):

    ui->customPlot->graph(0)->addData(key,point);
    //ui->customPlot->rescaleAxes();
    ui->customPlot->xAxis->setRange(key, timescale, Qt::AlignRight);
    ui->customPlot->replot();

    ui->customPlot2->graph(0)->addData(key,point2);
    //ui->customPlot2->rescaleAxes();
    ui->customPlot2->xAxis->setRange(key,timescale2,Qt::AlignRight);
    ui->customPlot2->replot();

    ui->customPlot3->graph(0)->addData(key,point3);
    //ui->customPlot3->rescaleAxes();
    ui->customPlot3->xAxis->setRange(key,timescale3,Qt::AlignRight);
    ui->customPlot3->replot();

    ui->customPlot4->graph(0)->addData(key,point4);
    //ui->customPlot4->rescaleAxes();
    ui->customPlot4->xAxis->setRange(key,timescale4,Qt::AlignRight);
    ui->customPlot4->replot();

    ui->customPlot5->graph(0)->addData(key,point5);
    //ui->customPlot5->rescaleAxes();
    ui->customPlot5->xAxis->setRange(key,timescale5,Qt::AlignRight);
    ui->customPlot5->replot();

    ui->customPlot6->graph(0)->addData(key,point6);
    //ui->customPlot6->rescaleAxes();
    ui->customPlot6->xAxis->setRange(key,timescale6,Qt::AlignRight);
    ui->customPlot6->replot();

    ui->customPlot7->graph(0)->addData(key,point7);
    //ui->customPlot7->rescaleAxes();
    ui->customPlot7->xAxis->setRange(key,timescale7,Qt::AlignRight);
    ui->customPlot7->replot();

    ui->customPlot8->graph(0)->addData(key,point8);
    //ui->customPlot8->rescaleAxes();
    ui->customPlot8->xAxis->setRange(key,timescale8,Qt::AlignRight);
    ui->customPlot8->replot();

    /*if(!allData2.empty())
    {
        ui->customPlot2->graph(0)->addData(key,allData2.front());
        allData2.pop_front();
    }

    ui->customPlot2->xAxis->setRange(key, timescale2, Qt::AlignRight);
    //ui->customPlot2->rescaleAxes();
    ui->customPlot2->replot();

    if(!allData3.empty())
    {
        ui->customPlot3->graph(0)->addData(key,allData3.front());
        allData3.pop_front();
    }

    ui->customPlot3->xAxis->setRange(key, timescale3, Qt::AlignRight);
    //ui->customPlot3->rescaleAxes();
    ui->customPlot3->replot();

    if(!allData4.empty())
    {
        ui->customPlot4->graph(0)->addData(key,allData4.front());
        allData4.pop_front();
    }

    ui->customPlot4->xAxis->setRange(key, timescale4, Qt::AlignRight);
    ui->customPlot4->rescaleAxes();
    ui->customPlot4->replot();

    if(!allData5.empty())
    {
        ui->customPlot5->graph(0)->addData(key,allData5.front());
        allData5.pop_front();
    }

    ui->customPlot5->xAxis->setRange(key, timescale5, Qt::AlignRight);
    ui->customPlot5->rescaleAxes();
    ui->customPlot5->replot();

    if(!allData6.empty())
    {
        ui->customPlot6->graph(0)->addData(key,allData6.front());
        allData6.pop_front();
    }

    ui->customPlot6->xAxis->setRange(key, timescale6, Qt::AlignRight);
    ui->customPlot6->rescaleAxes();
    ui->customPlot6->replot();

    if(!allData7.empty())
    {
        ui->customPlot7->graph(0)->addData(key,allData7.front());
        allData7.pop_front();
    }

    ui->customPlot7->xAxis->setRange(key, timescale7, Qt::AlignRight);
    ui->customPlot7->rescaleAxes();
    ui->customPlot7->replot();

    if(!allData8.empty())
    {
        ui->customPlot8->graph(0)->addData(key,allData8.front());
        allData8.pop_front();
    }

    ui->customPlot8->xAxis->setRange(key, timescale8, Qt::AlignRight);
    ui->customPlot8->rescaleAxes();
    ui->customPlot8->replot();*/

    // calculate frames per second:
    //static double lastFpsKey;
    //static int frameCount;
    //++frameCount;
    //if (key-lastFpsKey > 2) // average fps over 2 seconds
    //{
      //ui->statusBar->showMessage(
            //QString("%1 FPS, Total Data points: %2")
            //.arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
            //.arg(ui->customPlot->graph(0)->data()->size()+ui->customPlot->graph(1)->data()->size())
            //, 0);
      //lastFpsKey = key;
      //frameCount = 0;
    //}


}

//change background
bool bkflag=false;
void MainWindow::on_pushButton_clicked()
{
    if(!bkflag)
    {
        QPalette palette;
        palette.setBrush(QPalette::Background, QBrush(QPixmap(":/new/prefix1/background")));
        this->setPalette(palette);
        this->setAutoFillBackground(true);
        bkflag=true;
    }
    else
    {
        QPalette palette;
        palette.setBrush(QPalette::Background, QBrush(QPixmap(":/new/prefix1/background1")));
        this->setPalette(palette);
        this->setAutoFillBackground(true);
        bkflag=false;
    }
}

//update statusbar
void MainWindow::updateStatus(QString mes)
{
    statusBar()->clearMessage();
    tip->setText(mes);
    statusBar()->update();
}

void MainWindow::on_pushButtona_clicked()
{
    thread1->sendData(QString(tr("uaisa")));
}

void MainWindow::on_pushButtonb_clicked()
{
    thread1->sendData(QString(tr("uaisb")));
}

void MainWindow::on_pushButtonc_clicked()
{
    thread1->sendData(QString(tr("uaisc")));
}

void MainWindow::on_pushButtond_clicked()
{
    thread1->sendData(QString(tr("uaisd")));
}

void MainWindow::modalmessage(QString mes)
{
    QMessageBox::information(this, tr("Receive"), mes);
    disconnect(thread1,&serialThread::sendMessageSignal,this,&MainWindow::modalmessage);
}

void MainWindow::on_saveButton_clicked()
{
    QTime ct=QTime::currentTime();
    QString str=ct.toString("hh:mm:s").replace(QRegExp(":"),"-");
    QString name,name2,name3,name4,name5,name6,name7,name8;
    name=QString(tr("1lead"))+str+QString(tr("shortcut.pdf"));
    name2=QString(tr("2lead"))+str+QString(tr("shortcut.pdf"));
    name3=QString(tr("3lead"))+str+QString(tr("shortcut.pdf"));
    name4=QString(tr("4lead"))+str+QString(tr("shortcut.pdf"));
    name5=QString(tr("5lead"))+str+QString(tr("shortcut.pdf"));
    name6=QString(tr("6lead"))+str+QString(tr("shortcut.pdf"));
    name7=QString(tr("7lead"))+str+QString(tr("shortcut.pdf"));
    name8=QString(tr("8lead"))+str+QString(tr("shortcut.pdf"));
    ui->customPlot->savePdf(name);
    ui->customPlot2->savePdf(name2);
    ui->customPlot3->savePdf(name3);
    ui->customPlot4->savePdf(name4);
    ui->customPlot5->savePdf(name5);
    ui->customPlot6->savePdf(name6);
    ui->customPlot7->savePdf(name7);
    ui->customPlot8->savePdf(name8);
    ui->saveButton->setEnabled(false);
    QMessageBox::information(this, tr("Informatio"), QString(tr("pdf保存成功!")));
}

void MainWindow::on_resetButton_clicked()
{
    timescale=TIMESCALE;
}

void MainWindow::on_fastButton_clicked()
{
    timescale-=1;
}

void MainWindow::on_slowButton_clicked()
{
    timescale+=1;
}

void MainWindow::on_resetButton2_clicked()
{
    timescale2=TIMESCALE;
}

void MainWindow::on_slowButton2_clicked()
{
    timescale2+=1;
}

void MainWindow::on_fastbutton2_clicked()
{
    timescale2-=1;
}

void MainWindow::on_resetButton3_clicked()
{
    timescale3=TIMESCALE;
}

void MainWindow::on_slowButton3_clicked()
{
    timescale3+=1;
}

void MainWindow::on_fastbutton3_clicked()
{
    timescale3-=1;
}

void MainWindow::on_resetButton4_clicked()
{
    timescale4=TIMESCALE;
}

void MainWindow::on_slowButton4_clicked()
{
    timescale4+=1;
}

void MainWindow::on_fastbutton4_clicked()
{
    timescale4-=1;
}

void MainWindow::on_resetButton5_clicked()
{
    timescale5=TIMESCALE;
}

void MainWindow::on_slowButton5_clicked()
{
    timescale5+=1;
}

void MainWindow::on_fastbutton5_clicked()
{
    timescale5-=1;
}

void MainWindow::on_resetButton6_clicked()
{
    timescale6=TIMESCALE;
}

void MainWindow::on_slowButton6_clicked()
{
    timescale6+=1;
}

void MainWindow::on_fastbutton6_clicked()
{
    timescale6-=1;
}

void MainWindow::on_resetButton7_clicked()
{
    timescale7=TIMESCALE;
}

void MainWindow::on_slowButton7_clicked()
{
    timescale7+=1;
}

void MainWindow::on_fastbutton7_clicked()
{
    timescale7-=1;
}

void MainWindow::on_resetButton8_clicked()
{
    timescale8=TIMESCALE;
}

void MainWindow::on_slowButton8_clicked()
{
    timescale8+=1;
}

void MainWindow::on_fastbutton8_clicked()
{
    timescale8-=1;
}

void MainWindow::on_pauseButton_clicked()
{
    static int flag=0;
    if(!flag)
    {
        ui->saveButton->setEnabled(true);
        disconnect(thread2,&dataThread::plotSignal,this,&MainWindow::realtimeDataSlot);
        ui->pauseButton->setText("continue");
        flag=1;
    }
    else
    {
        ui->saveButton->setEnabled(false);
        connect(thread2,&dataThread::plotSignal,this,&MainWindow::realtimeDataSlot);
        ui->pauseButton->setText("pause");
        flag=0;
    }
}

void MainWindow::on_rescaleButton_clicked()
{
    ui->customPlot->rescaleAxes();
    ui->customPlot2->rescaleAxes();
    ui->customPlot3->rescaleAxes();
    ui->customPlot4->rescaleAxes();
    ui->customPlot5->rescaleAxes();
    ui->customPlot6->rescaleAxes();
    ui->customPlot7->rescaleAxes();
    ui->customPlot8->rescaleAxes();
}
