#ifndef TEST_SINGLEPAGE_H
#define TEST_SINGLEPAGE_H

#include <QObject>
#include <QTest>
#include <QDebug>
#include <QSignalSpy>

class SinglePage;

class TestSinglePage : public QObject
{
    Q_OBJECT
public:
    explicit TestSinglePage(QObject *parent = nullptr);
    ~TestSinglePage();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testConstructor();
    void testDestructor();
    void testShowDesktopNotify();
    void testOnTransChanged();
    void testOnThemeChanged();
    void testInitUI();
    void testInitTransparency();
    void testInitWindowProperties();
    void testInitWindowTheme();

private:
    SinglePage *m_singlePage;
};

#endif // TEST_SINGLEPAGE_H
