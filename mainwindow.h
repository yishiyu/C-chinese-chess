#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QLabel>
#include "board.h"

#define GRID_WIDTH 44

namespace Ui {
class MainWindow;
}

//主窗口继承棋盘类
class MainWindow : public QMainWindow, public Board
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    //棋子
    QLabel *QPieces[48];
    int SideTag = 16;

    //走棋合法判断
    int from_x=0,from_y=0,to_x=0,to_y=0;
    bool isLegal();

    //点击事件处理函数
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

    //图形执行走法
    void GameGo(int from_x,int from_y,int to_x,int to_y);
    void GameGo(::move step);
};

#endif // MAINWINDOW_H
