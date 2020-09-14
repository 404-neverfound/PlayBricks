#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Mythread.h"
#include <QPoint>
#include <QObject>
#include "Bricks.h"
class Mythread;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent=nullptr,bool Full=false,int level=1);
    ~MainWindow();
    bool Full;//记录窗口状态
    int level;//记录关卡
    int score;//记录分数
    QWidget *parent;//记录父窗口指针
    Mythread* ball;//计算小球轨迹和状态的线程
    Bricks Brick;//记录砖块信息
    AudioThread sound;
    void mouseMoveEvent(QMouseEvent *event);//实现鼠标拖动板的移动
    void Pop(int &vx,int &vy);//计算小球与板的碰撞结果
    void LoadMap();//首次加载砖块地图
    void NewMap();//加载新地图
public slots:
    void ball_moving(QPoint);//用于接收ball线程发送的轨迹信号，移动小球位置
    void timer(int times);//接收ball线程发送的时停道具信号，改变道具数量的显示
    void ptime(int t);//接收ball线程发送的穿透道具时间信号，改变道具剩余时间的显示
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
