#ifndef PREWINDOW_H
#define PREWINDOW_H

#include <QMainWindow>
#include <QPushButton>

namespace Ui {
class PreWindow;
}

class PreWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PreWindow(QWidget *parent = nullptr);
    ~PreWindow();
    bool Full=false;//记录窗口状态
    int Level_Num=10;//记录总关卡数
    QPushButton **Button;//根据关卡数新建相应的按钮
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_Button_clicked();

    void on_pushButton_4_clicked();

public:
    Ui::PreWindow *ui;
};

#endif // PREWINDOW_H
