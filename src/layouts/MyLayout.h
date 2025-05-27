#ifndef MY_LAYOUT_H
#define MY_LAYOUT_H
#include <QWidget>
#include <QLayout>

template<typename T>
class MyLayout : public QWidget {
    public:
        explicit MyLayout(QWidget* parent = nullptr);
        void addWidget(QWidget* widget);
        void removeMargin();
        void addAbsLayout(QLayout* absLayout);
        void setLayoutLocal(QLayout* layout);
        T* getLayout();
        void addSpacing(int amount);
        void setBackground(const QString& color);
    private:
        T* layoutPtr;
};

template<typename T>
MyLayout<T>::MyLayout(QWidget* parent): QWidget(parent), layoutPtr(new T())
{
    setLayout(layoutPtr);
}

template<typename T>
void MyLayout<T>::addWidget(QWidget* widget) {
    layoutPtr->addWidget(widget);
}

template<typename T>
void MyLayout<T>::removeMargin() {
    layoutPtr->setContentsMargins(0, 0, 0, 0);
    layoutPtr->setSpacing(0);
}

template<typename T>
void MyLayout<T>::setLayoutLocal(QLayout* layout) {
    setLayout(layout);
}

template<typename T>
void MyLayout<T>::addAbsLayout(QLayout* absLayout) {
    layoutPtr->addLayout(absLayout);
}

template<typename T>
T* MyLayout<T>::getLayout() {
    return layoutPtr;
}

template<typename T>
void MyLayout<T>::addSpacing(int amount) {
    layoutPtr->addSpacing(amount);
}

template<typename T>
void MyLayout<T>::setBackground(const QString& color) {
    setStyleSheet(color);
}

#endif