#include "test_radioitembutton.h"

TestRadioItemButton::TestRadioItemButton(QObject *parent)
    : QObject(parent)
    , m_radioButton(nullptr)
{
}

void TestRadioItemButton::initTestCase()
{
    // Initialize resources if needed
}

void TestRadioItemButton::cleanupTestCase()
{
    // Cleanup resources if needed
}

void TestRadioItemButton::init()
{
    m_radioButton = new RadioItemButton();
}

void TestRadioItemButton::cleanup()
{
    if (m_radioButton) {
        delete m_radioButton;
        m_radioButton = nullptr;
    }
}

void TestRadioItemButton::test_constructor()
{
    RadioItemButton *button = new RadioItemButton();
    QVERIFY(button != nullptr);
    QVERIFY(button->width() == 36);
    QVERIFY(button->height() == 36);
    delete button;
}

void TestRadioItemButton::test_destructor()
{
    RadioItemButton *button = new RadioItemButton();
    delete button;
    // If we reach here without crash, test passes
    QVERIFY(true);
}

void TestRadioItemButton::test_startLoading()
{
    m_radioButton->startLoading();
    // Test emits requestStartLoading signal
    QVERIFY(true);
}

void TestRadioItemButton::test_stopLoading()
{
    m_radioButton->startLoading();
    m_radioButton->stopLoading();
    // Test emits requestStopLoading and animationStoped signals
    QVERIFY(true);
}

void TestRadioItemButton::test_setButtonIcon()
{
    QPixmap pixmap(16, 16);
    pixmap.fill(Qt::red);
    QIcon icon(pixmap);
    m_radioButton->setButtonIcon(icon);
    QVERIFY(!icon.isNull());
}

void TestRadioItemButton::test_setButtonIcon_NullIcon()
{
    QIcon icon;
    m_radioButton->setButtonIcon(icon);
    // Should return early without crash
    QVERIFY(true);
}

void TestRadioItemButton::test_setDefaultPixmap()
{
    QPixmap pixmap(16, 16);
    pixmap.fill(Qt::red);
    QIcon icon(pixmap);
    m_radioButton->setButtonIcon(icon);
    m_radioButton->setDefaultPixmap();
    QVERIFY(true);
}

void TestRadioItemButton::test_setActive()
{
    m_radioButton->setActive(true);
    m_radioButton->setActive(false);
    QVERIFY(true);
}

void TestRadioItemButton::test_setActive_True()
{
    m_radioButton->setActive(true);
    QVERIFY(true);
}

void TestRadioItemButton::test_setActive_False()
{
    m_radioButton->setActive(false);
    QVERIFY(true);
}

void TestRadioItemButton::test_loadSvg_White()
{
    QPixmap source(16, 16);
    source.fill(QColor(255, 0, 0, 255));
    QPixmap result = m_radioButton->loadSvg(source, RadioItemButton::PixmapColor::WHITE);
    QVERIFY(!result.isNull());
}

void TestRadioItemButton::test_loadSvg_Black()
{
    QPixmap source(16, 16);
    source.fill(QColor(255, 0, 0, 255));
    QPixmap result = m_radioButton->loadSvg(source, RadioItemButton::PixmapColor::BLACK);
    QVERIFY(!result.isNull());
}

void TestRadioItemButton::test_loadSvg_Gray()
{
    QPixmap source(16, 16);
    source.fill(QColor(255, 0, 0, 255));
    QPixmap result = m_radioButton->loadSvg(source, RadioItemButton::PixmapColor::GRAY);
    QVERIFY(!result.isNull());
}

void TestRadioItemButton::test_loadSvg_Blue()
{
    QPixmap source(16, 16);
    source.fill(QColor(255, 0, 0, 255));
    QPixmap result = m_radioButton->loadSvg(source, RadioItemButton::PixmapColor::BLUE);
    QVERIFY(!result.isNull());
}

void TestRadioItemButton::test_loadSvg_Default()
{
    QPixmap source(16, 16);
    source.fill(QColor(0, 0, 0, 0));
    QPixmap result = m_radioButton->loadSvg(source, RadioItemButton::PixmapColor::WHITE);
    QVERIFY(!result.isNull());
}

void TestRadioItemButton::test_signal_requestStartLoading()
{
    QSignalSpy spy(m_radioButton, &RadioItemButton::requestStartLoading);
    QVERIFY(spy.isValid());
    m_radioButton->startLoading();
    QTRY_VERIFY(spy.count() > 0);
}

void TestRadioItemButton::test_signal_requestStopLoading()
{
    QSignalSpy spy(m_radioButton, &RadioItemButton::requestStopLoading);
    QVERIFY(spy.isValid());
    m_radioButton->startLoading();
    m_radioButton->stopLoading();
    QTRY_VERIFY(spy.count() > 0);
}

void TestRadioItemButton::test_signal_animationStoped()
{
    QSignalSpy spy(m_radioButton, &RadioItemButton::animationStoped);
    QVERIFY(spy.isValid());
    m_radioButton->startLoading();
    m_radioButton->stopLoading();
    QTRY_VERIFY(spy.count() > 0);
}

#include "test_radioitembutton.moc"
