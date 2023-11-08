#include "widget.h"
#include "./ui_widget.h"


QMediaPlaylist *playerlist;
int amount,play_now;
QStringList namelist;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowIcon (QIcon (":/pho/window.ico"));
    this->setWindowTitle ("Fly Your MOOD~");

    randombg(generateRandomNumber());//封面随机数生成

    // 设置显示位数
    clock->setDigitCount(8);
    // 设置显示的模式为十进制
    clock->setMode(QLCDNumber::Dec);
    //时钟样式
    clock->setSegmentStyle(QLCDNumber::Filled);
    clock->setStyleSheet ("background:transparent;color:#DCDCDC;");
    clock->move (30,260);
    clock->resize (340,100);

    //时钟定时器
    Ctimer->setInterval(1000);// 设置定时间隔，每秒更新时间
    connect(Ctimer, SIGNAL(timeout()), this, SLOT(changeTime()));
    Ctimer->start();// 启动!

    //自定义音量按钮
    vlobtn->move (140,110);
    vlobtn->resize (20,20);
    vlobtn->setMinimumSize (20,20);
    vlobtn->setMaximumSize (20,20);

    //设置按钮效果 ps：网上好哥哥写的，实现按压效果
    ui->playbtn->setStyleSheet("QPushButton{background-color: rgb(220,220,220);border:2px groove gray;border-radius:30px;padding:2px 4px;border-style: outset;}"
                               "QPushButton:hover{background-color:rgb(245,245,245); color: black;}"
                               "QPushButton:pressed{background-color:rgb(169,169,169);border-style: inset;}");
    ui->rdmbtn->setStyleSheet("QPushButton{background-color: rgb(220,220,220);border:2px groove gray;border-radius:30px;padding:2px 4px;border-style: outset;}"
                              "QPushButton:hover{background-color:rgb(245,245,245); color: black;}"
                              "QPushButton:pressed{background-color:rgb(169,169,169);border-style: inset;}");
    ui->brobtn->setStyleSheet("QPushButton{background-color: rgb(220,220,220);border:2px groove gray;border-radius:10px;padding:2px 4px;border-style: outset;}"
                              "QPushButton:hover{background-color:rgb(245,245,245); color: black;}"
                              "QPushButton:pressed{background-color:rgb(169,169,169);border-style: inset;}");
    vlobtn->setStyleSheet("QPushButton{background-color: rgb(220,220,220);border:2px groove gray;border-radius:10px;padding:2px 4px;border-style: outset;}"
                              "QPushButton:hover{background-color:rgb(245,245,245); color: black;}"
                              "QPushButton:pressed{background-color:rgb(169,169,169);border-style: inset;}");

    //按钮图标及图标大小
    ui->playbtn->setIcon (QIcon(":/pho/play.png"));
    ui->playbtn->setIconSize (QSize(70,70));
    ui->rdmbtn->setIcon (QIcon(":/pho/random1.png"));
    ui->rdmbtn->setIconSize (QSize(60,60));
    ui->brobtn->setIcon (QIcon(":/pho/browse.png"));
    ui->brobtn->setIconSize (QSize(20,20));
    ui->playbtn->setIcon (QIcon(":/pho/play.png"));
    ui->playbtn->setIconSize (QSize(70,70));
    vlobtn->setIcon (QIcon(":/pho/vloume.png"));
    vlobtn->setIconSize (QSize (18,18));

    //进度条 ps：网上好哥哥写的
    ui->playerslider->setStyleSheet ("/*horizontal ：水平QSlider*/QSlider::groove:horizontal {border: 0px solid #bbb;}"
                                    /*1.滑动过的槽设计参数*/
                                    "QSlider::sub-page:horizontal {"
                                    /*槽颜色*/
                                    "background: rgb(108,123,139);"
                                    /*外环区域倒圆角度*/
                                    "border-radius: 2px;"
                                    /*上遮住区域高度*/
                                    "margin-top:8px;"
                                    /*下遮住区域高度*/
                                    "margin-bottom:8px;}"

                                    /*2.未滑动过的槽设计参数*/
                                    "QSlider::add-page:horizontal {"
                                    /*槽颜色*/
                                    "background: rgb(255,255, 255);"
                                    /*外环大小0px就是不显示，默认也是0*/
                                    "border: 0px solid #777;"
                                    /*外环区域倒圆角度*/
                                    "border-radius: 2px;"
                                    /*上遮住区域高度*/
                                    "margin-top:9px;"
                                    /*下遮住区域高度*/
                                    "margin-bottom:9px;}"

                                    /*3.平时滑动的滑块设计参数*/
                                    "QSlider::handle:horizontal {"
                                    /*滑块颜色*/
                                    "background: rgb(79,79,79);"
                                    /*滑块的宽度*/
                                    "width: 5px;"
                                    /*滑块外环为1px，再加颜色*/
                                    "border: 1px solid rgb(79,79,79);"
                                    /*滑块外环倒圆角度*/
                                    "border-radius: 2px; "
                                    /*上遮住区域高度*/
                                    "margin-top:6px;"
                                    /*下遮住区域高度*/
                                    "margin-bottom:6px;}"

                                    /*4.手动拉动时显示的滑块设计参数*/
                                    "QSlider::handle:horizontal:hover {"
                                    /*滑块颜色*/
                                    "background: rgb(193,204,208);"
                                    /*滑块的宽度*/
                                    "width: 10px;"
                                    /*滑块外环为1px，再加颜色*/
                                    "border: 1px solid rgb(193,204,208);"
                                    /*滑块外环倒圆角度*/
                                    "border-radius: 5px; "
                                    /*上遮住区域高度*/
                                    "margin-top:4px;"
                                    /*下遮住区域高度*/
                                    "margin-bottom:4px;}");

    //连接播放器的播放状态，以便针对不同状态做出不同反应
    connect(player, &QMediaPlayer::mediaStatusChanged, [=](QMediaPlayer::MediaStatus mediaStatus) {
        //EndOfMedia 一个文件播放完成
        if (mediaStatus == QMediaPlayer::EndOfMedia) {
            //列表循环播放，最后一个文件则置零
            if(play_now==amount)
            {
                play_now=0;
            }
            else
            {
            play_now+=1;
            }
            playerlist->setCurrentIndex (play_now);
            ui->Inftext->setText (namelist.at(play_now));
            ui->Inftext->setAlignment (Qt::AlignCenter);
            player->play ();

        }
        //载入文件的同时显示文件名
        if(mediaStatus == QMediaPlayer::LoadedMedia)
        {
            playerlist->setCurrentIndex(play_now);
            ui->Inftext->setText (namelist.at(play_now));
            ui->Inftext->setAlignment (Qt::AlignCenter);
        }
    });
    //连接播放器进度与控制进度条位置改变
    connect(player,SIGNAL(positionChanged(qint64)),this,SLOT(positionChange(qint64)));
    connect(player,SIGNAL(durationChanged(qint64)),this,SLOT(musicTime()));
    //Slider位置改变可以控制播放进度
    connect(ui->playerslider,SIGNAL(sliderMoved(int)),this,SLOT(seekChange(int)));
    //左键减音量，右键加音量  ps:对应物理旋钮的状态逆时针减，顺时针加
    connect(vlobtn,SIGNAL(leftButtonClicked()),this, SLOT(decreaseVolume()));
    connect(vlobtn,SIGNAL(rightButtonClicked()),this, SLOT(increaseVolume()));

}

