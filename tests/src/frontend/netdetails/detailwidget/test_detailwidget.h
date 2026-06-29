#ifndef TEST_DETAILWIDGET_H
#define TEST_DETAILWIDGET_H

#include <QObject>
#include <QTest>
#include <QWidget>
#include <QLabel>
#include "../../../../../../src/frontend/netdetails/detailwidget.h"

class TestDetailWidget : public QObject
{
    Q_OBJECT
public:
    explicit TestDetailWidget(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_constructor_with_nullptr_button();
    void test_constructor_with_valid_button();
    void test_setKey();
    void test_destructor();
    void test_initUI_with_nullptr_button();
    void test_initUI_with_valid_button();

private:
    QWidget *m_parentWidget;
    QWidget *m_valueWidget;
    QWidget *m_copyButton;
};

#endif // TEST_DETAILWIDGET_H
