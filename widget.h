#ifndef WIDGET_H
#define WIDGET_H

#include<QApplication>
#include <QWidget>
#include<QFileDialog>
#include<QMessageBox>
#include<QDebug>
#include<QFileInfo>
#include<QMediaPlayer>
#include<QSoundEffect>
#include<QDir>
#include<QMediaPlaylist>
#include<QTime>
#include<QTimer>
#include<QStyleOption>
#include<QPainter>
#include <QRandomGenerator>
#include<QLCDNumber>
#include "RandL_button.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_brobtn_clicked();
    void on_playbtn_clicked();
    void on_rdmbtn_clicked();
    void positionChange(qint64 position);
    void seekChange(int position);
    void musicTime();
    void increaseVolume();
    void decreaseVolume();
    void paintEvent(QPaintEvent *e);
    int generateRandomNumber();
    void randombg(int bgnum);
    void changeTime();
private:
    Ui::Widget *ui;
    QLCDNumber  *clock = new QLCDNumber(this);
    RandLbtn *vlobtn = new RandLbtn(this);
    QMediaPlayer *player = new QMediaPlayer;
    QTimer *Ctimer = new QTimer(this);
    QString musicpath = "/",music,musicname;
    QFileInfo musicinfo;
    int moved;
};
#endif // WIDGET_H
