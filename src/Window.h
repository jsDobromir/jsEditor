#ifndef WINDOW_H
#define WINDOW_H
#include <QPainter>
#include <QWidget>
#include <QDebug>
#include <QMouseEvent>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QApplication>

class Window : public QWidget {
    Q_OBJECT
    public:
        Window(QWidget* parent, int w, int h);
        int getWidth() const;
        int getHeight() const;
        void addChildLayout(QWidget *widget, int scale);
        void setTextEditWidg(QTextEdit* textEditPtr);
        void setFocusTextEdit();
        QVBoxLayout* getLayout();
        void showWindow();
    protected:
        void paintEvent(QPaintEvent *event) override;
        void mousePressEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;
        void resizeEvent(QResizeEvent* event) override;
    private:
        int w;
        int h;
        QVBoxLayout *mainLayout;
    signals:
        void emitResizeSignal(int width, int height);
};

#endif