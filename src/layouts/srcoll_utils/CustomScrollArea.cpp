#include "CustomScrollArea.h"

CustomScrollArea::CustomScrollArea(QWidget* parent) : QScrollArea(parent), arrowsShown(false), preventHideFilter(false)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setFrameShape(QFrame::NoFrame);
    setStyleSheet("QScrollArea { border: none; }");
}

void CustomScrollArea::scrollHorizontally(int value)
{
    int newValue = horizontalScrollBar()->value() + value;
    this->horizontalScrollBar()->setValue(newValue);
}

bool CustomScrollArea::isArrowsShown()
{
    return this->arrowsShown;
}