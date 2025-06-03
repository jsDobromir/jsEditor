#ifndef V8_EXEC_H
#define V8_EXEC_H
#include <v8.h>
#include <libplatform/libplatform.h>
#include <QDebug>
#include <QObject>
#include <iostream>
#include "../application/Application.h"
#include "activeTabUtils/activeTab.h"

class Application;

class V8_Exec : public QObject{
    Q_OBJECT
    public:
        V8_Exec(int argc, char** argv, Application* application);
        ~V8_Exec();
        v8::Local<v8::ObjectTemplate> Initialize();
        static void logCallback(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void getAllTabs(const v8::FunctionCallbackInfo<v8::Value>& args);
        static v8::Local<v8::ObjectTemplate> getTabTemplate();
        void appendToOutput(std::string &str);
    signals:
        void scriptResultReady(const std::string& result);
    public slots:
        void initializeV8();
        void executeScript(const std::string &scriptToExecute);
        Q_INVOKABLE void updateActiveTab(ActiveTab tab);
    private:
        Application* application;
        ActiveTab activeTab;
        v8::Isolate::CreateParams create_params;
        std::unique_ptr<v8::Platform> platform;
        v8::Isolate* isolate;
        std::string lastOutput;
        std::vector<ActiveTab> openTabs;
        v8::Global<v8::ObjectTemplate> activeTabTemplate;
        int argc;
        char** argv;
};

#endif