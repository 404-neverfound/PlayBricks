#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <windows.h>
#include <time.h>
#include "Mythread.h"
#include <QMouseEvent>
#include <QDesktopWidget>
#include "prewindow.h"
static clock_t last_time=clock();
static int board_v=0;
MainWindow::MainWindow(QWidget *parent, bool Full,int level):ui(new Ui::MainWindow)
{
    this->level=level;//初始关卡
    score=0;//初始分数
    this->parent=parent;
    //初始化窗口设置
    setMaximumSize(1600,900);
    setMinimumSize(1600,900);
    if(Full){
        showFullScreen();
        QDesktopWidget *desktop=QApplication::desktop();
        QRect rect=desktop->screenGeometry();
        setGeometry((rect.width()-1600)/2,(rect.height()-900)/2,1600,900);//设置居中
    }
    else{
        showNormal();
        move(140,50);
    }
    ui->setupUi(this);
    ball=new Mythread;
    connect(ball,&Mythread::sendPoint,this,&MainWindow::ball_moving);
    connect(ball,&Mythread::sendptime,this,&MainWindow::ptime);
    connect(ball,&Mythread::sendtimes,this,&MainWindow::timer);
    ui->label_cai->hide();
    ui->pushButton->hide();
    ui->pushButton_2->hide();
    ui->pushButton_4->hide();
    ui->label_pass->hide();
    ui->label_ptime->hide();
    ui->label_timer->hide();
    ui->label_times->hide();
    ui->pushButton->setShortcut(Qt::Key_Escape);
    ui->pushButton_3->setShortcut(Qt::Key_Escape);
    Brick.readmap(level);
    if(Brick.bricks[Brick.Top_Margin][Brick.Left_Margin].label==nullptr)
        LoadMap();
    else
        NewMap();
    if(level==10){
        ball->ini(0,450);
        ball->vy=0;
        ball->vx=15;
        ball->get_pass(3);
    }
    ball->start();
}


MainWindow::~MainWindow()
{
    delete ui;
    ball->terminate();
}

void MainWindow::ball_moving(QPoint point){
    ui->label->move(point);
    score+=Brick.Hit(point.x()+20,point.y()+20,ball);
    ui->label_3->setText(QString::number(score));
    //通关：
    if(Brick.Number==0){
        score=0;
        ball->terminate();
        ui->pushButton_2->show();
        ui->pushButton_2->raise();
        return;
    }
    Pop(ball->vx,ball->vy);//判断小球与板相撞情况
    //失败：
    if(point.y()>900-40){
        ui->label_cai->show();
        ui->label_cai->raise();
        QTimer::singleShot(1000,ui->pushButton,SLOT(show()));
        ui->pushButton->raise();
        QTimer::singleShot(1000,ui->pushButton_4,SLOT(show()));
        ui->pushButton_4->raise();
    }
}
void MainWindow::mouseMoveEvent(QMouseEvent *event){
    clock_t now=clock();
    QPoint point1=event->pos();
    QPoint point2=ui->label_4->pos();
    board_v=(float)(point1.x()-75-point2.x())/(now-last_time);
    last_time=now;
    ui->label_4->move(point1.x()-75,810);
}
//为了代码可读性没有简化算式
void MainWindow::Pop(int &vx,int &vy){
    QPoint point1=ui->label->pos();
    QPoint point2=ui->label_4->pos();
    if(point1.y()+40>point2.y()&&point1.y()<point2.y()){
        if(point1.x()+40<point2.x()||point1.x()>point2.x()+150){//自动使用时停道具
            if(ball->times>0&&point1.y()+30>point2.y()){
                ball->the_world();
            }
        }
        else{
            if(vy==0)
                ball->ini(point1.x(),point1.y());
            if(point1.x()+40>=point2.x()&&point1.x()+20<point2.x()+10)//调整反弹角度
                vx-=3;
            else if(point1.x()+20>=point2.x()+10&&point1.x()+20<point2.x()+30)
                vx-=2;
            else if(point1.x()+20>=point2.x()+30&&point1.x()+20<point2.x()+50)
                vx-=1;
            else if(point1.x()+20>point2.x()+100&&point1.x()+20<=point2.x()+120)
                vx+=1;
            else if(point1.x()+20>point2.x()+120&&point1.x()+20<=point2.x()+140)
                vx+=2;
            else if(point1.x()+20>point2.x()+140&&point1.x()<=point2.x()+150)
                vx+=3;
            vx+=board_v*10;
            if(vx<-20)
                vx=-20;
            else if(vx>20)
                vx=20;
            if(vy>0)
                vy=-vy;
            sound.play_pop();
            ball->speedup();
        }
    }
}

