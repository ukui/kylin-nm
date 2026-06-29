#ifndef TEST_LISTITEMEDIT_H
#define TEST_LISTITEMEDIT_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include <QLineEdit>
#include <QStandardItemModel>
#include "listitemedit.h"

class TestListItemEdit : public QObject
{
    Q_OBJECT
public:
    explicit TestListItemEdit(QObject *parent = nullptr);
    ~TestListItemEdit() = default;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Test cases for ListItemEdit functions
    void test_constructor();
    void test_destructor();
    void test_createEditor();
    void test_createEditor_WithValidParent();
    void test_setEditorData();
    void test_setModelData();
    void test_updateEditorGeometry();

private:
    ListItemEdit *m_listItemEdit;
    QStandardItemModel *m_model;
};

#endif // TEST_LISTITEMEDIT_H