Widget::~Widget()
{
    delete ui;
}

//遍历选择的文件所在的文件夹，添加目录下所有音乐到播放列表
void addMusicslist(QString path)
{
    QDir dir(path);
    //判断路径存在性
    if(!dir.exists())
        return;

    QStringList filters;
    filters<<QString("*.flac")<<QString("*.mp3");
    dir.setFilter (QDir::Files | QDir::NoSymLinks); // 设置类型过滤器
    dir.setNameFilters (filters);
    //文件总数
    int dir_count = dir.count();
    if(dir_count <= 0)
    {
        return;
    }
    //把路径的‘/’换成‘\\’,否则会被识别成转置符 ps: 不如python，直接前面加个r就行了
    QChar separator = QChar('/');
    if(!path.contains(separator))
    {
        separator = QChar('\\');
    }
    QChar last_char = path.at(path.length()-1);
    if(last_char == separator)
    {
        separator = QChar();
    }
    //排队把文件路径添加进playerlist
    for(int i=0; i<dir_count; i++)
    {
        QString music_name = dir [i];
        QString music_path = path + separator + music_name;
        playerlist->addMedia(QUrl::fromLocalFile(music_path));
        //另外把文件名+后缀名存到namelist中，一会有大用(找播放到哪个文件了并显示文件名)
        namelist.append (music_name);
    }
}

