#ifndef RANDL_BUTTON_H
#define RANDL_BUTTON_H

#include <QObject>
#include <QPushButton>
#include <QMouseEvent>

class RandLbtn : public QPushButton {
    Q_OBJECT
    using QPushButton::QPushButton;
signals:
    void leftButtonClicked();
    void rightButtonClicked();

protected:
    void mousePressEvent(QMouseEvent *event) {
        QPushButton::mousePressEvent(event);
        if (event->button() == Qt::LeftButton)
        {

            emit leftButtonClicked();        //左键
        }

        if (event->button() == Qt::RightButton)
        {
            emit rightButtonClicked();      //右键
        }
    }
};

#endif //RANDL_BUTTON_H
