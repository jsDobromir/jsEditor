#ifndef MID_LAYOUT_H
#define MID_LAYOUT_H
#include "../../layouts/MyLayout.h"
#include "../../layouts/srcoll_utils/ScrollContainer.h"
#include "../../layouts/arrow_container/Arrow.h"
#include <QHBoxLayout>
#include <QPushButton>

class MidLayout : public MyLayout<QHBoxLayout> {
    Q_OBJECT
    public:
        explicit MidLayout(QWidget* parent = nullptr);
        void addArrows();
        void hideArrows();
        ScrollContainer* getScrollArea();
    public slots:
        void newDocumentCreated(QString filename);
    private:
        ScrollContainer *scrollArea;
        int arrowWidth;
        Arrow *leftArrow;
        Arrow *rightArrow;
    signals:
        void addDocumentAndTextEdit(FileInfo fileInfo);
        void removeDocumentAndTextEdit(uint64_t fileIndex, bool isActiveIndexClosed, bool isLastIndex);
        void widgedClickedMapSignalReThrow(uint64_t uniqueId);
};

#endif