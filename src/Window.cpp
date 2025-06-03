#include "Window.h"

Window::Window(QWidget* parent, int w, int h) : QWidget(parent), w(w), h(h), mainLayout(new QVBoxLayout())
{
    this->resize(w, h);
    this->setWindowTitle(" ");
    this->showMaximized();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    this->setLayout(mainLayout);
    // this->show();
}

int Window::getWidth() const {
    return w;
}
int Window::getHeight() const {
    return h;
}
void Window::mousePressEvent(QMouseEvent* event) {
}
void Window::addChildLayout(QWidget *widget, int scale){
    mainLayout->addWidget(widget, scale);
}
QVBoxLayout* Window::getLayout() {
    return this->mainLayout;
}
void Window::mouseMoveEvent(QMouseEvent* event) {
}
void Window::mouseReleaseEvent(QMouseEvent* event) {
}
void Window::paintEvent(QPaintEvent *event){
}

void Window::showWindow()
{
    this->show();
}

void Window::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    emit emitResizeSignal(width(), height());
}
