#include <QtTest>
#include "../../src/layouts/Button.h"
#include "../../src/layouts/MyLayout.h"
#include "../../src/structure/top_layout/TopLayout.h"

class TestTopLayout : public QObject {
    Q_OBJECT

    private slots:
        void testSignalEmitted();
        void testInitMethod();
    };

    void TestTopLayout::testSignalEmitted() {

        TopLayout *topLayout = new TopLayout();
        auto button2 = new Custom_Button::Button("Create", [topLayout]() { topLayout->newDocumentMethod(QString("New Document")); }, topLayout);

        QSignalSpy spy(topLayout, SIGNAL(newDocumentSignal(QString)));
        QVERIFY(spy.isValid());  // Ensure the spy is set up correctly

        button2->click();
        QCOMPARE(spy.count(), 1);  // Verify that the signal was emitted once
        QList<QVariant> arguments = spy.takeFirst();
        QCOMPARE(arguments.at(0).toString(), QString("New Document"));  // Check the argument passed with the signal
        delete topLayout;
    }

    void TestTopLayout::testInitMethod() {
        TopLayout *topLayout = new TopLayout();
        QBoxLayout *menuLayout = qobject_cast<QBoxLayout*>(topLayout->layout());
        QVERIFY(menuLayout != nullptr);

        auto button1 = new Custom_Button::Button("Open File", [this]() { qDebug()<<"Open File button"; }, nullptr);
        auto button2 = new Custom_Button::Button("Create", [this]() { qDebug()<<"Create File button"; }, nullptr);
        auto button3 = new Custom_Button::Button("Save File", [this]() { qDebug()<<"Save File button"; }, nullptr);


        MyLayout<QHBoxLayout> *leftMenuLayout = new MyLayout<QHBoxLayout>();
        MyLayout<QHBoxLayout> *rightMenuLayout = new MyLayout<QHBoxLayout>();
        QBoxLayout *menuLayoutbottm = new QBoxLayout(QBoxLayout::LeftToRight);

        leftMenuLayout->addWidget(button1);
        leftMenuLayout->addSpacing(10);
        leftMenuLayout->addWidget(button2);
        QLayoutItem* button1Item = leftMenuLayout->getLayout()->itemAt(0);
        QLayoutItem* button2Item = leftMenuLayout->getLayout()->itemAt(2);
        QVERIFY(button1Item != nullptr);
        QCOMPARE(button1Item->widget(), button1);

        QVERIFY(button2Item != nullptr);
        QCOMPARE(button2Item->widget(), button2);

        rightMenuLayout->addWidget(button3);

        menuLayoutbottm->addWidget(leftMenuLayout);
        menuLayoutbottm->addStretch();
        menuLayoutbottm->addWidget(rightMenuLayout);

        QLayoutItem* item = menuLayoutbottm->itemAt(0);
        QVERIFY(item != nullptr);
        QCOMPARE(item->widget(), leftMenuLayout);

        QLayoutItem *item2 = menuLayoutbottm->itemAt(2);
        QVERIFY(item2 != nullptr);
        QCOMPARE(item2->widget(), rightMenuLayout);

        delete leftMenuLayout;
        delete rightMenuLayout;
        delete menuLayout;
    }

QTEST_MAIN(TestTopLayout)

// 👇 This line is *required* for Q_OBJECT to work!
#include "test_top_structure.moc"