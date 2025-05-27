#ifndef FILE_WIDGET_H
#define FILE_WIDGET_H
#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QApplication>
#include <QStyle>
#include <QFrame>
#include <QStyleOption>
#include <QPainter>
#include <QMouseEvent>
class FileWidget : public QFrame {
    Q_OBJECT
    public:
        explicit FileWidget(QString filename, int closeIndex, int itemWidth, int shownIndex, QWidget *parent = nullptr);
        void setActive();
        void setUnactive();
        void setWidth(int width) {
            setFixedWidth(width);
            itemWidth = width;
        }
        int getWidth() {
            return itemWidth;
        }
        void setCloseIndex(int closeIndex){
            this->closeIndex = closeIndex;
        }
        int getCloseIndex() const;
        int getShownIndex() const;
        bool getIsActive() const;
        QPushButton *closeButton;
        int getUniqueId() const;
    protected:
        void paintEvent(QPaintEvent *event);
        void mousePressEvent(QMouseEvent* event) override;
    private slots:
        void onCloseButtonClicked();
    private:
        QString filename;
        int itemWidth;
        int closeIndex;
        bool isActive;
        int shownIndex;
        uint64_t uniqueId;
        static uint64_t nextId;
    signals:
        void closeButtonSignal(int closeIndex);
        void widgetClickedSignal(int closeIndex, uint64_t uniqueId);
};

#endif