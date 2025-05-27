#include <QtTest>
#include <QPushButton>
#include "../../src/layouts/srcoll_utils/ScrollContainer.h"

class TestScrollContainer : public QObject {
    Q_OBJECT

    private slots:
        void testaddFileCont();
        void testGetShownIndex();
    };

    void TestScrollContainer::testaddFileCont() {
        ScrollContainer *container = new ScrollContainer();
        
        for (int i =0;i<20;i++) {
            container->addFileCont("New Document");
        }
        std::vector<FileWidget*> fileWidgets = container->getFileWidgets();
        QCOMPARE(fileWidgets.size(), 20);
        QVERIFY(fileWidgets[fileWidgets.size()-1]->getIsActive());
        int UNIQUE_ID = 19;
        for (int i=0;i<20;i++) {
            QCOMPARE(fileWidgets[i]->getCloseIndex(), i);
            QCOMPARE(fileWidgets[i]->getUniqueId(), i);
        }
        QCOMPARE(container->getActiveIndex(), 19);

        for (int i=0; i<20;i++) {
            if (i%2==0) {
                fileWidgets[i]->closeButton->click();
            }
        }
        fileWidgets = container->getFileWidgets();
        QCOMPARE(fileWidgets.size(), 10);
        QCOMPARE(container->getActiveIndex(), fileWidgets.size()-1);

        for (int i=0; i<fileWidgets.size();i++) {
            QCOMPARE(fileWidgets[i]->getCloseIndex(), i);
        }

        fileWidgets[fileWidgets.size()-1]->closeButton->click();
        fileWidgets = container->getFileWidgets();

        QCOMPARE(container->getActiveIndex(), fileWidgets.size()-1);

        fileWidgets[0]->setActive();
        for (int i=1; i<fileWidgets.size();i++) {
            fileWidgets[i]->setUnactive();
        }
        container->setActiveIndex(0);
        QCOMPARE(container->getActiveIndex(), 0);
        int prevSize = fileWidgets.size();
        fileWidgets[0]->closeButton->click();
        QCOMPARE(container->getActiveIndex(), 0);
        QCOMPARE(container->getFileWidgets().size(), prevSize-1);

        int nextClosingIndex = container->getFileWidgets().size();
        int nextActiveIndex = container->getFileWidgets().size();
        for (int i=0; i<10;i++) {
            container->addFileCont("New Document");
            QCOMPARE(container->getActiveIndex(), nextActiveIndex);
            nextActiveIndex++;
        }
        fileWidgets = container->getFileWidgets();
        QCOMPARE(fileWidgets.size(), nextClosingIndex+10);

        QCOMPARE(container->getActiveIndex(), fileWidgets.size()-1);

        delete container;
    }

    void TestScrollContainer::testGetShownIndex() {
        ScrollContainer *container = new ScrollContainer();

        int START_INDEX = 1;
        for (int i =0;i<10;i++) {
            container->addFileCont("New Document");
        }

        std::vector<FileWidget*> fileWidgets = container->getFileWidgets();
        for (FileWidget* fileWidget : fileWidgets) {
            QCOMPARE(fileWidget->getShownIndex(), START_INDEX);
            START_INDEX++;
        }

        //delete index 4 and 5 -> 5 and 6 shownIndex
        fileWidgets[4]->closeButton->click();
        fileWidgets[5]->closeButton->click();

        container->addFileCont("New Document");
        container->addFileCont("New Document");

        fileWidgets = container->getFileWidgets();

        QCOMPARE(fileWidgets[fileWidgets.size()-1]->getShownIndex(), 6);
        QCOMPARE(fileWidgets[fileWidgets.size()-2]->getShownIndex(), 5);

        
    }

QTEST_MAIN(TestScrollContainer)

#include "test_scroll_cont.moc"
