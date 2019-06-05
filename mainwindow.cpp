#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "waypoint.h"
#include "enemy.h"
#include "bullet.h"

static const int Towercost=200;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
  , waves(0)
  , playerHp(5)
  //, m_playrGold(1000)
  , gameLose(false)
  , gameWin(false)
{
    ui->setupUi(this);
    loadTowerPositions();

    //preLoadWavesInfo();
    addwaypoint();
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateMap()));
    timer->start(30);
    QTimer::singleShot(300, this, SLOT(gameStart()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.drawPixmap(0,0,this->width(),this->height(),QPixmap(":/pictures/bg1.png"));

    foreach (const towerposition &towerPos, t_towerpositionlist) {
        towerPos.drawTower(&painter);
    }
    foreach (Tower *tower,t_towerlist) {
        tower->drawTower(&painter);
    }

        foreach (Enemy*enemy, enemylist) enemy->draw(&painter);


    foreach (const waypoint *wayPoint, waypointlist)
        wayPoint->draw(&painter);
    foreach (const Bullet *bullet, bulletlist)
        bullet->drawBullet(&painter);

    if (gameWin || gameLose)
    {
        QString text = gameLose ? "YOU LOST!!!" : "YOU WIN!!!";
        QPainter painter(this);
        painter.setPen(QPen(Qt::red));
        painter.drawText(rect(), Qt::AlignCenter, text);
        return;
    }

    drawWave(&painter);
    drawHP(&painter);
    drawPlayerGold(&painter);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {

    QPoint pressPos = event->pos();
    auto it = t_towerpositionlist.begin();
    while (it != t_towerpositionlist.end())
    {
        if (canbuyTower() && it->containTower(pressPos) && !it->canhaveTower())
        {
            //m_audioPlayer->playSound(TowerPlaceSound);
            playerGold -= Towercost;
            it->sethaveTower();

            Tower *tower = new Tower(it->centerPos(), this);
            t_towerlist.push_back(tower);
            update();
            break;
        }

        ++it;
    }
}

void MainWindow::loadTowerPositions() {
    QPoint pos[]={
        QPoint(100, 480),
        QPoint(200, 480),
        QPoint(380, 400),
        QPoint(380, 300),
        QPoint(380, 200),
        QPoint(620, 220),
        QPoint(780, 350),
        QPoint(300, 480),
        QPoint(950, 350),
        QPoint(950, 450),
        QPoint(950, 550),
        QPoint(550, 350)	};
    int len	= sizeof(pos) / sizeof(pos[0]);
    for (int i = 0; i < len; ++i)
        t_towerpositionlist.push_back(pos[i]);

}

void MainWindow::removeEnemy(Enemy *enemy) {
    Q_ASSERT(enemy);
    enemylist.removeOne(enemy);
    if (enemylist.empty()) {
        waves++;
        if (!loadWaves()) {
            gameWin=true;
        }
    }
}

QList<Enemy *> MainWindow::enemyList() const {
    return enemylist;
}

void MainWindow::removeBullet(Bullet *bullet) {
    Q_ASSERT(bullet);

    bulletlist.removeOne(bullet);
    delete bullet;
}

void MainWindow::addwaypoint() {
    waypoint *wayPoint1 = new waypoint(QPoint(420, 285));
    waypointlist.push_back(wayPoint1);

    waypoint *wayPoint2 = new waypoint(QPoint(35, 285));
    waypointlist.push_back(wayPoint2);
    wayPoint2->setNextWayPoint(wayPoint1);

    waypoint *wayPoint3 = new waypoint(QPoint(35, 195));
    waypointlist.push_back(wayPoint3);
    wayPoint3->setNextWayPoint(wayPoint2);

    waypoint *wayPoint4 = new waypoint(QPoint(445, 195));
    waypointlist.push_back(wayPoint4);
    wayPoint4->setNextWayPoint(wayPoint3);

    waypoint *wayPoint5 = new waypoint(QPoint(445, 100));
    waypointlist.push_back(wayPoint5);
    wayPoint5->setNextWayPoint(wayPoint4);

    waypoint *wayPoint6 = new waypoint(QPoint(35, 100));
    waypointlist.push_back(wayPoint6);
    wayPoint6->setNextWayPoint(wayPoint5);
}

bool MainWindow::loadWaves(){
    if(waves>=6){
        return false;
    }
    waypoint*startpoint=waypointlist.back();
    int enemyInterval[]={100,500,600,1000,3000,6000};
    for(int i=0;i<6;i++){
        Enemy *enemy=new Enemy(startpoint,this);
        enemylist.push_back(enemy);
        QTimer::singleShot(enemyInterval[i],enemy,SLOT(do_Activate()));
    }
    return true;
}
/*void MainWindow::upDateMap(){
    foreach (Enemy*enemy, enemylist)
        enemy->move();
    update();
}
*/

void MainWindow::getHpDamage(int damage/* = 1*/)
{
    //audioPlayer->playSound(LifeLoseSound);
    playerHp -= damage;
    if (playerHp <= 0)
        doGameOver();
}

void MainWindow::doGameOver()
{
    if (!gameLose)
    {
        gameLose = true;
        // 此处应该切换场景到结束场景
        // 暂时以打印替代,见paintEvent处理
    }
}

void MainWindow::updateMap() {
    foreach (Enemy *enemy, enemylist )
        enemy->move();
    foreach (Tower *tower, t_towerlist)
        tower->checkEnemyInRange();
    update();
}

void MainWindow::addBullet(Bullet *bullet) {
    Q_ASSERT(bullet);

    bulletlist.push_back(bullet);
}


bool MainWindow::canbuyTower() const {
    if (playerGold >= Towercost) return true;
    else return false;
}

void MainWindow::awardGold() {
    playerGold+=100;
    update();
}

void MainWindow::drawWave(QPainter *painter)
{
    painter->setPen(QPen(Qt::red));
    painter->drawText(QRect(400, 5, 100, 25), QString("WAVE : %1").arg(waves + 1));
}

void MainWindow::drawHP(QPainter *painter)
{
    painter->setPen(QPen(Qt::red));
    painter->drawText(QRect(30, 5, 100, 25), QString("HP : %1").arg(waves));
}

void MainWindow::drawPlayerGold(QPainter *painter)
{
    painter->setPen(QPen(Qt::red));
    painter->drawText(QRect(200, 5, 200, 25), QString("GOLD : %1").arg(waves));
}
void MainWindow::gameStart()
{
    loadWaves();
}
