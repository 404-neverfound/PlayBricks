#include "Bricks.h"
#include <math.h>
#include <random>
#include "audiothread.h"


Brick::Brick(){}
Brick::Brick(int x,int y){
    CenterX=x;
    CenterY=y;
    type=empty;//默认类型为空
}
bool Brick::is_hit(int x,int y,Mythread *ball){
    if(type==empty)
        return false;
    //计算小球与砖块的中心距离
    int disx=x-CenterX;
    int disy=y-CenterY;
    if(disx<0)
        disx=-disx;
    if(disy<0)
        disy=-disy;
    if(disx<=35&&disy<=35){
        if(!ball->pass&&type!=pass){
            if(disx>=disy)
                ball->vx=-ball->vx;
            if(disy>=disx)
                ball->vy=-ball->vy;
        }
        return true;//相撞返回真
    }
    return false;//未相撞返回假
}
bool Brick::is_diamond(){
    return type==diamond;
}
bool Brick::is_stone(){
    return type==stone;
}
bool Brick::is_bomb(){
    return type==bomb;
}
bool Brick::is_pass(){
    return type==pass;
}
bool Brick::is_timer(){
    return type==timer;
}

int Brick::destroy(Brick b[20][40],int i,int j,int Left_Margin,int Top_Margin,int Right_Margin,int Bottom_Margin,int &Number,Mythread *ball){
    if(type==empty)
        return 0;
    Number--;
    int score=0;
    if(type==diamond)
        score=10;
    else if(type==stone)
        score=1;
    else if(type==pass)
        emit ball->get_pass(5);
    else if(type==timer)
        emit ball->get_times(3);
    if(type==bomb){
        score=1;
        type=empty;
        label->setGeometry(CenterX-60,CenterY-60,120,120);
        label->setPixmap(QPixmap(":/sources/爆炸.png"));
        label->setScaledContents(true);
        if(j>Left_Margin){
            score+=b[i][j-1].destroy(b,i,j-1,Left_Margin,Top_Margin,Right_Margin,Bottom_Margin,Number,ball);
        }
        if(j<Right_Margin-1){
            score+=b[i][j+1].destroy(b,i,j+1,Left_Margin,Top_Margin,Right_Margin,Bottom_Margin,Number,ball);
        }
        if(i>Top_Margin){
            score+=b[i-1][j].destroy(b,i-1,j,Left_Margin,Top_Margin,Right_Margin,Bottom_Margin,Number,ball);
            if(j>Left_Margin){
                score+=b[i-1][j-1].destroy(b,i-1,j-1,Left_Margin,Top_Margin,Right_Margin,Bottom_Margin,Number,ball);
            }
            if(j<Right_Margin-1){
                score+=b[i-1][j+1].destroy(b,i-1,j+1,Left_Margin,Top_Margin,Right_Margin,Bottom_Margin,Number,ball);
            }
        }
        if(i<Bottom_Margin-1){
            score+=b[i+1][j].destroy(b,i+1,j,Left_Margin,Top_Margin,Right_Margin,Bottom_Margin,Number,ball);
            if(j>Left_Margin){
                score+=b[i+1][j-1].destroy(b,i+1,j-1,Left_Margin,Top_Margin,Right_Margin,Bottom_Margin,Number,ball);
            }
            if(j<Right_Margin-1){
                score+=b[i+1][j+1].destroy(b,i+1,j+1,Left_Margin,Top_Margin,Right_Margin,Bottom_Margin,Number,ball);
            }
        }
        QTimer::singleShot(800,label,SLOT(hide()));
        return score;
    }
    type=empty;
    label->setVisible(false);
    return score;
}

Bricks::Bricks(){
    Left_Margin=1;
    Right_Margin=39;
    Top_Margin=1;
    Bottom_Margin=15;
    Number =0;
    for(int i=Top_Margin;i<Bottom_Margin;i++){
        for(int j=Left_Margin;j<Right_Margin;j++){
            bricks[i][j]=Brick(j*40+20,i*40+20);
        }
    }
}
void Bricks::random(){
    srand((unsigned)time(NULL));
    Number=0;
    for(int i=Top_Margin;i<Bottom_Margin;i++){
        for(int j=Left_Margin;j<Right_Margin;j++){
            int chance=rand()%100;
            if(chance<80){
                bricks[i][j].type=stone;
                Number++;
            }
            else if(chance<85){
                bricks[i][j].type=diamond;
                Number++;
            }
            else if(chance<90){
                bricks[i][j].type=bomb;
                Number++;
            }
            else if(chance<91){
                bricks[i][j].type=pass;
                Number++;
            }
            else if(chance<92){
                bricks[i][j].type=timer;
                Number++;
            }
            else bricks[i][j].type=empty;
        }
    }
}
void Bricks::readmap(int n){
    QString name=":/sources/关卡"+QString::number(n)+".txt";
    char str[64];
    Number=0;
    QFile file(name);
    file.open(QFile::ReadOnly);
    if(!file.isOpen()){
        random();
        return;
    }
    int i=Top_Margin;
    int j=Left_Margin;
    int length=0,p=0;
    while((length=file.read(str,64))!=0){
        for(p=0;p<length;p++){
            if(str[p]<'0'||str[p]>'5')
                continue;
            bricks[i][j++].type=str[p]-'0';
            if(str[p]-'0'!=empty)
                Number++;
            if(j==Right_Margin){
                i++;
                j=Left_Margin;
                if(i==Bottom_Margin){
                    file.close();
                    return;
                }
            }
        }
    }
    for(;i<Bottom_Margin;i++){
        for(;j<Right_Margin;j++){
            bricks[i][j].type=empty;
        }
    }
    file.close();
}

int Bricks::Hit(int x, int y,Mythread *ball){
    for(int i=y/40-1;i<=y/40+1&&i<Bottom_Margin;i++){
        for(int j=x/40-1;j<=x/40+1&&j<Right_Margin;j++){
            if(i>=Top_Margin&&j>=Left_Margin&&bricks[i][j].is_hit(x,y,ball)){
                if(bricks[i][j].type==stone){
                    sound_stone[seq_stone++].play(stone);
                    if(seq_stone>=10)
                        seq_stone=0;
                }
                else if(bricks[i][j].type==bomb){
                    sound_bomb[seq_bomb++].play(bomb);
                    if(seq_bomb>=5)
                        seq_bomb=0;
                }
                else{
                    sound_diamond[seq_diamond++].play(diamond);
                    if(seq_diamond>=5)
                        seq_diamond=0;
                    }
                return bricks[i][j].destroy(bricks,i,j,Left_Margin,Top_Margin,Right_Margin,Bottom_Margin,Number,ball);
            }
        }
    }
    return 0;
}
