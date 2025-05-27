#include <QtTest>
#include <QPushButton>
#include "../../src/application/Application.h"
#include "../../src/layouts/srcoll_utils/ScrollContainer.h"
#include "../../src/structure/mid_layout/MidLayout.h"
#include "../../src/layouts/file_widget/FileWidget.h"
#include "../../src/structure/bottom_layout/BottomLayout.h"
#include "../../src/structure/top_layout/TopLayout.h"

class TestApplication : public QObject {
    Q_OBJECT
    
    private slots:
        void testAddDocumentAndTextEditHandler();
    };

    void TestApplication::testAddDocumentAndTextEditHandler() {
        Application *application = new Application();
        BottomLayout *bottomLayout = new BottomLayout();
        TopLayout *topLayout = new TopLayout();
        const std::map<uint64_t, QPointer<QPlainTextEdit>>& textEditGlobalMap= application->getTextEditGlobalMap();

        QCOMPARE(textEditGlobalMap.size(), 0);

        MidLayout *midLayout = new MidLayout(nullptr);
        QVERIFY(midLayout != nullptr);

        QObject::connect(midLayout, &MidLayout::addDocumentAndTextEdit, application, &Application::addDocumentAndTextEditHandler);
        QObject::connect(midLayout, &MidLayout::removeDocumentAndTextEdit, application, &Application::removeDocumentAndTextEditHandler);

        application->setLayouts(topLayout, midLayout, bottomLayout);

        ScrollContainer *scrollArea = midLayout->getScrollArea();
        QVERIFY(scrollArea != nullptr); 
        std::vector<FileWidget*> fileWidgets = scrollArea->getFileWidgets();
        QCOMPARE(fileWidgets.size(), 0);

        QSignalSpy spy1(midLayout, &MidLayout::addDocumentAndTextEdit);
        QVERIFY(spy1.isValid());
        midLayout->newDocumentCreated("New Document");
        QList<QVariant> arguments1 = spy1.takeFirst();
        QCOMPARE(arguments1.at(0).toInt(), 0);
        const std::map<uint64_t, QPointer<QPlainTextEdit>>& textEditGlobalMap2= application->getTextEditGlobalMap();
        QCOMPARE(textEditGlobalMap2.size(), 1);
        

        QSignalSpy spy2(midLayout, &MidLayout::addDocumentAndTextEdit);
        QVERIFY(spy2.isValid());
        midLayout->newDocumentCreated("New Document");
        QList<QVariant> arguments2 = spy2.takeFirst();
        QCOMPARE(arguments2.at(0).toInt(), 1);
        const std::map<uint64_t, QPointer<QPlainTextEdit>>& textEditGlobalMap3= application->getTextEditGlobalMap();
        QCOMPARE(textEditGlobalMap3.size(), 2);

        midLayout->newDocumentCreated("New Document");
        midLayout->newDocumentCreated("New Document");

        const std::map<uint64_t, QPointer<QPlainTextEdit>>& textEditGlobalMap4= application->getTextEditGlobalMap();
        QCOMPARE(textEditGlobalMap4.size(), 4);


        fileWidgets = scrollArea->getFileWidgets();
        int lastElementUniqueId = fileWidgets[fileWidgets.size()-1]->getUniqueId();
        QCOMPARE(fileWidgets.size(), 4);

        QPointer<QPlainTextEdit> lastElementTextEdit = textEditGlobalMap4.at(lastElementUniqueId);
        QVERIFY(lastElementTextEdit != nullptr);
        qDebug()<<lastElementTextEdit;

        QSignalSpy spyMidLayout(midLayout, &MidLayout::removeDocumentAndTextEdit);
        QVERIFY(spyMidLayout.isValid());

        FileWidget *widget3 = fileWidgets[fileWidgets.size()-1];
        widget3->closeButton->click();

        QCOMPARE(spyMidLayout.count(), 1);
        QList<QVariant> argumentsMidL = spyMidLayout.takeFirst();
        QCOMPARE(argumentsMidL.size(), 3);
        QCOMPARE(argumentsMidL.at(0).toInt(), lastElementUniqueId);
        QCOMPARE(argumentsMidL.at(1).toBool(), true);
        QCOMPARE(argumentsMidL.at(2).toBool(), true);

        QVERIFY(lastElementTextEdit == nullptr);

        delete application;
    }

QTEST_MAIN(TestApplication)

#include "test_app_class.moc"
