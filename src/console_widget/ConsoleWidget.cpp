#include "ConsoleWidget.h"

ConsoleWidget::ConsoleWidget(QWidget* parent) : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground);
    setStyleSheet(R"(
        ConsoleWidget {
            background-color: rgb(240, 234, 234);
            border: 2px solid rgb(180, 180, 180);
            border-radius: 4px;
        }
    )");

    show(); 
    raise();
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QWidget* topRowWidget = new QWidget();
    QHBoxLayout* topRowLayout = new QHBoxLayout(topRowWidget);
    topRowLayout->setContentsMargins(0, 0, 0, 0);
    topRowLayout->setSpacing(0);

    QLabel* spacerLeft = new QLabel();
    QLabel* spacerRight = new QLabel();

    // Actual label (centered visually)
    QLabel* editorTitle = new QLabel("JavaScript Editor");
    editorTitle->setStyleSheet(R"(
        font-weight: bold;
        font-size: 12pt;
    )");
    editorTitle->setAlignment(Qt::AlignCenter);

    // Close button (aligned right)
    QPushButton* closeButton = new QPushButton();
    QIcon closeIcon = style()->standardIcon(QStyle::SP_TitleBarCloseButton);
    closeButton->setIcon(closeIcon);
    closeButton->setFixedSize(24, 24);
    closeButton->setIconSize(QSize(16, 16));
    closeButton->setCursor(Qt::PointingHandCursor);
    closeButton->setStyleSheet("border: none; background: transparent;");

    // Layout trick: place 3 columns — left spacer, center label, right close button
    topRowLayout->addWidget(spacerLeft, 1);     // stretch
    topRowLayout->addWidget(editorTitle, 0);    // label in center
    topRowLayout->addWidget(spacerRight, 1);
    topRowLayout->addWidget(closeButton, 1);    // right aligned due to stretch

    mainLayout->addWidget(topRowWidget);

    QVBoxLayout* editorLayout = new QVBoxLayout();
    editorLayout->setSpacing(0);
    editorLayout->setContentsMargins(0, 0, 0, 0);
    
    QPlainTextEdit* editor = new QPlainTextEdit();
    editor->setStyleSheet("background-color: rgb(40, 40, 40);; color: white; border: none; font-size: 12pt;");
    editorLayout->addWidget(editor);

    mainLayout->addLayout(editorLayout, 5);

    outputLayout = new QVBoxLayout();
    outputLayout->setContentsMargins(0, 0, 0, 0);
    textEdit = new QTextEdit;
    // textEdit->setText("This is some read-only text.");
    textEdit->setReadOnly(true);
    textEdit->setStyleSheet("background-color: rgb(240, 234, 234); color: black; border: none; font-size: 12pt;");
    textEdit->hide();
    outputLayout->addWidget(textEdit);
    mainLayout->addLayout(outputLayout, 1);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    QPushButton* execute = new QPushButton("Execute");
    execute->setStyleSheet("font-size: 14pt; font-weight: bold;");
    execute->setCursor(Qt::PointingHandCursor);
    buttonLayout->addWidget(execute);
    mainLayout->addLayout(buttonLayout, 1);

    WidgetCoords coords = calcWidgetCoords();
    QTimer::singleShot(0, this,[this, coords, editor] {
        this->setGeometry(coords.x, coords.y, coords.newWidth, coords.newHeight);
        editor->setFocus();
    });
    
    QObject::connect(execute, &QPushButton::clicked, [this, editor]{
        QString scriptSource = editor->toPlainText();
        QByteArray utf8Script = scriptSource.toUtf8();
        std::string scriptStdString(utf8Script.constData(), utf8Script.size());
        if (!scriptStdString.empty()){
            emit sendScriptForExec(scriptStdString);
        }
    });
}

void ConsoleWidget::showConsoleWidgetHandler()
{
    qDebug()<<"showConsoleWidgetHandler";

}

void ConsoleWidget::resizeTopWindowHandler(int width, int height)
{   
    WidgetCoords coords = calcWidgetCoords();
    this->setGeometry(coords.x, coords.y, coords.newWidth, coords.newHeight);
}

void ConsoleWidget::setTextEditContent(const std::string &content)
{ 
    QString qResult = QString::fromUtf8(content.c_str(), static_cast<int>(content.size()));
    textEdit->setPlainText(qResult);
    outputLayout->setContentsMargins(2, 0, 2, 0);
    textEdit->show();
}

WidgetCoords ConsoleWidget::calcWidgetCoords()
{
    if (!parentWidget()) return {0, 0, 0, 0};

    QSize parentSize = parentWidget()->size();
    int parentW = parentSize.width();
    int parentH = parentSize.height();

    if (parentW == 0 || parentH == 0) {
        // Avoid division by zero or invalid size
        return {0, 0, 400, 200};
    }

    // Calculate aspect ratio of parent
    float aspectRatio = float(parentW) / float(parentH);

    // Scale factor for smaller size (e.g., 60%)
    const float scale = 0.4f;
    const float heightScale = 0.55f;
    // Start by scaling width and compute height from aspect ratio
    int w = static_cast<int>(parentW * scale);
    int h = static_cast<int>(parentH * heightScale);

    // Calculate top-left corner to center the widget
    int x = (parentW - w) / 2;
    int y = (parentH - h) / 2;

    return {x, y, w, h};
}