#ifndef TEST_MULTIPLEDNSWIDGET_H
#define TEST_MULTIPLEDNSWIDGET_H

#include <QObject>
#include <QRegExp>

class MultipleDnsWidget;
class TestMultipleDnsWidget : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testConstructor();
    void testConstructor_settingShowFalse();
    void testSetEditEnabled_True();
    void testSetEditEnabled_False();
    void testGetDns_Empty();
    void testGetDns_WithDnsEntries();
    void testGetDns_WithDuplicates();
    void testSetDnsListText_Empty();
    void testSetDnsListText_WithDnsEntries();
    void testOnAddBtnClicked();
    void testOnAddBtnClicked_MultipleTimes();
    void testOnRemoveBtnClicked();
    void testOnRemoveBtnClicked_EmptyList();
    void testSetPlaceholderTextColor();
    void testShowDnsSettingWidget_InvalidInterface();

private:
    MultipleDnsWidget *m_widget;
    QRegExp m_regExp;
};

#endif // TEST_MULTIPLEDNSWIDGET_H
