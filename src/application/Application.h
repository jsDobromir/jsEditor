#ifndef APPLICATION_H
#define APPLICATION_H
#include <QObject>
#include <QDebug>
#include <QPlainTextEdit>
#include <QPointer>
#include <QPushButton>
#include <QThread>
#include <QString>
#include "../Window.h"
#include "../console_widget/ConsoleWidget.h"
#include "../structure/top_layout/TopLayout.h"
#include "../structure/mid_layout/MidLayout.h"
#include "../structure/bottom_layout/BottomLayout.h"
#include "ActiveTab.h"
#include "../v8_stuff/V8_Exec.h"

class V8_Exec;

class Application : public QObject {
    Q_OBJECT
    public:
        explicit Application(Window *win, int argc, char* argv[]);
        void setLayouts(TopLayout* topLayout, MidLayout* midLayout, BottomLayout* bottomLayout, int argc, char* argv[]);
        const std::map<uint64_t, QPointer<QPlainTextEdit>>& getTextEditGlobalMap() const;
        void addConsoleButton();
        void addActiveTab(FileInfo& fileInfo);
        ~Application();
    public slots:
        void addDocumentAndTextEditHandler(FileInfo fileInfo);
        void removeDocumentAndTextEditHandler(uint64_t fileIndex, bool isActiveIndexClosed ,bool isLastIndex);
        void widgetClickedHanlder(uint64_t uniqueId);
        void resizeHandler(int width, int height);
        void handleScriptResult(const std::string& result);
        void recieveScriptForExec(const std::string& script);
        QString cb(const QString& str);
        std::vector<ActiveTab> getAllOpenTabs();
    signals:
        void runScript(const std::string &script);
    private:
        TopLayout* topLayout;
        MidLayout* midLayout;
        BottomLayout* bottomLayout;
        std::map<uint64_t, QPointer<QPlainTextEdit>> textEditGlobalMap;
        static bool console_init;
        Window* window;
        QPushButton *console_button;
        std::unique_ptr<ConsoleWidget> consoleWidget;
        QThread* workerThread;
        V8_Exec* v8ExecWorker;
        ActiveTab activeTab;
};

#endif