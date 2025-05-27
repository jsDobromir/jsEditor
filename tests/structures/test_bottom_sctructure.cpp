#include <QtTest>
#include <QTextEdit>
#include "../../src/structure/bottom_layout/BottomLayout.h"

class TestBottomLayout : public QObject {
    Q_OBJECT

    private slots:
        void testAddQTextEditor();
    };

    void TestBottomLayout::testAddQTextEditor() {
        // QTextEdit *textEdit1 = new QTextEdit();
        // qDebug()<<textEdit1;
        // BottomLayout *bottomLayout = new BottomLayout();
        // bottomLayout->addTextEditor(textEdit1);
        // QCOMPARE(bottomLayout->getTextEditor(), textEdit1);

        // QTextEdit *textEditNew = new QTextEdit();
        // bottomLayout->removeTextEditor(textEditNew);
        // QCOMPARE(bottomLayout->getTextEditor(), textEditNew);
        // QCOMPARE(textEdit1, nullptr);
    }

QTEST_MAIN(TestBottomLayout)

// 👇 This line is *required* for Q_OBJECT to work!
#include "test_bottom_sctructure.moc"