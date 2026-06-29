#ifndef TEST_INFOBUTTON_H
#define TEST_INFOBUTTON_H

#include <QObject>
#include <QWidget>

class TestInfoButton : public QObject
{
    Q_OBJECT
public:
    explicit TestInfoButton(QObject *parent = nullptr);
    ~TestInfoButton();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_constructor_withNullParent();
    void test_constructor_withValidParent();
    void test_destructor();
    void test_buttonSize();
    void test_backgroundColor();
    void test_foregroundColor();
    void test_enterEvent();
    void test_leaveEvent();
    void test_mousePressEvent();
    void test_mouseReleaseEvent();
    void test_paintEvent();
    void test_gsettingsConnection();
};

#endif
