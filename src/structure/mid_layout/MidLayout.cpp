#include "MidLayout.h"

MidLayout::MidLayout(QWidget* parent) : MyLayout<QHBoxLayout>(parent), scrollArea(new ScrollContainer())
{
    removeMargin();
    
    leftArrow = new Arrow(nullptr, ARROW_DIRECTION::LEFT);
    rightArrow = new Arrow(nullptr, ARROW_DIRECTION::RIGHT);

    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    QObject::connect(leftArrow, &Arrow::arrowButtonSignal, scrollArea, &ScrollContainer::arrowButtonSignalHandler);
    QObject::connect(rightArrow, &Arrow::arrowButtonSignal, scrollArea, &ScrollContainer::arrowButtonSignalHandler);
    
    getLayout()->addWidget(leftArrow, 1);
    getLayout()->addWidget(scrollArea, 50);
    getLayout()->addWidget(rightArrow, 1);

    QObject::connect(scrollArea, &ScrollContainer::horizontalScrollbarNeeded, [this]() {this->addArrows();});
    QObject::connect(scrollArea, &ScrollContainer::horizontalScrollbarNeededHide, [this]() {this->hideArrows();});
    QObject::connect(scrollArea, &ScrollContainer::closeButtonTextEditSignal, [this](int fileIndex, bool isActiveIndexClosed, bool isLastIndex) {
        emit removeDocumentAndTextEdit(fileIndex, isActiveIndexClosed, isLastIndex);
    });
    QObject::connect(scrollArea, &ScrollContainer::widgedClickedMapSignal, [this](uint64_t uniqueId) {
        emit widgedClickedMapSignalReThrow(uniqueId);
    });
}

void MidLayout::newDocumentCreated(QString filename)
{
    qDebug()<<"newDocumentCreated log";
    uint64_t fileIndex = scrollArea->addFileCont(filename);
    emit addDocumentAndTextEdit(fileIndex);
}

void MidLayout::addArrows()
{
    leftArrow->showButton();
    rightArrow->showButton();
}
void MidLayout::hideArrows()
{
    leftArrow->hideButton();
    rightArrow->hideButton();
}

ScrollContainer* MidLayout::getScrollArea()
{
    return this->scrollArea;
}