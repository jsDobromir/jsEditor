#ifndef SCROLL_CONTAINER_H
#define SCROLL_CONTAINER_H
#include "CustomScrollArea.h"
#include "../file_widget/FileWidget.h"
#include "../arrow_container/Arrow.h"
#include <QVBoxLayout>
#include <QBoxLayout>
#include <QWidget>
#include <vector>
#include <QTimer>

class ScrollContainer : public QWidget{ 
    Q_OBJECT
    public:
        enum SCROLL_TRIGGERS {
            NONE,
            ADD_ITEM,
            CLOSE_ITEM
        };
        explicit ScrollContainer(QWidget* parent = nullptr);
        CustomScrollArea* getScrollArea();
        uint64_t addFileCont(QString filename);
        const std::vector<FileWidget*>& getFileWidgets() const;
        void updateVisibleItems();
        void updateVisibleItemsInArea();
        void updateVisibleItemsInAreaRightArrow(int lastIndex);
        QBoxLayout* getInnerLayout();
        int getActiveIndex() const;
        void setActiveIndex(int activeIndex);
    public slots:
        void closeButtonHandler(int closeIndex);
        void arrowButtonSignalHandler(ARROW_DIRECTION direction);
        void widgetClickedHandler(int closeIndex, uint64_t uniqueId);
    private:
        int getShownIndex();
        CustomScrollArea* scrollArea;
        QBoxLayout* innerLayout;
        QWidget* contentWidget;
        int contentWidgetWidth;
        int activeIndex;
        std::vector<FileWidget*> fileWidgets;
        SCROLL_TRIGGERS currentTrigger;
        bool scrollFromArrow;
        std::map<int, bool> indexMap;
    signals:
        void horizontalScrollbarNeeded();
        void horizontalScrollbarNeededHide();
        void closeButtonTextEditSignal(uint64_t fileIndex, bool isActiveIndexClosed, bool isLastIndex);
        void widgedClickedMapSignal(uint64_t uniqueId);
};

#endif