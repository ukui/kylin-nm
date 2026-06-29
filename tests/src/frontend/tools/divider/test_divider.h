#ifndef TEST_DIVIDER_H
#define TEST_DIVIDER_H

#include <QObject>
#include <QTest>
#include <QWidget>
#include <QPainter>
#include <QMetaMethod>

class TestDivider : public QObject
{
    Q_OBJECT
public:
    explicit TestDivider(QObject *parent = nullptr);
    ~TestDivider() = default;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testConstructorDefault();
    void testConstructorWithDarkPal();
    void testConstructorWithParent();
    void testOnPaletteChanged();
    void testOnPaletteChangedWithDarkTheme();
    void testOnPaletteChangedWithoutSchema();
    void testPaintEvent();
    void testFixedHeight();

signals:
    void paletteChanged();

private:
    QWidget *m_parentWidget;
};

#endif // TEST_DIVIDER_H
