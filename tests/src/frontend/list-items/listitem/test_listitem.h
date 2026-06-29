#ifndef TEST_LISTITEM_H
#define TEST_LISTITEM_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include <QPaintEvent>
#include <QMouseEvent>
#include "listitem.h"

class TestListItem : public QObject
{
    Q_OBJECT
public:
    explicit TestListItem(QObject *parent = nullptr);
    ~TestListItem() = default;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Test cases for FreqLabel
    void test_freqLabel_constructor();

    // Test cases for FixPushButton
    void test_fixPushButton_constructor();
    void test_fixPushButton_setButtonText();
    void test_fixPushButton_getText();

    // Test cases for NameLabel
    void test_nameLabel_constructor();
    void test_nameLabel_setLabelText();
    void test_nameLabel_setLabelMaximumWidth();
    void test_nameLabel_setPointSizeOffset();

    // Test cases for ListItem (via subclass)
    void test_listItem_constructor();
    void test_listItem_destructor();
    void test_listItem_setName();
    void test_listItem_setActive();
    void test_listItem_setConnectState();
    void test_listItem_setConnectivityText();
    void test_listItem_onPaletteChanged();

private:
    FreqLabel *m_freqLabel;
    FixPushButton *m_fixPushButton;
    NameLabel *m_nameLabel;
    ListItem *m_listItem;
};

class TestListItemChild : public ListItem
{
    Q_OBJECT
public:
    explicit TestListItemChild(QWidget *parent = nullptr) : ListItem(parent) {}

protected:
    void onRightButtonClicked() override {}
    void onNetButtonClicked() override {}
    void onNetButtonReleased() override {}
    void onMenuTriggered(QAction *action) override {}

public:
    using ListItem::setName;
    using ListItem::setActive;
    using ListItem::setConnectState;
    using ListItem::setConnectivityText;
    using ListItem::onPaletteChanged;
};

#endif // TEST_LISTITEM_H
