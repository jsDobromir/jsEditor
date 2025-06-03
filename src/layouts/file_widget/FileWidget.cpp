#include "FileWidget.h"

uint64_t FileWidget::nextId = 0;

FileWidget::FileWidget(QString filename, int closeIndex, int itemWidth, int shownIndex, QWidget *parent) : QFrame(parent), filename(filename), closeIndex(closeIndex), itemWidth(itemWidth), isActive(false), shownIndex(shownIndex), uniqueId(nextId++)
{
    setMinimumWidth(itemWidth);

    setObjectName("FileWidget");
    setStyleSheet("#FileWidget { background-color: rgb(255, 254, 254);}");
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 15);
    
    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setAlignment(Qt::AlignCenter);
    QIcon closeIcon = QApplication::style()->standardIcon(QStyle::SP_TitleBarCloseButton);
    closeButton = new QPushButton(this);
    closeButton->setCursor(Qt::PointingHandCursor);
    closeButton->setIcon(closeIcon);
    closeButton->setFixedSize(15, 15);
    closeButton->setStyleSheet("border: none; background: transparent;");

    topLayout->addStretch();
    topLayout->addWidget(closeButton);
                
    QVBoxLayout* labelLayout = new QVBoxLayout();
    labelLayout->setAlignment(Qt::AlignCenter);
    this->filename = filename + QString::number(shownIndex);
    QLabel* label = new QLabel(this->filename, this);
    label->setAlignment(Qt::AlignCenter);
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    label->setStyleSheet("background: transparent");

    labelLayout->addWidget(label, Qt::AlignCenter);

    mainLayout->addLayout(topLayout, 1);
    mainLayout->addLayout(labelLayout, 10);
    setLayout(mainLayout);
    QObject::connect(closeButton, &QPushButton::clicked, this, &FileWidget::onCloseButtonClicked);
}

void FileWidget::setActive()
{
    this->isActive = true;
    setStyleSheet("#FileWidget { background-color: rgb(250, 250, 250); border-bottom: 3px solid orange;}");
}

void FileWidget::setUnactive()
{
    this->isActive = false;
    setStyleSheet("#FileWidget { background-color: rgb(250, 250, 250); border-bottom: 1px solid transparent;}");
}

void FileWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

int FileWidget::getCloseIndex() const
{
    return this->closeIndex;
}

bool FileWidget::getIsActive() const
{
    return this->isActive;
}

int FileWidget::getShownIndex() const
{
    return this->shownIndex;
}

int FileWidget::getUniqueId() const
{
    return this->uniqueId;
}
void FileWidget::onCloseButtonClicked()
{
    emit closeButtonSignal(this->closeIndex);
}
void FileWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        emit widgetClickedSignal(closeIndex, uniqueId);
    }

}

QString FileWidget::getFileName() const
{
    return this->filename;
}