#ifndef APPLICATION_H
#define APPLICATION_H
#include <QObject>
#include <QDebug>
#include <QPlainTextEdit>
#include <QPointer>
#include <QPushButton>
#include "../Window.h"
#include "../structure/top_layout/TopLayout.h"
#include "../structure/mid_layout/MidLayout.h"
#include "../structure/bottom_layout/BottomLayout.h"

class Application : public QObject {
    Q_OBJECT
    public:
        explicit Application(Window *win);
        void setLayouts(TopLayout* topLayout, MidLayout* midLayout, BottomLayout* bottomLayout);
        const std::map<uint64_t, QPointer<QPlainTextEdit>>& getTextEditGlobalMap() const;
        void addConsoleButton();
    public slots:
        void addDocumentAndTextEditHandler(uint64_t fileIndex);
        void removeDocumentAndTextEditHandler(uint64_t fileIndex, bool isActiveIndexClosed ,bool isLastIndex);
        void widgetClickedHanlder(uint64_t uniqueId);
        void resizeHandler(int width, int height);
    private:
        TopLayout* topLayout;
        MidLayout* midLayout;
        BottomLayout* bottomLayout;
        std::map<uint64_t, QPointer<QPlainTextEdit>> textEditGlobalMap;
        static bool console_init;
        Window* window;
        QPushButton *console_button;
};

#endif