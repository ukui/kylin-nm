#include "test_kylable.h"
#include "../../../../../../src/frontend/tools/kylable.h"
#include <QApplication>
#include <QDebug>
#include <cmath>

TestKyLable::TestKyLable(QObject *parent)
    : QObject(parent)
{
}

void TestKyLable::initTestCase()
{
    int argc = 0;
    char **argv = nullptr;
    QApplication app(argc, argv);
}

void TestKyLable::cleanupTestCase()
{
}

void TestKyLable::init()
{
    m_kyLable = new KyLable(nullptr);
    m_fixLabel = new FixLabel(nullptr);
}

void TestKyLable::cleanup()
{
    delete m_kyLable;
    delete m_fixLabel;
    m_kyLable = nullptr;
    m_fixLabel = nullptr;
}

// Test mixColor function
void TestKyLable::testMixColor_biasLessThanZero()
{
    QColor c1(255, 0, 0);
    QColor c2(0, 255, 0);
    QColor result = mixColor(c1, c2, -0.5);
    QVERIFY(result == c1);
}

void TestKyLable::testMixColor_biasEqualToZero()
{
    QColor c1(255, 0, 0);
    QColor c2(0, 255, 0);
    QColor result = mixColor(c1, c2, 0.0);
    QVERIFY(result == c1);
}

void TestKyLable::testMixColor_biasBetweenZeroAndOne()
{
    QColor c1(255, 0, 0);
    QColor c2(0, 255, 0);
    QColor result = mixColor(c1, c2, 0.5);
    QVERIFY(result.red() == 127);
    QVERIFY(result.green() == 127);
    QVERIFY(result.blue() == 0);
}

void TestKyLable::testMixColor_biasEqualToOne()
{
    QColor c1(255, 0, 0);
    QColor c2(0, 255, 0);
    QColor result = mixColor(c1, c2, 1.0);
    QVERIFY(result == c2);
}

void TestKyLable::testMixColor_biasGreaterThanOne()
{
    QColor c1(255, 0, 0);
    QColor c2(0, 255, 0);
    QColor result = mixColor(c1, c2, 1.5);
    QVERIFY(result == c2);
}

void TestKyLable::testMixColor_biasIsNaN()
{
    QColor c1(255, 0, 0);
    QColor c2(0, 255, 0);
    QColor result = mixColor(c1, c2, std::numeric_limits<double>::quiet_NaN());
    QVERIFY(result == c1);
}

// Test FixLabel class
void TestKyLable::testFixLabel_SetAndGetText()
{
    FixLabel label(nullptr);
    QString testText = "Test Text";
    label.setLabelText(testText);
    QVERIFY(label.getText() == testText);
}

void TestKyLable::testFixLabel_SetLongText()
{
    FixLabel label(nullptr);
    label.setFixedWidth(100);
    QString longText = "This is a very long text that should be elided when displayed";
    label.setLabelText(longText);
    QVERIFY(label.getText() == longText);
}

void TestKyLable::testFixLabel_SetEmptyText()
{
    FixLabel label(nullptr);
    QString emptyText = "";
    label.setLabelText(emptyText);
    QVERIFY(label.getText() == emptyText);
}

void TestKyLable::testFixLabel_Event()
{
    FixLabel label(nullptr);
    QEvent paintEvent(QEvent::Paint);
    bool result = label.event(&paintEvent);
    QVERIFY(result == true);
}

// Test KyLable class
void TestKyLable::testKyLable_Constructor()
{
    KyLable *label = new KyLable(nullptr);
    QVERIFY(label != nullptr);
    QVERIFY(label->testAttribute(Qt::WA_Hover) == true);
    QVERIFY(label->hasMouseTracking() == true);
    delete label;
}

void TestKyLable::testKyLable_onPaletteChanged()
{
    KyLable label(nullptr);
    QSignalSpy spy(&label, SIGNAL(paletteChanged()));
    label.onPaletteChanged();
    QVERIFY(label.testAttribute(Qt::WA_Hover) == true);
}

void TestKyLable::testKyLable_setPressColor()
{
    KyLable label(nullptr);
    label.setPressColor();
    QVERIFY(label.testAttribute(Qt::WA_Hover) == true);
}

void TestKyLable::testKyLable_setHoverColor()
{
    KyLable label(nullptr);
    label.setHoverColor();
    QVERIFY(label.testAttribute(Qt::WA_Hover) == true);
}

void TestKyLable::testKyLable_setNormalColor()
{
    KyLable label(nullptr);
    label.setNormalColor();
    QVERIFY(label.testAttribute(Qt::WA_Hover) == true);
}

void TestKyLable::testKyLable_enterEvent()
{
    KyLable label(nullptr);
    QEvent enterEvent(QEvent::Enter);
    label.enterEvent(&enterEvent);
    QVERIFY(label.testAttribute(Qt::WA_Hover) == true);
}

void TestKyLable::testKyLable_leaveEvent()
{
    KyLable label(nullptr);
    QEvent leaveEvent(QEvent::Leave);
    label.leaveEvent(&leaveEvent);
    QVERIFY(label.testAttribute(Qt::WA_Hover) == true);
}

void TestKyLable::testKyLable_mousePressEvent()
{
    KyLable label(nullptr);
    QMouseEvent pressEvent(QEvent::MouseButtonPress, QPointF(10, 10), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    label.mousePressEvent(&pressEvent);
    QVERIFY(label.testAttribute(Qt::WA_Hover) == true);
}

void TestKyLable::testKyLable_mouseReleaseEvent()
{
    KyLable label(nullptr);
    QMouseEvent releaseEvent(QEvent::MouseButtonRelease, QPointF(10, 10), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    label.mouseReleaseEvent(&releaseEvent);
    QVERIFY(label.testAttribute(Qt::WA_Hover) == true);
}

void TestKyLable::testKyLable_paintEvent()
{
    KyLable label(nullptr);
    QPaintEvent paintEvent(label.rect());
    label.paintEvent(&paintEvent);
    QVERIFY(label.testAttribute(Qt::WA_Hover) == true);
}

QColor TestKyLable::mixColor(const QColor& c1, const QColor& c2, qreal bias)
{
    // 把比例限制在 0~1 之间，防止越界
    bias = qBound(0.0, bias, 1.0);

    // 分别对 R、G、B、A 做线性插值
    int r = c1.red()   * (1.0 - bias) + c2.red()   * bias;
    int g = c1.green() * (1.0 - bias) + c2.green() * bias;
    int b = c1.blue()  * (1.0 - bias) + c2.blue()  * bias;
    int a = c1.alpha() * (1.0 - bias) + c2.alpha() * bias;

    return QColor(r, g, b, a);
}
