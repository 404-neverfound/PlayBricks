#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QThread>
#include <QPoint>
#include <QObject>
#include "audiothread.h"

class Mythread:public QThread{
    Q_OBJECT
public:
     Mythread();
     QPoint point;//记录小球位置点
     int vx,vy;//记录水平和竖直方向的速度
     int speed=5;//记录默认初始速度
     int times=0;//记录时停道具数量
     int t=0;//记录穿透剩余时间
     int gap=30;//线程刷新间隔
     bool pass=false;//记录球的穿透状态
     AudioThread sound;
     void speedup();//提升默认初始速度
     void ini(int x=780,int y=770);//初始化球的位置
     void the_world();//启动时停
     void get_pass(int t);//获得穿透道具
     void get_times(int n);//获得时停道具
public slots:
     void end_pass();//计算穿透道具剩余时间
signals:
    void sendPoint(QPoint);//向MainWindow发送小球位置信息
    void sendtimes(int t);//向MainWindow发送时停道具数量信息
    void sendptime(int n);//向MainWindow发送穿透剩余时间信息
protected:
    void run();//计算小球轨迹
};


#endif // MYTHREAD_H

