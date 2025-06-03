#include "ScrollContainer.h"
#include <QPushButton>

ScrollContainer::ScrollContainer(QWidget* parent) : QWidget(parent), scrollArea(new CustomScrollArea()), innerLayout(new QBoxLayout(QBoxLayout::LeftToRight)), activeIndex(-1), currentTrigger(SCROLL_TRIGGERS::NONE), scrollFromArrow(false)
{
    contentWidget = new QWidget();
    innerLayout->setContentsMargins(0, 0, 0, 0);
    contentWidget->setLayout(innerLayout);
    scrollArea->setWidget(contentWidget);
    scrollArea->setWidgetResizable(true);

    QVBoxLayout* contentLayout = new QVBoxLayout(this);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->addWidget(scrollArea, Qt::AlignCenter);
    this->setLayout(contentLayout);

    auto connection = std::make_shared<QMetaObject::Connection>();
    *connection = connect(scrollArea->horizontalScrollBar(), &QScrollBar::rangeChanged,
        this, [this, connection](int, int) {
            if (scrollFromArrow) {
                scrollFromArrow = false;
                return;
            }
            emit horizontalScrollbarNeeded();

            int viewportWidth = scrollArea->viewport()->width();
            int accumulatedWidth = 0;
            if (this->activeIndex == (fileWidgets.size() - 1) ){
                updateVisibleItems();
            }
            else {
                updateVisibleItemsInArea();
            }

    });
}

CustomScrollArea* ScrollContainer::getScrollArea() {
    return this->scrollArea;
}

FileInfo ScrollContainer::addFileCont(QString filename)
{   
    int itemIndex = fileWidgets.size();
    int itemMinWidth = 140;
    int shownIndex = getShownIndex();
    FileWidget *fileWidget = new FileWidget(filename, itemIndex, itemMinWidth, shownIndex);
    fileWidget->setActive();
    this->innerLayout->addWidget(fileWidget);
    fileWidgets.push_back(fileWidget);
    
    for(int i=0; i < fileWidgets.size();i++) {
        if (i != itemIndex) {
            fileWidgets[i]->setUnactive();
        }
    }
    
    this->scrollFromArrow = false;
    this->activeIndex = fileWidgets.size()-1;
    QObject::connect(fileWidget, &FileWidget::closeButtonSignal, this, &ScrollContainer::closeButtonHandler);
    QObject::connect(fileWidget, &FileWidget::widgetClickedSignal, this, &ScrollContainer::widgetClickedHandler);
    FileInfo fileInfo(false, fileWidget->getUniqueId(), fileWidget->getFileName());
    return fileInfo;
}

void ScrollContainer::closeButtonHandler(int closeIndex)
{   
    if (fileWidgets.size()<=1) return;
    bool isLastIndex = (closeIndex == fileWidgets.size() - 1);
    FileWidget* fileWidgetToDel = fileWidgets[closeIndex];
    int shownIndex = fileWidgetToDel->getShownIndex();
    int uniqueId = fileWidgetToDel->getUniqueId();
    delete fileWidgetToDel;
    fileWidgets.erase(fileWidgets.begin()+closeIndex);
    
    for (int i = 0; i < fileWidgets.size(); i++) {
        fileWidgets[i]->setCloseIndex(i);
    }
    this->indexMap[shownIndex] = false;
    bool isActiveIndexClosed = (this->activeIndex == closeIndex);
    if (isActiveIndexClosed) {
        if (isLastIndex) {
            this->activeIndex = fileWidgets.size()-1;
        }
        else{
            this->activeIndex = closeIndex;
        }
        FileWidget* lastElement = fileWidgets[this->activeIndex];
        lastElement->setActive();
        for (int i=0; i<fileWidgets.size();i++) {
            if (i != activeIndex) {
                fileWidgets[i]->setUnactive();
            }
        }
    }

    int totalWidth = 0;
    int activeCloseIndex = -1;
    for (FileWidget* widget : fileWidgets) {
        totalWidth += widget->width();
        if (widget->getIsActive()) {
            activeCloseIndex = widget->getCloseIndex();
        }
    }
    this->activeIndex = activeCloseIndex;
    if (totalWidth < (scrollArea->viewport()->width()) ) {
        emit horizontalScrollbarNeededHide();
    }
    if (this->activeIndex == (fileWidgets.size() - 1) ){
        updateVisibleItems();
    }
    else {
        updateVisibleItemsInArea();
    }
    emit closeButtonTextEditSignal(uniqueId, isActiveIndexClosed, isLastIndex);
}

void ScrollContainer::widgetClickedHandler(int closeIndex, uint64_t uniqueId)
{   
    fileWidgets[this->activeIndex]->setUnactive();
    fileWidgets[closeIndex]->setActive();
    this->activeIndex = closeIndex;
    emit widgedClickedMapSignal(uniqueId);
}

const std::vector<FileWidget*>& ScrollContainer::getFileWidgets() const {
    return fileWidgets;
}

QBoxLayout* ScrollContainer::getInnerLayout() {
    return this->innerLayout;
}

