#ifndef TIMEDISPLAY_H
#define TIMEDISPLAY_H
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class TimeDisplay; }
QT_END_NAMESPACE

class TimeDisplay : public QMainWindow
{
    Q_OBJECT

public:
    TimeDisplay(QWidget *parent = nullptr);
    ~TimeDisplay();

private:
    Ui::TimeDisplay *ui;
public slots:
    void timerUpdata(void);
};

#endif // TIMEDISPLAY_H
