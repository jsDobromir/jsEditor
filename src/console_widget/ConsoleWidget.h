#ifndef CONSOLE_WIDGET_H
#define CONSOLE_WIDGET_H
#include <QWidget>
#include <QObject>
#include <QDebug>
#include <QSize>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QIcon>
#include <QStyle>
#include <QPlainTextEdit>
#include <QString>
#include <QByteArray>

struct WidgetCoords {
    int x;
    int y;
    int newWidth;
    int newHeight;
};

class ConsoleWidget : public QWidget {
    Q_OBJECT
    public:
        ConsoleWidget(QWidget* parent = nullptr);
        WidgetCoords calcWidgetCoords();
        void setTextEditContent(const std::string &content);
    public slots:
        void showConsoleWidgetHandler();
        void resizeTopWindowHandler(int width, int height);
    signals:
        void sendScriptForExec(const std::string &script);
    private:
        QWidget* container;
        QWidget* topWindow;
        QTextEdit* textEdit;
        QVBoxLayout* outputLayout;
};

#endif