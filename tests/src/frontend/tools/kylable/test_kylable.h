#ifndef TEST_KYLABLE_H
#define TEST_KYLABLE_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include <QWidget>
#include <QMouseEvent>
#include <QLabel>
#include <QColor>
#include "../../../../../src/frontend/tools/kylable.h"

class TestKyLable : public QObject
{
    Q_OBJECT
public:
    explicit TestKyLable(QObject *parent = nullptr);
    QColor mixColor(const QColor& c1, const QColor& c2, qreal bias);
private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Test mixColor function
    void testMixColor_biasLessThanZero();
    void testMixColor_biasEqualToZero();
    void testMixColor_biasBetweenZeroAndOne();
    void testMixColor_biasEqualToOne();
    void testMixColor_biasGreaterThanOne();
    void testMixColor_biasIsNaN();

    // Test FixLabel class
    void testFixLabel_SetAndGetText();
    void testFixLabel_SetLongText();
    void testFixLabel_SetEmptyText();
    void testFixLabel_Event();

    // Test KyLable class
    void testKyLable_Constructor();
    void testKyLable_onPaletteChanged();
    void testKyLable_setPressColor();
    void testKyLable_setHoverColor();
    void testKyLable_setNormalColor();
    void testKyLable_enterEvent();
    void testKyLable_leaveEvent();
    void testKyLable_mousePressEvent();
    void testKyLable_mouseReleaseEvent();
    void testKyLable_paintEvent();

private:
    KyLable *m_kyLable;
    FixLabel *m_fixLabel;
};

#endif // TEST_KYLABLE_H