void Widget::on_brobtn_clicked()
{
    //浏览
    music = QFileDialog::getOpenFileName(this, tr("What do u want to listen today?"), musicpath , "*.flac *.mp3");
    //把选择的文件的信息存下来
    musicinfo = QFileInfo(music);
    musicname = musicinfo.baseName ();
    musicpath = musicinfo.absolutePath();

    //按了取消，啥都没选就显示这个
    if(music.isEmpty ())
    {
        QMessageBox::warning(this,"Are u sure?","Don't want to listen anything right now?");
    }
    else
    {
        //设置歌名显示栏格式
        ui->Inftext->setFont (QFont("微软雅黑",10));
        //居中 ps：很玄学，之前要设置两次才能真的居中
        ui->Inftext->setAlignment (Qt::AlignCenter);
    }

    playerlist = new QMediaPlaylist;
    playerlist->clear ();

    addMusicslist (musicpath);//自动添加选中文件目录下所有文件
    amount = playerlist->mediaCount ();//曲目总数
    playerlist->setPlaybackMode (QMediaPlaylist::Sequential);//播放模式，列表循环

    //找刚浏览选的文件，并从刚选的开始播放
    for(int i=0;i<amount;i++)
    {
        if(musicinfo.fileName ()==namelist.at (i))
        {
            play_now=i;
            break;
        }
    }

    //播放器初始化
    player->setPlaylist(playerlist);
    player->setVolume(60);//初始音量

}

//播放暂停按钮
void Widget::on_playbtn_clicked()
{
    //如果没在播放并且刚浏览选了文件就开始播放
    if(player->state ()!=1&& music.isEmpty ()==0)
    {
        //按钮图标换成暂停
        ui->playbtn->setIcon (QIcon(":/pho/stop.png"));
        ui->playbtn->setIconSize (QSize(60,60));
        player->play();
    }
    else
    {
        //图标再换回播放
        ui->playbtn->setIcon (QIcon(":/pho/play.png"));
        ui->playbtn->setIconSize (QSize(70,70));
        player->pause ();
    }
}

//随机播放按钮，效果是按下就切随机歌
void Widget::on_rdmbtn_clicked()
{
    //前提先浏览选了文件
    if(!music.isEmpty ())
    {
        //没在播放
        if(player->state ()==0)
        {
            playerlist->setPlaybackMode(QMediaPlaylist::Random);//播放模式改成随机
            play_now=playerlist->currentIndex ();//获取当前在播哪个
            //更新歌名显示
            ui->Inftext->setText (namelist.at(play_now));
            ui->Inftext->setAlignment (Qt::AlignCenter);
            //播放暂停按钮图标改成暂停
            ui->playbtn->setIcon (QIcon(":/pho/stop.png"));
            ui->playbtn->setIconSize (QSize(60,60));
            player->play();
        }
        else
        {
            player->stop ();
            playerlist->setPlaybackMode(QMediaPlaylist::Random);
            playerlist->next ();//播随机列表的下一个
            play_now=playerlist->currentIndex ();

            ui->Inftext->setText (namelist.at(play_now));
            ui->Inftext->setAlignment (Qt::AlignCenter);

            ui->playbtn->setIcon (QIcon(":/pho/stop.png"));
            ui->playbtn->setIconSize (QSize(60,60));
            player->play ();
        }
    }
}

//进度控制滑块
void Widget::positionChange(qint64 position)
{
    ui->playerslider->setMaximum(player->duration() / 1000);//设置滑块范围
    ui->playerslider->setValue(position / 1000);//滑块随动
    moved = position;
    QTime moveTime(0,(moved/60000) % 60,(moved / 1000) % 60);//换算时间
    ui->nowtime ->setText(moveTime.toString("mm:ss"));//显示
}

