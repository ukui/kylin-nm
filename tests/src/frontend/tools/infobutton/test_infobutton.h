#ifndef TEST_INFOBUTTON_H
#define TEST_INFOBUTTON_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include <QWidget>
#include <QMouseEvent>

class TestInfoButton : public QObject
{
    Q_OBJECT
public:
    explicit TestInfoButton(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testConstructor();
    void testInitUI();
    void testOnPaletteChanged();
    void testEnterEvent();
    void testLeaveEvent();
    void testMousePressEvent();
    void testMouseReleaseEvent();

private:
    void simulateEnterEvent(QWidget *widget);
    void simulateLeaveEvent(QWidget *widget);
    void simulateMousePress(QWidget *widget);
    void simulateMouseRelease(QWidget *widget);
};

#endif // TEST_INFOBUTTON_H
