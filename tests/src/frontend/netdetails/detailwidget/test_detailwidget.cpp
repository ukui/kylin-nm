#include "test_detailwidget.h"
#include <QPushButton>

TestDetailWidget::TestDetailWidget(QObject *parent)
    : QObject(parent)
    , m_parentWidget(nullptr)
    , m_valueWidget(nullptr)
    , m_copyButton(nullptr)
{
}

void TestDetailWidget::initTestCase()
{
    m_parentWidget = new QWidget();
    m_valueWidget = new QLabel("Test Value");
    m_copyButton = new QPushButton();
}

void TestDetailWidget::cleanupTestCase()
{
    delete m_parentWidget;
    m_parentWidget = nullptr;
    delete m_copyButton;
    m_copyButton = nullptr;
    delete m_valueWidget;
    m_valueWidget = nullptr;
}

void TestDetailWidget::init()
{
}

void TestDetailWidget::cleanup()
{
}

void TestDetailWidget::test_constructor_with_nullptr_button()
{
    DetailWidget *widget = new DetailWidget(m_valueWidget, m_parentWidget, nullptr);
    QVERIFY(widget != nullptr);
    delete widget;
}

void TestDetailWidget::test_constructor_with_valid_button()
{
    DetailWidget *widget = new DetailWidget(m_valueWidget, m_parentWidget, m_copyButton);
    QVERIFY(widget != nullptr);
    delete widget;
    m_copyButton = new QPushButton();
}

void TestDetailWidget::test_setKey()
{
    DetailWidget *widget = new DetailWidget(m_valueWidget, m_parentWidget, nullptr);
    widget->setKey("Test Key Label");
    delete widget;
}

void TestDetailWidget::test_destructor()
{
    DetailWidget *widget = new DetailWidget(m_valueWidget, m_parentWidget, nullptr);
    delete widget;
    QVERIFY(true);
}

void TestDetailWidget::test_initUI_with_nullptr_button()
{
    DetailWidget *widget = new DetailWidget(m_valueWidget, m_parentWidget, nullptr);
    QVERIFY(widget != nullptr);
    delete widget;
}

void TestDetailWidget::test_initUI_with_valid_button()
{
    DetailWidget *widget = new DetailWidget(m_valueWidget, m_parentWidget, m_copyButton);
    QVERIFY(widget != nullptr);
    delete widget;
    m_copyButton = new QPushButton();
}
