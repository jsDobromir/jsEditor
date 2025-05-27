#ifndef BUTTON_H
#define BUTTON_H
#include <QPushButton>

namespace Custom_Button {

    using Callback = std::function<void()>;

    class Button : public QPushButton {
        Q_OBJECT
        public:
            explicit Button(const QString& text, Callback cb, QWidget* parent = nullptr)
            : QPushButton(text, parent)
            {
                QObject::connect(this, &QPushButton::clicked, [this, cb]{ cb(); });
            }
    };
}; 

#endif