#include "TopLayout.h"
#include <QDebug>

TopLayout::TopLayout(QWidget *parent) : MyLayout<QHBoxLayout>(parent), leftMenuLayout(new MyLayout<QHBoxLayout>()), rightMenuLayout(new MyLayout<QHBoxLayout>()), menuLayout(new QBoxLayout(QBoxLayout::LeftToRight))
{
    this->init();
}

void TopLayout::newDocumentMethod(QString filename)
{
    qDebug()<<"new Doc Method";
    emit newDocumentSignal(filename);
}

void TopLayout::init() {
    auto button1 = new Custom_Button::Button("Open File", [this]() { qDebug()<<"Open File button"; }, nullptr);
    auto button2 = new Custom_Button::Button("Create", [this]() { this->newDocumentMethod(QString("New Document")); }, nullptr);
    auto button3 = new Custom_Button::Button("Save File", [this]() { qDebug()<<"Save File button"; }, nullptr);
    
    leftMenuLayout->addWidget(button1);
    leftMenuLayout->addSpacing(10);
    leftMenuLayout->addWidget(button2);

    rightMenuLayout->addWidget(button3);

    menuLayout->addWidget(leftMenuLayout);
    menuLayout->addStretch();
    menuLayout->addWidget(rightMenuLayout);

    addAbsLayout(menuLayout);
}