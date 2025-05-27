#include <QtTest>
#include "../src/layouts/Button.h"
class TestCustomButtom : public QObject {
    Q_OBJECT

    private slots:
        void test_constructor_behavior();
    };

    void TestCustomButtom::test_constructor_behavior() {
        bool called = false;
        Custom_Button::Callback cb = [&called]() {
            called = true;
        };

        Custom_Button::Button button1("button1", cb ,nullptr);
        button1.show();

        QTest::mouseClick(&button1, Qt::LeftButton);

        QVERIFY(called);
    }

QTEST_MAIN(TestCustomButtom)

// 👇 This line is *required* for Q_OBJECT to work!
#include "test_custom_button.moc"