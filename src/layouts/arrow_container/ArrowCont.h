#ifndef LEFT_ARROW_CONT
#define LEFT_ARROW_CONT
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QApplication>
#include <QStyle>



class ArrowCont : public QWidget{
    Q_OBJECT
    public:
        explicit ArrowCont(QWidget *parent = nullptr, Qt::Alignment alignment = Qt::AlignLeft);
        void showArrow();
        void hideArrow();
        bool isArrowHidden();
        QPushButton* getButton();
    private:
        Qt::Alignment alignment;
        QPushButton *arrowButton;
    signals:
        void arrowButtonSignal(Qt::Alignment alignment);
};

#endif