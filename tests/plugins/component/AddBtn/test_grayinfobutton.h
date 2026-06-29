#ifndef TEST_GRAYINFOBUTTON_H
#define TEST_GRAYINFOBUTTON_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include <QWidget>

class TestGrayInfoButton : public QObject
{
    Q_OBJECT
public:
    explicit TestGrayInfoButton(QObject *parent = nullptr);
    ~TestGrayInfoButton();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // test_constructor_withNullParent
    void test_constructor_withNullParent();
    // test_constructor_withValidParent
    void test_constructor_withValidParent();
    // test_destructor
    void test_destructor();
    // test_buttonSize
    void test_buttonSize();
    // test_iconSetting
    void test_iconSetting();
    // test_propertyUseButtonPalette
    void test_propertyUseButtonPalette();
    // test_flatProperty
    void test_flatProperty();
};

#endif // TEST_GRAYINFOBUTTON_H
