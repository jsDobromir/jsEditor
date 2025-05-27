#include <QtTest>
#include "../src/Window.h"
class TestWindow : public QObject {
    Q_OBJECT

    private slots:
        void test_constructor_behavior();
    };

    void TestWindow::test_constructor_behavior() {
        int expectedWidth = 800;
        int expectedHeight = 600;

        Window window(nullptr, expectedWidth, expectedHeight);
        QVERIFY(window.layout() != nullptr);

        QCOMPARE(window.layout(), window.getLayout());
    }

QTEST_MAIN(TestWindow)

// 👇 This line is *required* for Q_OBJECT to work!
#include "test_utils.moc"
