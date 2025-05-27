#ifndef ARROW_H
#define ARROW_H
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QApplication>
#include <QStyle>
#include <QIcon>

enum ARROW_DIRECTION {
    LEFT,
    RIGHT
};

class Arrow : public QWidget {
    Q_OBJECT
    public:
        explicit Arrow(QWidget* parent = nullptr, ARROW_DIRECTION direction = ARROW_DIRECTION::LEFT);
        void showButton();
        void hideButton();
        bool isArrowHidden();
    private:
        ARROW_DIRECTION direction;
        QPushButton *arrowButton;
        bool arrowHidden;
        QIcon transparentIcon;
        QIcon icon;
    signals:
        void arrowButtonSignal(ARROW_DIRECTION direction);
};

#endif