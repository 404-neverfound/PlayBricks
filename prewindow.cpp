#include "ui_prewindow.h"
#include "mainwindow.h"
#include "prewindow.h"
#include <QPushButton>
#include <QDesktopWidget>
#include <QFont>

PreWindow::PreWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PreWindow)
{
    Button=new QPushButton*[Level_Num];
    ui->setupUi(this);
    setMaximumSize(1600,900);
    setMinimumSize(1600,900);
    QFont font;
    font.setFamily("华文行楷");
    font.setPixelSize(30);
    font.setBold(true);
    int n=0;
    for(int i=1,j=1;n<Level_Num;i++){
        for(j=1;j<13&&n<Level_Num;j++,n++){
            Button[n]=new QPushButton(ui->centralwidget);
            Button[n]->setText("第"+QString::number(n+1)+"关");
            Button[n]->setGeometry(j*110,i*90,100,80);
            Button[n]->setFont(font);
            Button[n]->hide();
            connect(Button[n],SIGNAL(clicked()),this,SLOT(on_Button_clicked()));
        }
    }
    ui->pushButton_4->hide();
    ui->pushButton_4->setShortcut(Qt::Key_Escape);
}

PreWindow::~PreWindow()
{
    delete ui;
}

void PreWindow::on_pushButton_clicked()
{
    ui->pushButton->hide();
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();
    for(int i=0;i<Level_Num;i++)
        Button[i]->show();
    ui->pushButton_4->show();
}

void PreWindow::on_pushButton_2_clicked()
{
    Full=!Full;
    if(Full==true){
        showFullScreen();
        QDesktopWidget *desktop=QApplication::desktop();
        QRect rect=desktop->screenGeometry();
        ui->centralwidget->setGeometry((rect.width()-1600)/2,(rect.height()-900)/2,1600,900);
    }
    else{
        showNormal();
    }
}

void PreWindow::on_Button_clicked(){
    QString str=((QPushButton*)sender())->text();
    MainWindow *w=new MainWindow(this,Full,str.mid(1,str.length()-2).toInt());
    //MainWindow *w=new MainWindow(this,Full,0);//隐藏关卡
    ui->pushButton->show();
    ui->pushButton_2->show();
    ui->pushButton_3->show();
    for(int i=0;i<Level_Num;i++)
        Button[i]->hide();
    ui->pushButton_4->hide();
    w->show();
    hide();
}

void PreWindow::on_pushButton_3_clicked()
{
    exit(0);
}

void PreWindow::on_pushButton_4_clicked()
{
    ui->pushButton->show();
    ui->pushButton_2->show();
    ui->pushButton_3->show();
    for(int i=0;i<Level_Num;i++)
        Button[i]->hide();
    ui->pushButton_4->hide();
}
