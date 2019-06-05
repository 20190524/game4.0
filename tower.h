#ifndef TOWER_H
#define TOWER_H

#include <QPoint>
#include <QSize>
#include <QPixmap>
#include <QObject>

class QPainter;
class MainWindow;
class QTimer;
class Enemy;
class Bullet;


class Tower: QObject
{
    Q_OBJECT
public:
    Tower(QPoint pos,MainWindow *game,const QPixmap &sprite = QPixmap(":/pictures/tank2.png"));
    ~Tower();

    void drawTower(QPainter *painter);
    void chooseEnemyforAttack(Enemy *enemy);
    void attackEnemy();
    void loseSightofEnmey();
    void enemyKilled();
    void checkEnemyInRange();

    //void removeBullet();

private slots:
    void shootWeapon();

private:
    bool t_attack;
    int t_attackRange;
    int t_attackDamage;
    int t_attackBreak;
    double t_rotationSprite;


    QPoint t_pos;
    QTimer * t_attackBreakTimer;
    QPixmap t_sprite;
    MainWindow * t_game;
    Enemy * t_chooseEnemy;

    static const QSize fixedSize;


};

#endif // TOWER_H
