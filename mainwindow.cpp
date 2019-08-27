#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //数组与棋子标签绑定

    {
        /*
        红方：帅 仕 仕 相 相 马 马 车 车 炮 炮 兵
              16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
        黑方：将 士 士 象 象 马 马 车 车 炮 炮 卒
              32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47
        */
        QPieces[32] = ui->b_chess5;
        QPieces[33] = ui->b_chess4;
        QPieces[34] = ui->b_chess6;
        QPieces[35] = ui->b_chess3;
        QPieces[36] = ui->b_chess7;
        QPieces[37] = ui->b_chess2;
        QPieces[38] = ui->b_chess8;
        QPieces[39] = ui->b_chess1;
        QPieces[40] = ui->b_chess9;
        QPieces[41] = ui->b_chess10;
        QPieces[42] = ui->b_chess11;
        QPieces[43] = ui->b_chess12;
        QPieces[44] = ui->b_chess13;
        QPieces[45] = ui->b_chess14;
        QPieces[46] = ui->b_chess15;
        QPieces[47] = ui->b_chess16;

        QPieces[16] = ui->b_chess5_2;
        QPieces[17] = ui->b_chess4_2;
        QPieces[18] = ui->b_chess6_2;
        QPieces[19] = ui->b_chess3_2;
        QPieces[20] = ui->b_chess7_2;
        QPieces[21] = ui->b_chess2_2;
        QPieces[22] = ui->b_chess8_2;
        QPieces[23] = ui->b_chess1_2;
        QPieces[24] = ui->b_chess9_2;
        QPieces[25] = ui->b_chess10_2;
        QPieces[26] = ui->b_chess11_2;
        QPieces[27] = ui->b_chess12_2;
        QPieces[28] = ui->b_chess13_2;
        QPieces[29] = ui->b_chess14_2;
        QPieces[30] = ui->b_chess15_2;
        QPieces[31] = ui->b_chess16_2;
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::isLegal()
{
    /*
    红方：帅 仕 仕 相 相 马 马 车 车 炮 炮 兵
          16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
    黑方：将 士 士 象 象 马 马 车 车 炮 炮 卒
          32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47
    */
    //获取被移动的棋子和移动终点的棋子
    int from = from_x+3 + from_y*16 + 48;
    int to = to_x+3 + to_y*16 + 48;

    switch (board[from]) {
    case 16:    //帅
        if(KingPosition[to] && !(board[to]&SideTag)){
            for(int i=0;i<4;i++){
                if(from + KingDir[i] == to)
                    return true;
            }
        }
        break;
    case 17:    //仕
    case 18:
        if(AdvisorPosition[to] && !(board[to]&SideTag)){
            for(int i=0;i<4;i++){
                if(from + AdvisorDir[i] == to)
                    return true;
            }
        }
        break;
    case 19:    //相
    case 20:
        if(BishopPosition[to] && !(board[to]&SideTag)){
            for(int i=0;i<4;i++){
                if((from + BishopDir[i] == to ) && !(board[from + BishopCheck[i]]))
                    return true;
            }
        }
        break;
    case 21:    //马
    case 22:
        if(KnightPosition[to] && !(board[to]&SideTag)){
            for(int i=0;i<8;i++){
                if((from + KnightDir[i] == to ) && !(board[from + KnightCheck[i]])){
                    return true;
                }

            }
        }
        break;
    case 23:    //车
    case 24:
        //落子处合法
        if(RookPosition[to] && !(board[to]&SideTag)){
            //遍历四个方向
            for(int j=0;j<4;j++){
                //搜索八个位置
                for(int i=1;i<9;i++){
                    //如果出了棋盘则停止
                    if(!RookPosition[from + i * RookDir[j]]){
                        break;
                    }
                    else{
                        //可以到达
                        if(from + i * RookDir[j] == to){
                            return true;
                        }//路径中有棋子
                        else if(board[from + i * RookDir[j]]){
                            break;
                        }
                    }
                }
            }
        }
        break;
    case 25:    //炮
    case 26:
        //落子处合法
        if(CannonPosition[to] && !(board[to]&SideTag)){
            //遍历四个方向
            for(int j=0;j<4;j++){
                bool OverFlag = false;
                //遍历八个位置
                for(int i=1;i<9;i++){
                    //已经翻山
                    if(OverFlag){
                        //当前位置有子
                        if(board[from + i*CannonDir[j]]){
                            //为目的地且可以吃子
                            if(from + i*CannonDir[j]==to && board[to]&(48-SideTag)){
                                //qDebug("Sidetag:: %d",48-SideTag);
                                return true;
                            }
                            else {
                                break;
                            }
                        }
                    }
                    //没有翻山
                    else {
                        //可以到达且目的地没有子
                        if(from + i*CannonDir[j]==to && !board[to]){
                            return true;
                        }
                        //翻山
                        if(board[from + i*CannonDir[j]]){
                            OverFlag = true;
                        }
                    }
                }
            }
        }
        break;
    case 27:    //兵
    case 28:
    case 29:
    case 30:
    case 31:
        if(SoldierPostion[side][to] && !(board[to]&SideTag)){
            for(int i=0;i<3;i++){
                if(from + SoldierDir[side][i] == to)
                    return true;
            }
        }
        break;
    }
    return false;
}

//鼠标点下去
void MainWindow::mousePressEvent(QMouseEvent *e)
{
    int x = (int)((e->x()+5.25)/44.5);
    int y = (int)((e->y()-17.75)/44.5);

    //合法
    if(x>=0&&x<9&&y>=0&&y<10){
        from_x = x;
        from_y = y;
    }
}

//鼠标抬起来
void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    int x = (int)((e->x()+5.25)/44.5);
    int y = (int)((e->y()-17.75)/44.5);

    if(Defeat||Victory)
        return;

    //合法
    if(x>=0&&x<9&&y>=0&&y<10){
        to_x = x;
        to_y = y;

        if(isLegal()){
            //qDebug("player: %d, %d, %d, %d",from_x,from_y,to_x,to_y);

            //图形执行走法
            GameGo(from_x,from_y,to_x,to_y);

            //逻辑执行走法
            ::move temp;
            temp = H_Player(from_x,from_y,to_x,to_y);

            //为电脑执行图形走法
            //qDebug("bestmove.local: %d,%d", temp.from, temp.to);
            //qDebug("bestmove: %d,%d,%d,%d", temp.from%16-3,temp.from/16-3, temp.to%16-3,temp.to/16-3);
            if(!Defeat && !Victory){
                GameGo(temp);
            }
            else {
                if(Victory){
                    GameGo(temp);
                    qDebug("***Victory***",0);
                }
                if(Defeat)
                    qDebug("***Defeat***",1);
            }

        }
    }
}

//图形走法
void MainWindow::GameGo(int from_x,int from_y,int to_x,int to_y)
{
    QLabel *from;
    QLabel *to;

    from = QPieces[board[from_x+3+from_y*16+48]];
    to = QPieces[board[to_x+3+to_y*16+48]];

    if(board[to_x+3+to_y*16+48]){
        to->setVisible(false);
    }

    from->setGeometry(44*to_x,44*to_y,44,44);

}

void MainWindow::GameGo(::move step){

    QLabel *from;
    QLabel *capture;

    //qDebug("bestmove.piece: %d, %d", board[step.to], step.capture);

    from = QPieces[board[step.to]];
    capture = QPieces[step.capture];

    if(step.capture){
        capture->setVisible(false);
    }
    from->setGeometry((44*(step.to%16-3)),(44*(step.to/16-3)),44,44);
}

/*
 * 点击测试数据
pos:17  42
pos:372  38
pos:17  219
pos:370  218

pos:17  264
pos:371  263
pos:13  441
pos:373  437

17--372--44.25
40--218--44.5
262--440--44.5

(17,40)--44.5
*/






