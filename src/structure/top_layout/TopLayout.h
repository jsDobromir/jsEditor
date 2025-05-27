#ifndef TOP_LAYOUT
#define TOP_LAYOUT
#include <QHBoxLayout>
#include <QBoxLayout>
#include "../../layouts/MyLayout.h"
#include "../../layouts/Button.h"

class TopLayout : public MyLayout<QHBoxLayout> {
    Q_OBJECT
    public:
        explicit TopLayout(QWidget *parent = nullptr);
        void newDocumentMethod(QString filename);
    protected:
        void init();
    private:
        MyLayout<QHBoxLayout> *leftMenuLayout;
        MyLayout<QHBoxLayout> *rightMenuLayout;
        QBoxLayout *menuLayout;
    signals:
        void newDocumentSignal(QString fileName);
};

#endif