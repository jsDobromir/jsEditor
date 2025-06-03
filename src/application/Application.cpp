#include "Application.h"

bool Application::console_init = false;

Application::Application(Window *win, int argc, char* argv[]) : window(win), topLayout(nullptr), midLayout(nullptr), bottomLayout(nullptr), console_button(nullptr) {
    workerThread = new QThread(this);
    v8ExecWorker = new V8_Exec(argc, argv, this);
    v8ExecWorker->moveToThread(workerThread);
    connect(workerThread, &QThread::started, v8ExecWorker, &V8_Exec::initializeV8);
    connect(this, &Application::runScript, v8ExecWorker, &V8_Exec::executeScript);
    connect(v8ExecWorker, &V8_Exec::scriptResultReady, this, &Application::handleScriptResult);
    workerThread->start();
    qRegisterMetaType<ActiveTab>("ActiveTab");
    qRegisterMetaType<std::vector<ActiveTab>>("std::vector<ActiveTab>");
}

void Application::addDocumentAndTextEditHandler(FileInfo fileInfo)
{
    qDebug()<<fileInfo.uniqueId;
    QPlainTextEdit* textEdit = new QPlainTextEdit();
    textEdit->setLineWrapMode(QPlainTextEdit::LineWrapMode::NoWrap);
    textEditGlobalMap[fileInfo.uniqueId] = QPointer<QPlainTextEdit>(textEdit);
    this->bottomLayout->addTextEditor(textEdit);
    if (!Application::console_init) {
        this->addConsoleButton();
        Application::console_init = true;
    }
    this->addActiveTab(fileInfo);
}

void Application::widgetClickedHanlder(uint64_t uniqueId)
{
    auto el = textEditGlobalMap.find(uniqueId);
    this->bottomLayout->addTextEditor(el->second);
}

void Application::removeDocumentAndTextEditHandler(uint64_t fileIndex, bool isActiveIndexClosed, bool isLastIndex)
{
    if (isActiveIndexClosed) {
        if (isLastIndex) {
            auto el = textEditGlobalMap.find(fileIndex);
            if (el != textEditGlobalMap.begin()) {
                --el;
                this->bottomLayout->removeTextEditor(el->second);
                auto origEl = textEditGlobalMap.find(fileIndex);
                textEditGlobalMap.erase(fileIndex);
            }
        }
        else {
            auto el = textEditGlobalMap.find(fileIndex);
            if (el != textEditGlobalMap.end()) {
                ++el;
                this->bottomLayout->removeTextEditor(el->second);
                textEditGlobalMap.erase(fileIndex);
            }
        }
    }
    else {
        textEditGlobalMap.erase(fileIndex);
        this->bottomLayout->setActive();
    }
}

void Application::setLayouts(TopLayout* topLayout, MidLayout* midLayout, BottomLayout* bottomLayout, int argc, char* argv[])
{
    this->topLayout = topLayout;
    this->midLayout = midLayout;
    this->bottomLayout = bottomLayout;
    this->topLayout->newDocumentMethod(QString("New Document"));
    this->consoleWidget = std::make_unique<ConsoleWidget>(window);
    connect(this->consoleWidget.get(), &ConsoleWidget::sendScriptForExec, this, &Application::recieveScriptForExec);
}

const std::map<uint64_t, QPointer<QPlainTextEdit>>& Application::getTextEditGlobalMap() const
{
    return textEditGlobalMap;
}

void Application::resizeHandler(int width, int height)
{
    int x = width - console_button->width();
    int y = height/2;
    console_button->move(x, y);
    console_button->show();
}

void Application::addConsoleButton()
{
    console_button = new QPushButton(this->window);
    console_button->setIcon(QIcon(":/icons/js.png"));
    console_button->setIconSize(QSize(32, 32));
    console_button->setFixedSize(32, 32);
    console_button->raise();
    console_button->hide();
}

void Application::handleScriptResult(const std::string& result)
{
    this->consoleWidget.get()->setTextEditContent(result);
}

void Application::recieveScriptForExec(const std::string& script)
{
    emit runScript(script);
}

QString Application::cb(const QString& str)
{
    auto it = textEditGlobalMap.find(0);
    QString textContent = it->second->toPlainText();
    return textContent;
}

std::vector<ActiveTab> Application::getAllOpenTabs()
{
    qDebug()<<"getAllOpenTabs";
    std::vector<ActiveTab> activeTabs;
    activeTabs.push_back(activeTab);
    ActiveTab tab2;
    tab2.isFile = false;
    tab2.name = "NewDocument1";
    tab2.index = 1;
    tab2.textContent = "content in document 2";
    activeTabs.push_back(tab2);
    return activeTabs;
}

void Application::addActiveTab(FileInfo& fileInfo)
{
    auto it = textEditGlobalMap.find(fileInfo.uniqueId);
    QString textContent = it->second->toPlainText();
    ActiveTab activeTabLocal;
    activeTabLocal.isFile = fileInfo.isFile;
    activeTabLocal.index = fileInfo.uniqueId;
    activeTabLocal.name = fileInfo.fileName;
    activeTabLocal.textContent = textContent;
    this->activeTab = std::move(activeTabLocal);

    QMetaObject::invokeMethod(v8ExecWorker, "updateActiveTab",
        Qt::BlockingQueuedConnection,
        Q_ARG(ActiveTab, activeTab));
}

Application::~Application()
{
    connect(workerThread, &QThread::finished, v8ExecWorker, &QObject::deleteLater);
    workerThread->quit();
    workerThread->wait();
}