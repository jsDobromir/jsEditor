#include "Arrow.h"

Arrow::Arrow(QWidget *parent, ARROW_DIRECTION direction)
    :QWidget(parent), direction(direction), arrowHidden(true)
    {   
        setFixedWidth(30);
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        setStyleSheet("background-color: transparent;");
        QVBoxLayout* mainLayout = new QVBoxLayout(this);

        QPixmap transparentPixmap(18, 18);
        transparentPixmap.fill(Qt::transparent);
        transparentIcon = QIcon(transparentPixmap);
                
        arrowButton = new QPushButton(this);
        arrowButton->setStyleSheet("QPushButton { border: none; background: transparent; }");
        icon = QApplication::style()->standardIcon((direction == ARROW_DIRECTION::LEFT ? QStyle::SP_ArrowLeft : QStyle::SP_ArrowRight));
        arrowButton->setIcon(transparentIcon);
        arrowButton->setIconSize(QSize(18, 18));
        arrowButton->setFixedSize(18, 18);

        QObject::connect(arrowButton, &QPushButton::clicked, [this]{ 
            emit arrowButtonSignal(this->direction);
        });
        
        mainLayout->addWidget(arrowButton, Qt::AlignCenter);
        setLayout(mainLayout);
    }

void Arrow::showButton()
{
    this->arrowButton->setIcon(icon);
    this->arrowHidden = false;
}

void Arrow::hideButton()
{
    this->arrowButton->setIcon(transparentIcon);
    this->arrowHidden = true;
}

bool Arrow::isArrowHidden()
{
    return this->arrowHidden;
}