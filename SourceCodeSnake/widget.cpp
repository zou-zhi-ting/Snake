#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QPalette>
#include <QPainter>
#include <QRect>
#include <QBrush>
#include <QKeyEvent>
#include <QTime>
#include <ctime>
#include <QMessageBox>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    Initial();
    m_Timer=new QTimer(this);

    m_Timer->stop();
    m_Timer->setInterval(SNAKE_RATE*100);
    connect(m_Timer,&QTimer::timeout,[=](){
            if(canMove){
                //更新蛇的坐标
                MoveSnake();
                if(beEaten){
                    generateFood();
                }
                Search();
                //重新绘制
                SetScore();
                update();
            }


        });
    this->grabKeyboard();
}
void Widget::Initial()
{
    this->setWindowTitle("贪吃蛇");
    resize(WIN_WIDTH,WIN_HEIGHT);
    ui->lab_ground->setGeometry(GROUND_X,GROUND_Y,GROUND_WIDTH,GROUND_HEIGTH);
    ui->lab_ground->setAttribute(Qt::WA_TranslucentBackground, false);
    m_score=0;
    ui->lab_score->setText("得分："+QString::number(m_score));
    /**/
    /*SNAKE_HEAD=new Body(100,100);
    SNAKE_HEAD->next=new Body(100-SNAKE_WIDTH,100);
    SNAKE_HEAD->next->next=new Body(100-2*SNAKE_WIDTH,100);
    SNAKE_HEAD->next->next->next=new Body(100-3*SNAKE_WIDTH,100);
    SNAKE_HEAD->next->next->next->next=new Body(100-4*SNAKE_WIDTH,100);*/
    /**/
    /**/
    SNAKE_HEAD=new Body(GROUND_X+200,GROUND_X+200);
    SNAKE_HEAD->next=new Body(GROUND_X+200,GROUND_X+200+SNAKE_WIDTH);
    SNAKE_HEAD->next->next=new Body(GROUND_X+200,GROUND_X+200+2*SNAKE_WIDTH);
    SNAKE_HEAD->next->next->next=new Body(GROUND_X+200,GROUND_X+200+3*SNAKE_WIDTH);
    SNAKE_HEAD->next->next->next->next=new Body(GROUND_X+200,GROUND_X+200+4*SNAKE_WIDTH);
    /**/
}


void Widget::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QBrush brush;
    /*绘制背景色*/
    brush.setColor(Qt::black);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    painter.drawRect(QRect(GROUND_X,GROUND_Y,GROUND_WIDTH,GROUND_HEIGTH));
    /**/
    /*绘制食物*/
    brush.setColor(Qt::yellow);
    painter.setBrush(brush);
    painter.drawEllipse(m_Food.x(),m_Food.y(),SNAKE_WIDTH,SNAKE_WIDTH);

    /*绘制蛇*/
    brush.setColor(Qt::red);
    painter.setBrush(brush);
    if(SNAKE_HEAD){
        auto p=SNAKE_HEAD;
        painter.drawEllipse(p->x,p->y,SNAKE_WIDTH,SNAKE_WIDTH);
        while(p->next){
            p=p->next;
            painter.drawRect(QRect(p->x,p->y,SNAKE_WIDTH,SNAKE_WIDTH));
        }
    }
}
void Widget::keyPressEvent(QKeyEvent *event){
    switch(event->key())
         {
         case Qt::Key_Up:
            if(UP||DOWN){
                break;
            }
            else if(LEFT){
                LEFT=false;
                UP=true;
            }else if(RIGHT){
                RIGHT=false;
                UP=true;
            }
             break;
         case Qt::Key_Down:
            if(UP||DOWN){
                break;
            }
            else if(LEFT){
                LEFT=false;
                DOWN=true;
            }else if(RIGHT){
                RIGHT=false;
                DOWN=true;
            }
             break;
         case Qt::Key_Left:
            if(LEFT||RIGHT){
                break;
            }
            else if(UP){
                UP=false;
                LEFT=true;
            }else if(DOWN){
                DOWN=false;
                LEFT=true;
            }
             break;
         case Qt::Key_Right:
            if(LEFT||RIGHT){
                break;
            }
            else if(UP){
                UP=false;
                RIGHT=true;
            }else if(DOWN){
                DOWN=false;
                RIGHT=true;
            }
             break;
         default:
             //QGraphicsView::keyPressEvent(event);
             break;
         }
}
void Widget::MoveSnake()
{
    if(UP){
        auto p=SNAKE_HEAD;
        int x,y,tx,ty;
        x=p->x;
        y=p->y-SNAKE_SPEED*SNAKE_WIDTH;
        while(p){
            tx=p->x;
            ty=p->y;
            p->x=x;
            p->y=y;
            p=p->next;
            x=tx;
            y=ty;
        }
    }else if(DOWN){
        auto p=SNAKE_HEAD;
        int x,y,tx,ty;
        x=p->x;
        y=p->y+SNAKE_SPEED*SNAKE_WIDTH;
        while(p){
            tx=p->x;
            ty=p->y;
            p->x=x;
            p->y=y;
            p=p->next;
            x=tx;
            y=ty;
        }
    }else if(LEFT){
        auto p=SNAKE_HEAD;
        int x,y,tx,ty;
        x=p->x-SNAKE_SPEED*SNAKE_WIDTH;
        y=p->y;
        while(p){
            tx=p->x;
            ty=p->y;
            p->x=x;
            p->y=y;
            p=p->next;
            x=tx;
            y=ty;
        }
    }else if(RIGHT){
        auto p=SNAKE_HEAD;
        int x,y,tx,ty;
        x=p->x+SNAKE_SPEED*SNAKE_WIDTH;
        y=p->y;
        while(p){
            tx=p->x;
            ty=p->y;
            p->x=x;
            p->y=y;
            p=p->next;
            x=tx;
            y=ty;
        }
    }
}
void Widget::EatFood(int x, int y){
    Body* tmp=new Body(x,y);
    tmp->next=SNAKE_HEAD->next;
    SNAKE_HEAD->next=tmp;
    beEaten=true;
}


