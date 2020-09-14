#ifndef BRICKS_H
#define BRICKS_H
#include <QLabel>
#include "audiothread.h"
#include "Mythread.h"

class Brick{
public:
    Brick();//默认构造函数
    Brick(int x,int y);//按中心位置构造
    int CenterX,CenterY;//记录中心位置
    enum Brick_Type{empty=0,stone=1,diamond=2,bomb=3,pass=4,timer=5};//对砖块类型分类
    int type;//记录砖块类型
    QLabel *label=nullptr;//记录砖块控件指针
    //判断砖块类型
    bool is_diamond();
    bool is_stone();
    bool is_bomb();
    bool is_pass();
    bool is_timer();
    bool is_hit(int x,int y,Mythread *ball);//判断小球是否与砖块相撞
    int destroy(Brick b[20][40],int i,int j,int Left_Margin,int Top_Margin,int Right_Margin,int Bottom_Margin,int &Number,Mythread *ball);//破坏砖块
};
class Bricks{
public:
    Bricks();
    Brick bricks[20][40];//砖块数组
    int Number;//记录砖块数量
    //播放音频的次序
    int seq_stone=0;
    int seq_diamond=0;
    int seq_bomb=0;
    //音频线程
    AudioThread sound_stone[10];
    AudioThread sound_diamond[5];
    AudioThread sound_bomb[5];
    void random();//随机生成砖块
    void readmap(int n);//从对应文件中读取砖块
    int Hit(int x,int y,Mythread *ball);//判断小球是否与砖块相撞
    //记录砖块布局的四个边缘
    int Left_Margin;
    int Right_Margin;
    int Top_Margin;
    int Bottom_Margin;
};

#endif // BRICKS_H
