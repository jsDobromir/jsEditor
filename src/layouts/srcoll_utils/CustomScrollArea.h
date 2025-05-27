#ifndef CUSTOM_SCROLL_AREA_H
#define CUSTOM_SCROLL_AREA_H
#include <QScrollArea>
#include <QScrollBar>
#include <QEvent>
#include <QDebug>

class CustomScrollArea : public QScrollArea {
    Q_OBJECT
    public:
        explicit CustomScrollArea(QWidget *parent = nullptr);
        void scrollHorizontally(int value);
        bool isArrowsShown();
        void showArrows() {
            this->arrowsShown = true;
        }
        void hideArrows() {
            this->arrowsShown = false;
        }
        void resizeArea(int amount) {
            QWidget* widget = this->widget();
            widget->resize(amount, 20);
        }
        void setPreventHideFilter(bool prevent) {
            preventHideFilter = prevent;
        }
    protected:
        // bool eventFilter(QObject *obj, QEvent *event) override;
    private:
        bool arrowsShown;
        bool preventHideFilter;
    signals:
        void horizontalScrollbarNeeded();
        void horizontalScrollbarNeededHide();
};

#endif