void MainWindow::LoadMap(){
    for(int i=Brick.Top_Margin;i<Brick.Bottom_Margin;i++){
        for(int j=Brick.Left_Margin;j<Brick.Right_Margin;j++){
            QLabel *temp=new QLabel(ui->centralwidget);
            temp->setGeometry(j*40,i*40,40,40);
            if(Brick.bricks[i][j].is_diamond()){
                temp->setPixmap(QPixmap(":/sources/钻石.png"));
                temp->show();
                temp->setScaledContents(true);
            }
            else if(Brick.bricks[i][j].is_stone()){
                int num=rand()%5+1;
                temp->setPixmap(QPixmap(":/sources/石头"+QString::number(num)+".png"));//随机挑选一种外观的石头
                temp->setScaledContents(true);
            }
            else if(Brick.bricks[i][j].is_bomb()){
                temp->setPixmap(QPixmap(":/sources/炸弹.png"));
                temp->setScaledContents(true);
            }
            else if(Brick.bricks[i][j].is_pass()){
                temp->setPixmap(QPixmap(":/sources/穿透.png"));
                temp->setScaledContents(true);
            }
            else if(Brick.bricks[i][j].is_timer()){
                temp->setPixmap(QPixmap(":/sources/时停.png"));
                temp->setScaledContents(true);
            }
            Brick.bricks[i][j].label=temp;
        }
    }
}

void MainWindow::NewMap(){
    QLabel* temp;
    for(int i=Brick.Top_Margin;i<Brick.Bottom_Margin;i++){
        for(int j=Brick.Left_Margin;j<Brick.Right_Margin;j++){
            temp=Brick.bricks[i][j].label;
            temp->clear();
            temp->setGeometry(j*40,i*40,40,40);
            temp->show();
            if(Brick.bricks[i][j].is_diamond()){
                temp->setPixmap(QPixmap(":/sources/钻石.png"));
                temp->show();
                temp->setScaledContents(true);
            }
            else if(Brick.bricks[i][j].is_stone()){
                int num=rand()%5+1;
                temp->setPixmap(QPixmap(":/sources/石头"+QString::number(num)+".png"));//随机挑选一种外观的石头
                temp->setScaledContents(true);
            }
            else if(Brick.bricks[i][j].is_bomb()){
                temp->setPixmap(QPixmap(":/sources/炸弹.png"));
                temp->setScaledContents(true);
            }
            else if(Brick.bricks[i][j].is_pass()){
                temp->setPixmap(QPixmap(":/sources/穿透.png"));
                temp->setScaledContents(true);
                Brick.bricks[i][j].label=temp;
            }
            else if(Brick.bricks[i][j].is_timer()){
                temp->setPixmap(QPixmap(":/sources/时停.png"));
                temp->setScaledContents(true);
            }
        }
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    Brick.readmap(++level);
    NewMap();
    ball->ini(ui->label_4->x()+55,ui->label_4->y()-40);
    if(level==10){
        ball->ini(0,450);
        ball->vy=0;
        ball->vx=20;
        ball->get_pass(5);
    }
    ball->speed=5;
    ball->gap=30;
    ball->start();
    ui->pushButton_2->hide();
}

void MainWindow::timer(int times){
    if(times<=0){
        ui->label_timer->hide();
        ui->label_times->hide();
        return;
    }
    ui->label_timer->show();
    ui->label_timer->raise();
    ui->label_times->show();
    ui->label_times->raise();
    ui->label_times->setNum(times);
}

void MainWindow::ptime(int t){
    if(t==0){
        ui->label_pass->hide();
        ui->label_ptime->hide();
        return;
    }
    ui->label_pass->show();
    ui->label_pass->raise();
    ui->label_ptime->show();
    ui->label_ptime->raise();
    ui->label_ptime->setNum(t);
}

void MainWindow::on_pushButton_clicked()
{
    parent->show();
    ball->terminate();
    delete ball;
    this->close();
}
void MainWindow::on_pushButton_3_clicked()
{
    parent->show();
    ball->terminate();
    delete ball;
    this->close();
}

void MainWindow::on_pushButton_4_clicked()
{
    score=0;
    Brick.readmap(level);
    NewMap();
    ball->speed=5;
    ball->gap=30;
    ball->ini(ui->label_4->x()+55,ui->label_4->y()-40);
    ball->start();
    ui->label_cai->hide();
    ui->pushButton->hide();
    ui->pushButton_4->hide();
}
