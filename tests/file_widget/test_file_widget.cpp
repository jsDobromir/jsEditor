#include <QtTest>
#include <QPushButton>
#include "../../src/layouts/file_widget/FileWidget.h"

class TestFileWidget : public QObject {
    Q_OBJECT

    private slots:
        void testWidgetSignal();
    };
    
    void TestFileWidget::testWidgetSignal() {
        FileWidget *widget1 = new FileWidget("New Document", 0, 140, 1, nullptr);
        FileWidget *widget2 = new FileWidget("New Document", 1, 140, 2, nullptr);
        FileWidget *widget3 = new FileWidget("New Document", 2, 140, 3, nullptr);
        
        QSignalSpy spy(widget1, &FileWidget::closeButtonSignal);
        QVERIFY(spy.isValid());
        widget1->closeButton->click();
        QCOMPARE(spy.count(), 1);
        QList<QVariant> arguments = spy.takeFirst();
        QCOMPARE(arguments.at(0).toInt(), 0);
        QCOMPARE(widget1->getUniqueId(), 0);
        delete widget1;
        
        QSignalSpy spy2(widget2, &FileWidget::closeButtonSignal);
        QVERIFY(spy2.isValid());
        widget2->closeButton->click();
        QCOMPARE(spy2.count(), 1);
        QList<QVariant> arguments2 = spy2.takeFirst();
        QCOMPARE(arguments2.at(0).toInt(), 1);
        QCOMPARE(widget2->getUniqueId(), 1);

        delete widget2;

        QSignalSpy spy3(widget3, &FileWidget::closeButtonSignal);
        QVERIFY(spy3.isValid());
        widget3->closeButton->click();
        QCOMPARE(spy3.count(), 1);
        QList<QVariant> arguments3 = spy3.takeFirst();
        QCOMPARE(arguments3.at(0).toInt(), 2);
        QCOMPARE(widget3->getUniqueId(), 2);

        delete widget3;
    }

QTEST_MAIN(TestFileWidget)

#include "test_file_widget.moc"
