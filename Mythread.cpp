#include "Mythread.h"
#include <windows.h>
#include <QThread>
#include <QTimer>
#include <audiothread.h>

Mythread::Mythread(){
    Mythread::ini();
    vx=0;
    vy=-speed;
}
void Mythread::speedup(){

    if(speed<10){
        speed++;//通过改变初始速度提高速度
        return;
    }
    if(gap>11){
        gap--;//通过减少刷新间隔提高速度
        return;
    }
}

void Mythread::ini(int x, int y){
    point.rx()=x;
    point.ry()=y;
    vx=0;
    vy=-speed;
}

void Mythread::run(){
    while(isRunning()){
        if(point.x()<0&&vx<0){
            vx=-vx;
        }
        //为了代码可读性没有简化算式
        if(point.x()>1600-40&&vx>0){
            vx=-vx;
        }
        if(point.y()<0){
            vy=-vy;
            if(vy==0)
                vy++;
        }
        if(point.y()>900-40)
            return;
        point.setX(point.x()+vx);
        point.setY(point.y()+vy);
        emit sendPoint(point);
        Sleep(gap);//刷新间隔
    }
}
void Mythread::get_pass(int t){
    bool temp=pass;
    pass=true;
    this->t=t;
    if(temp==false)
        end_pass();
}
void Mythread::end_pass(){
    if(t==0){
        pass=false;
        emit sendptime(0);
        return;
    }
    emit sendptime(t);
    t--;
    QTimer::singleShot(1000,this,SLOT(end_pass()));
}
void Mythread::get_times(int n){
    times+=n;
    sendtimes(times);
}
void Mythread::the_world(){
    if(vy==0)
        return;
    times--;
    sendtimes(times);
    sound.play_times();
    vx=0;
    vy=0;
}

