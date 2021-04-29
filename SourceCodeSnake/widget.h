#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVector>
#include <QTimer>
#include "body.h"
#include <QPoint>
#define WIN_WIDTH 700
#define WIN_HEIGHT 500
#define GROUND_X 25
#define GROUND_Y 25
#define GROUND_WIDTH 500
#define GROUND_HEIGTH 450
#define SNAKE_WIDTH 10
#define SNAKE_SPEED 1
#define SNAKE_RATE 4

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void Initial();
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void MoveSnake();
    void EatFood(int x,int y);
    void generateFood();
    void Search();
    void SetScore();
    void ClearSnake();
private slots:
    void on_pbt_start_clicked();

    void on_pbt_exit_clicked();

private:
    int m_score;
    QTimer *m_Timer;
    Body* SNAKE_HEAD=nullptr;
    bool UP=false;
    bool DOWN=false;
    bool LEFT=false;
    bool RIGHT=false;
    bool FOUND=false;
    bool beEaten=true;
    bool canMove=true;
    QPoint m_Food;
private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
