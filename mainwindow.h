#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>

#include <QMainWindow>
#include <QList>
#include "towerposition.h"
#include "tower.h"
#include "waypoint.h"
#include "enemy.h"
#include<QTimer>

class waypoint;
class Enemy;
class Bullet;
class AudioPlayer;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void loadTowerPositions();
    bool canbuyTower() const;
    void removeEnemy(Enemy *enemy);
    void removeBullet(Bullet *bullet);
    QList<Enemy *> enemyList() const;
    void addwaypoint();
    bool loadWaves();
    void getHpDamage(int damage=1);
    void doGameOver();

    void addBullet(Bullet *bullet);
    void awardGold();

    void drawHP(QPainter *painter);
    void drawPlayerGold(QPainter *painter);
    void drawWave(QPainter *painter);

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
//    void upDateMap();
private slots:
    void gameStart();
    void updateMap();
private:
    int waves;//敌人波数
    bool gameWin;
    bool gameLose;
    int playerHp;
    int playerGold;


    Ui::MainWindow *ui;
    QList<Tower *> t_towerlist;
    QList<towerposition> t_towerpositionlist;
    QList<Enemy *> enemylist;
    QList<Bullet *> bulletlist;
    QList<waypoint *> waypointlist;

};

#endif // MAINWINDOW_H
