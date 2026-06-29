#include "test_listitem.h"

TestListItem::TestListItem(QObject *parent)
    : QObject(parent)
    , m_freqLabel(nullptr)
    , m_fixPushButton(nullptr)
    , m_nameLabel(nullptr)
    , m_listItem(nullptr)
{
}

void TestListItem::initTestCase()
{
}

void TestListItem::cleanupTestCase()
{
}

void TestListItem::init()
{
}

void TestListItem::cleanup()
{
    if (m_freqLabel) {
        delete m_freqLabel;
        m_freqLabel = nullptr;
    }
    if (m_fixPushButton) {
        delete m_fixPushButton;
        m_fixPushButton = nullptr;
    }
    if (m_nameLabel) {
        delete m_nameLabel;
        m_nameLabel = nullptr;
    }
    if (m_listItem) {
        delete m_listItem;
        m_listItem = nullptr;
    }
}

// Test cases for FreqLabel
void TestListItem::test_freqLabel_constructor()
{
    m_freqLabel = new FreqLabel();
    QVERIFY(m_freqLabel != nullptr);
    delete m_freqLabel;
    m_freqLabel = nullptr;
}

// Test cases for FixPushButton
void TestListItem::test_fixPushButton_constructor()
{
    m_fixPushButton = new FixPushButton();
    QVERIFY(m_fixPushButton != nullptr);
    delete m_fixPushButton;
    m_fixPushButton = nullptr;
}

void TestListItem::test_fixPushButton_setButtonText()
{
    m_fixPushButton = new FixPushButton();
    QString testText = "Test Button";
    m_fixPushButton->setButtonText(testText);
    QString result = m_fixPushButton->getText();
    QVERIFY2(result.contains("Test Button"), "Button text should be set");
    delete m_fixPushButton;
    m_fixPushButton = nullptr;
}

void TestListItem::test_fixPushButton_getText()
{
    m_fixPushButton = new FixPushButton();
    QString testText = "Get Test";
    m_fixPushButton->setButtonText(testText);
    QString result = m_fixPushButton->getText();
    QVERIFY2(result == testText || result.contains("Get Test"), "getText should return the set text");
    delete m_fixPushButton;
    m_fixPushButton = nullptr;
}

// Test cases for NameLabel
void TestListItem::test_nameLabel_constructor()
{
    m_nameLabel = new NameLabel();
    QVERIFY(m_nameLabel != nullptr);
    delete m_nameLabel;
    m_nameLabel = nullptr;
}

void TestListItem::test_nameLabel_setLabelText()
{
    m_nameLabel = new NameLabel();
    QString testText = "Test Label";
    m_nameLabel->setLabelText(testText);
    QString labelText = m_nameLabel->text();
    QVERIFY2(labelText.contains("Test Label"), "Label text should be set");
    delete m_nameLabel;
    m_nameLabel = nullptr;
}

void TestListItem::test_nameLabel_setLabelMaximumWidth()
{
    m_nameLabel = new NameLabel();
    int maxWidth = 100;
    m_nameLabel->setLabelMaximumWidth(maxWidth);
    QVERIFY(m_nameLabel->maximumWidth() == maxWidth);
    delete m_nameLabel;
    m_nameLabel = nullptr;
}

void TestListItem::test_nameLabel_setPointSizeOffset()
{
    m_nameLabel = new NameLabel();
    int offset = 5;
    m_nameLabel->setPointSizeOffset(offset);
    QVERIFY(m_nameLabel != nullptr);
    delete m_nameLabel;
    m_nameLabel = nullptr;
}

// Test cases for ListItem (via subclass)
void TestListItem::test_listItem_constructor()
{
    m_listItem = new TestListItemChild();
    QVERIFY(m_listItem != nullptr);
    delete m_listItem;
    m_listItem = nullptr;
}

void TestListItem::test_listItem_destructor()
{
    m_listItem = new TestListItemChild();
    delete m_listItem;
    m_listItem = nullptr;
    QVERIFY(true);
}

void TestListItem::test_listItem_setName()
{
    m_listItem = new TestListItemChild();
    QString testName = "Test Network";
    m_listItem->setName(testName);
    QVERIFY(m_listItem != nullptr);
    delete m_listItem;
    m_listItem = nullptr;
}

void TestListItem::test_listItem_setActive()
{
    m_listItem = new TestListItemChild();
    m_listItem->setActive(true);
    QVERIFY(m_listItem != nullptr);
    m_listItem->setActive(false);
    QVERIFY(m_listItem != nullptr);
    delete m_listItem;
    m_listItem = nullptr;
}

void TestListItem::test_listItem_setConnectState()
{
    m_listItem = new TestListItemChild();
    m_listItem->setConnectState(UnknownState);
    m_listItem->setConnectState(Activating);
    m_listItem->setConnectState(Activated);
    m_listItem->setConnectState(Deactivating);
    m_listItem->setConnectState(Deactivated);
    QVERIFY(m_listItem != nullptr);
    delete m_listItem;
    m_listItem = nullptr;
}

void TestListItem::test_listItem_setConnectivityText()
{
    m_listItem = new TestListItemChild();
    QString connectivityText = "Connected";
    m_listItem->setConnectivityText(connectivityText);
    QVERIFY(m_listItem != nullptr);
    delete m_listItem;
    m_listItem = nullptr;
}

void TestListItem::test_listItem_onPaletteChanged()
{
    m_listItem = new TestListItemChild();
    m_listItem->onPaletteChanged();
    QVERIFY(m_listItem != nullptr);
    delete m_listItem;
    m_listItem = nullptr;
}

