#include <QtTest>
#include <QPushButton>
#include "../src/layouts/MyLayout.h"
class TestMyLayout : public QObject {
    Q_OBJECT

    private slots:
        void test_layout_constructor();
        void test_addWidget();
        void test_layout_add();
    };

    void TestMyLayout::test_layout_constructor() {
        MyLayout<QVBoxLayout> layoutWidget;
        QVBoxLayout* layout = layoutWidget.getLayout();

        QVERIFY(layoutWidget.layout() == layout);
    }

    void TestMyLayout::test_addWidget() {
        MyLayout<QVBoxLayout> layoutWidget;
        QVBoxLayout* layout = layoutWidget.getLayout();

        QPushButton* button = new QPushButton("Test");
        layout->addWidget(button);

        QLayoutItem* item = layout->itemAt(0);
        QVERIFY(item != nullptr);
        QCOMPARE(item->widget(), button);

        QCOMPARE(button->parentWidget(), &layoutWidget);
    }

    void TestMyLayout::test_layout_add() {
        MyLayout<QVBoxLayout> layoutWidget;
        QVBoxLayout* addedLayout = new QVBoxLayout();
        layoutWidget.addAbsLayout(addedLayout);

        QVBoxLayout* layout = layoutWidget.getLayout();
        QLayoutItem* item = layout->itemAt(0);
        QVERIFY(item != nullptr);
        QVERIFY(item->layout() != nullptr);

        // Check if it's our inner layout
        QCOMPARE(item->layout(), addedLayout);
    }

QTEST_MAIN(TestMyLayout)

// 👇 This line is *required* for Q_OBJECT to work!
#include "test_layout.moc"