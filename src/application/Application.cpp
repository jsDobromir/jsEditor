#include "Application.h"

bool Application::console_init = false;

Application::Application(Window *win) : window(win), topLayout(nullptr), midLayout(nullptr), bottomLayout(nullptr), console_button(nullptr) {
    
}

void Application::addDocumentAndTextEditHandler(uint64_t fileIndex)
{
    qDebug()<<fileIndex;
    QPlainTextEdit* textEdit = new QPlainTextEdit();
    textEdit->setLineWrapMode(QPlainTextEdit::LineWrapMode::NoWrap);
    textEditGlobalMap[fileIndex] = QPointer<QPlainTextEdit>(textEdit);
    this->bottomLayout->addTextEditor(textEdit);
    if (!Application::console_init) {
        this->addConsoleButton();
        Application::console_init = true;
    }
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

void Application::setLayouts(TopLayout* topLayout, MidLayout* midLayout, BottomLayout* bottomLayout)
{
    this->topLayout = topLayout;
    this->midLayout = midLayout;
    this->bottomLayout = bottomLayout;
    this->topLayout->newDocumentMethod(QString("New Document"));
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