#ifndef BOTTOM_LAYOUT
#define BOTTOM_LAYOUT
#include <QVBoxLayout>
#include <QPlainTextEdit>
#include "../../layouts/MyLayout.h"

class BottomLayout : public MyLayout<QVBoxLayout> {

    public:
        explicit BottomLayout(QWidget* parent = nullptr)
            : MyLayout<QVBoxLayout>(parent), textEdit(nullptr) {
            removeMargin();
        }
        void setActive() {
            this->textEdit->setFocus();
            this->textEdit->moveCursor(QTextCursor::End);
        }
        QPlainTextEdit* getTextEditor() {
            return textEdit;
        }
        void addTextEditor(QPlainTextEdit* textEdit) {
            if (this->textEdit == nullptr) {
                this->textEdit = textEdit;
                addWidget(this->textEdit);
                this->textEdit->setFocus();
                this->textEdit->moveCursor(QTextCursor::End);
            }
            else {
                QLayoutItem* item;
                QVBoxLayout* layout = getLayout();
                while ((item = layout->takeAt(0)) != nullptr) {
                    if (QWidget* widget = item->widget()) {
                        layout->removeWidget(widget);
                        widget->setParent(nullptr);
                    }
                    delete item;
                }
                this->textEdit = textEdit;
                addWidget(this->textEdit);
                this->textEdit->setFocus();
                this->textEdit->moveCursor(QTextCursor::End);

            }
        }
        void removeTextEditor(QPlainTextEdit* textEdit) {
            QLayoutItem* item;
            QVBoxLayout* layout = getLayout();
            while ((item = layout->takeAt(0)) != nullptr) {
                if (QWidget* widget = item->widget()) {
                    layout->removeWidget(widget);
                    widget->setParent(nullptr);
                    delete widget;
                }
                delete item;
            }
            this->textEdit = textEdit;
            addWidget(this->textEdit);
            this->textEdit->setFocus();
            this->textEdit->moveCursor(QTextCursor::End);
        }
        // void addConsoleButton() {
        //     if (this->console_button == nullptr) {
        //         qDebug()<<parentWidget();
        //         console_button = new QPushButton(parentWidget());
        //         console_button->setIcon(QIcon(":/icons/js.png"));
        //         console_button->setIconSize(QSize(32, 32));
        //         console_button->setFixedSize(32, 32);
        //         console_button->raise();
        //         console_button->show();
        //     }
        // }
    protected:
        void resizeEvent(QResizeEvent* event) override {
            // MyLayout<QVBoxLayout>::resizeEvent(event);
            // int x = width() - console_button->width();
            // int y = height()/2;
            // console_button->move(x, y);
        }
    private:
        QPlainTextEdit *textEdit;
};

#endif