void Widget::generateFood()
{
    auto p=SNAKE_HEAD;
    QVector<QPoint> vec;
    while(p){
        vec.push_back(QPoint(p->x,p->y));
        p=p->next;
    }
    int w=(qrand()%(GROUND_WIDTH/SNAKE_WIDTH))*SNAKE_WIDTH+GROUND_X;
    int h=(qrand()%(GROUND_HEIGTH/SNAKE_WIDTH))*SNAKE_WIDTH+GROUND_Y;
    for(int i=0;i<vec.size();i++){
        if(vec[i].x()==w&&vec[i].y()==h){
            i=0;
            w=(qrand()%(GROUND_WIDTH/SNAKE_WIDTH))*SNAKE_WIDTH+GROUND_X;
            h=(qrand()%(GROUND_WIDTH/SNAKE_WIDTH))*SNAKE_WIDTH+GROUND_X;
        }
        else{
            continue;
        }
    }
    m_Food=QPoint(w,h);
    beEaten=false;
}

void Widget::Search()
{
    if(SNAKE_HEAD->x==m_Food.x()&&SNAKE_HEAD->y==m_Food.y()){
        EatFood(m_Food.x(),m_Food.y());
        m_score+=5;
    }
}

void Widget::SetScore()
{
    auto p=SNAKE_HEAD;
    while(p){
        if(p->x>GROUND_X+GROUND_WIDTH||p->x<GROUND_X||p->y>GROUND_Y+GROUND_HEIGTH||p->y<GROUND_Y){
            QMessageBox::information(this,QStringLiteral("失败"),QStringLiteral("游戏结束，再接再厉！"));
            canMove=false;
            m_Timer->stop();
            return;
        }
        p=p->next;
    }
    ui->lab_score->setText("得分："+QString::number(m_score));
}

void Widget::ClearSnake()
{
    auto p=SNAKE_HEAD;
    while(p){
        auto tmp=p;
        p=p->next;
        delete tmp;
    }
    UP=false;
    DOWN=false;
    LEFT=false;
    RIGHT=false;
}
Widget::~Widget()
{
    delete ui;
}

void Widget::on_pbt_start_clicked()
{
    m_score=0;
    ClearSnake();
    canMove=true;
    beEaten=true;
    SNAKE_HEAD=nullptr;
    SNAKE_HEAD=new Body(GROUND_X+200,GROUND_X+200);
    SNAKE_HEAD->next=new Body(GROUND_X+200,GROUND_X+200+SNAKE_WIDTH);
    SNAKE_HEAD->next->next=new Body(GROUND_X+200,GROUND_X+200+2*SNAKE_WIDTH);
    SNAKE_HEAD->next->next->next=new Body(GROUND_X+200,GROUND_X+200+3*SNAKE_WIDTH);
    SNAKE_HEAD->next->next->next->next=new Body(GROUND_X+200,GROUND_X+200+4*SNAKE_WIDTH);
    m_Timer->stop();
    m_Timer->start();
    UP=true;
}

void Widget::on_pbt_exit_clicked()
{
    this->close();
}