//滑块控制进度
void Widget::seekChange(int position)
{
    player -> setPosition(position * 1000);
    moved = position * 1000;
    QTime moveTime(0,(moved/60000) % 60,(moved / 1000) % 60);
    ui->nowtime ->setText(moveTime.toString("mm:ss"));
}

//总时长获取
void Widget::musicTime(){
    qint64 time = player->duration();//获取文件时间长度
    QTime Time(0,(time % (1000 * 60 * 60)) / (1000 * 60),(time % (1000 * 60)) / 1000);
    ui->alltime->setText(Time.toString("mm:ss"));
}

//加音量
void Widget::increaseVolume()
{
    if(player->volume() < 100)
    {
        player->setVolume(player->volume() + 10);
        vlobtn->setIcon (QIcon(":/pho/vloume.png"));
        vlobtn->setIconSize (QSize (18,18));
    }
}

//减音量
void Widget::decreaseVolume()
{
    if(player->volume() > 0)
    {
        player->setVolume(player->volume() - 10);
    }
    if(player->volume ()==0)
    {
        vlobtn->setIcon (QIcon(":/pho/mult.png"));
        vlobtn->setIconSize (QSize (18,18));
    }
}

//实现widget背景正常显示的一种方法，重写事件函数
void Widget::paintEvent(QPaintEvent *e)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

//随机背景图
void Widget::randombg(int bgnum)
{
    switch(bgnum)
    {
    case 1:
        QWidget::setStyleSheet ("#Widget {border-image: url(:/pho/bg1.png); } "
                               "#Widget * { border-image:url(); }");
        break;
    case 2:
        QWidget::setStyleSheet ("#Widget {border-image: url(:/pho/bg2.png); } "
                               "#Widget * { border-image:url(); }");
        break;
    case 3:
        QWidget::setStyleSheet ("#Widget {border-image: url(:/pho/bg3.png); } "
                               "#Widget * { border-image:url(); }");
        break;
    case 4:
        QWidget::setStyleSheet ("#Widget {border-image: url(:/pho/bg4.png); } "
                               "#Widget * { border-image:url(); }");
        break;
    case 5:
        QWidget::setStyleSheet ("#Widget {border-image: url(:/pho/bg5.png); } "
                               "#Widget * { border-image:url(); }");
        break;
    case 6:
        QWidget::setStyleSheet ("#Widget {border-image: url(:/pho/bg6.png); } "
                               "#Widget * { border-image:url(); }");
        break;
    case 7:
        QWidget::setStyleSheet ("#Widget {border-image: url(:/pho/bg7.png); } "
                               "#Widget * { border-image:url(); }");
        break;
    case 8:
        QWidget::setStyleSheet ("#Widget {border-image: url(:/pho/bg8.png); } "
                               "#Widget * { border-image:url(); }");
        break;

    case 9:
        QWidget::setStyleSheet ("#Widget {border-image: url(:/pho/bg9.png); } "
                               "#Widget * { border-image:url(); }");
        break;
    case 10:
        QWidget::setStyleSheet ("#Widget {border-image: url(:/pho/bg10.png); } "
                               "#Widget * { border-image:url(); }");
        break;
    case 11:
        QWidget::setStyleSheet ("#Widget {border-image: url(:/pho/bg11.png); } "
                               "#Widget * { border-image:url(); }");
        break;
    case 12:
        QWidget::setStyleSheet ("#Widget {border-image: url(:/pho/bg12.png); } "
                               "#Widget * { border-image:url(); }");
        break;
    case 13:
        QWidget::setStyleSheet ("#Widget {border-image: url(:/pho/bg13.png); } "
                               "#Widget * { border-image:url(); }");
        break;
    case 14:
        QWidget::setStyleSheet ("#Widget {border-image: url(:/pho/bg14.png); } "
                               "#Widget * { border-image:url(); }");
        break;
    default:
        break;
    }
}

void Widget::changeTime()
{
    // 获取系统当前时间
    QDateTime dateTime = QDateTime::currentDateTime();
    clock->display(dateTime.toString("HH:mm:ss"));
}

//随机数
int Widget::generateRandomNumber()
{
    return QRandomGenerator::global()->bounded(1, 15);
}