void ScrollContainer::updateVisibleItems()
{
    if (!scrollArea || !scrollArea->viewport()) return;

    // Step 1: Show all widgets initially
    for (FileWidget* widget : fileWidgets) {
        widget->setVisible(true);
    }

    int viewportWidth = scrollArea->viewport()->width();
    int accumulatedWidth = 0;

    // Step 2: Show widgets that fit from the end (right to left)
    for (int i = fileWidgets.size() - 1; i >= 0; --i) {
        FileWidget* widget = fileWidgets[i];
        int widgetWidth = widget->width();

        if (accumulatedWidth + widgetWidth <= viewportWidth) {
            widget->setVisible(true);
            accumulatedWidth += widgetWidth;
        } else {
            widget->setVisible(false);
        }
    }
}

void ScrollContainer::updateVisibleItemsInArea()
{
    if (!scrollArea || !scrollArea->viewport()) return;
    if (activeIndex < 0 || activeIndex >= fileWidgets.size()) return;

    // Step 1: Show all widgets initially
    for (int i = 0; i < fileWidgets.size(); i++) {
        fileWidgets[i]->setVisible(true);
    }

    int viewportWidth = scrollArea->viewport()->width();
    int accumulatedWidth = 0;

    for (int i = activeIndex; i < fileWidgets.size(); i++) {
        FileWidget* widget = fileWidgets[i];
        int widgetWidth = widget->width();
        if (accumulatedWidth + widgetWidth <= viewportWidth) {
            widget->setVisible(true);
            accumulatedWidth += widgetWidth;
        } else {
            widget->setVisible(false);
        }
    }
    if (accumulatedWidth < viewportWidth) {
        for (int i = activeIndex-1; i>=0;i--) {
            FileWidget* widget = fileWidgets[i];
            int widgetWidth = widget->width();
            if (accumulatedWidth + widgetWidth <= viewportWidth) {
                widget->setVisible(true);
                accumulatedWidth += widgetWidth;
            } else {
                widget->setVisible(false);
            }
        }
    }
}

void ScrollContainer::updateVisibleItemsInAreaRightArrow(int lastIndex) {
    if (!scrollArea || !scrollArea->viewport()) return;
    if (activeIndex < 0 || activeIndex >= fileWidgets.size()) return;

    // Step 1: Show all widgets initially
    for (int i = 0; i < fileWidgets.size(); i++) {
        fileWidgets[i]->setVisible(true);
    }

    int viewportWidth = scrollArea->viewport()->width();
    int accumulatedWidth = 0;

    for (int i = lastIndex; i>= 0; --i) {
        FileWidget* widget = fileWidgets[i];
        int widgetWidth = widget->width();
        if (accumulatedWidth + widgetWidth <= viewportWidth) {
            widget->setVisible(true);
            accumulatedWidth += widgetWidth;
        } else {
            widget->setVisible(false);
        }
    }
    if (accumulatedWidth < viewportWidth) {
        for (int i = lastIndex+1; i<fileWidgets.size();i++) {
            FileWidget* widget = fileWidgets[i];
            int widgetWidth = widget->width();
            if (accumulatedWidth + widgetWidth <= viewportWidth) {
                widget->setVisible(true);
                accumulatedWidth += widgetWidth;
            } else {
                widget->setVisible(false);
            }
        }
    }
}

void ScrollContainer::arrowButtonSignalHandler(ARROW_DIRECTION direction) {
    if (direction == ARROW_DIRECTION::RIGHT) {
        if(activeIndex>-1 && activeIndex<fileWidgets.size()-1) {
            this->activeIndex++;
            FileWidget* lastElement = fileWidgets[this->activeIndex];
            lastElement->setActive();
            for (FileWidget* ptr : fileWidgets) {
                if (ptr != lastElement) {
                    ptr->setUnactive();
                }
            }
            this->scrollFromArrow = true;
            if (!lastElement->isVisible()){
                updateVisibleItemsInAreaRightArrow(activeIndex);
            }
            QTimer::singleShot(0, this, [this, lastElement]() {
                QPoint targetPos = lastElement->mapTo(scrollArea->widget(), QPoint(0, 0));
                scrollArea->horizontalScrollBar()->setValue(targetPos.x());
            });
        }
    }
    else {
        if (activeIndex>0) {
            this->activeIndex--;
            FileWidget* firstElement = fileWidgets[this->activeIndex];
            firstElement->setActive();
            for (FileWidget* ptr : fileWidgets) {
                if (ptr != firstElement) {
                    ptr->setUnactive();
                }
            }
            this->scrollFromArrow = true;
            updateVisibleItemsInArea();
            QTimer::singleShot(0, this, [this, firstElement]() {
                QPoint targetPos = firstElement->mapTo(scrollArea->widget(), QPoint(0, 0));
                scrollArea->horizontalScrollBar()->setValue(targetPos.x());
            });
        }
    }
}

int ScrollContainer::getShownIndex()
{
    if (indexMap.empty()) {
        indexMap[1] = true;
        auto it = std::prev(indexMap.end());
        return (it->first);
    }
    for (const auto& [key, value] : indexMap) {
        if (value == false) {
            indexMap[key] = true;
            return key;
        }
    }
    indexMap[fileWidgets.size()+1] = true;
    auto it = std::prev(indexMap.end());
    return (it->first);
}

int ScrollContainer::getActiveIndex() const
{
    return this->activeIndex;
}

void ScrollContainer::setActiveIndex(int activeIndex)
{
    this->activeIndex = activeIndex;
}