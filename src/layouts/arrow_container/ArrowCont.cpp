#include "ArrowCont.h"

ArrowCont::ArrowCont(QWidget *parent, Qt::Alignment alignment) : QWidget(parent), alignment(alignment), arrowButton(new QPushButton(this))
{   
    setObjectName("ArrowCont");
    QVBoxLayout* mainLayout = new QVBoxLayout();
    QIcon icon = QApplication::style()->standardIcon((alignment == Qt::AlignLeft ? QStyle::SP_ArrowLeft : QStyle::SP_ArrowRight));
    arrowButton->setStyleSheet("#ArrowCont QPushButton{ border: none; background: transparent; }");
    arrowButton->setIcon(icon);
    arrowButton->setIconSize(QSize(15, 15));
    arrowButton->setFixedSize(15, 15);
    QObject::connect(arrowButton, &QPushButton::clicked, [this]{
        emit arrowButtonSignal(this->alignment);
    });
    mainLayout->addWidget(arrowButton);
    setLayout(mainLayout);
}

QPushButton* ArrowCont::getButton() {
    return arrowButton;
}
void ArrowCont::showArrow()
{

}
void ArrowCont::hideArrow()
{

}
bool ArrowCont::isArrowHidden()
{

}