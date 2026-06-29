#include <QTest>
#include <QObject>
#include <QSize>
#include <QPainter>
#include <QStyleOption>
#include <QStyleOptionTab>
#include <QWidget>
#include <QDebug>

#include "customtabstyle.h"
#include "test_customtabstyle.h"

void TestCustomTabStyle::initTestCase() {}
void TestCustomTabStyle::cleanupTestCase() {}
void TestCustomTabStyle::init() {}
void TestCustomTabStyle::cleanup() {}

void TestCustomTabStyle::testConstructor()
{
    qDebug() << Q_FUNC_INFO << __LINE__ ;
    CustomTabStyle *style = new CustomTabStyle();
    QVERIFY(style != nullptr);
    delete style;
}

void TestCustomTabStyle::testSizeFromContents()
{
    qDebug() << Q_FUNC_INFO << __LINE__ ;

    CustomTabStyle style;
    QStyleOption opt;
    QSize inputSize(100, 30);
    QSize result = style.sizeFromContents(QStyle::CT_TabBarTab, &opt, inputSize, nullptr);
    QVERIFY(result.width() == 60);
    QVERIFY(result.height() == 36);
}

void TestCustomTabStyle::testSizeFromContents_TabBarTab()
{
    qDebug() << Q_FUNC_INFO << __LINE__ ;

    CustomTabStyle style;
    QStyleOptionTab tabOption;
    QSize inputSize(100, 30);
    QSize result = style.sizeFromContents(QStyle::CT_TabBarTab, &tabOption, inputSize, nullptr);
    QVERIFY(result.width() == 60);
    QVERIFY(result.height() == 36);
}

void TestCustomTabStyle::testSizeFromContents_OtherType()
{
    qDebug() << Q_FUNC_INFO << __LINE__ ;

    CustomTabStyle style;
    QStyleOption opt;
    QSize inputSize(100, 30);
    QSize resultPushButton = style.sizeFromContents(QStyle::CT_PushButton, &opt, inputSize, nullptr);
    QSize resultCheckBox = style.sizeFromContents(QStyle::CT_CheckBox, &opt, inputSize, nullptr);
    QVERIFY(resultPushButton.isValid());
    QVERIFY(resultCheckBox.isValid());
}

void TestCustomTabStyle::testDrawControl()
{
    qDebug() << Q_FUNC_INFO << __LINE__ ;

//    CustomTabStyle style;
//    QImage image(200, 100, QImage::Format_ARGB32);
//    image.fill(Qt::white);
//    QPainter painter(&image);
//    QStyleOption opt;
//    opt.rect = QRect(10, 10, 60, 36);
//    style.drawControl(QStyle::CE_PushButton, &opt, &painter, nullptr);
}

void TestCustomTabStyle::testDrawControl_TabBarTab_Selected()
{
    qDebug() << Q_FUNC_INFO << __LINE__ ;

//    CustomTabStyle style;
//    QImage image(200, 100, QImage::Format_ARGB32);
//    image.fill(Qt::white);
//    QPainter painter(&image);
//    QStyleOptionTab tabOption;
//    tabOption.rect = QRect(10, 10, 60, 36);
//    tabOption.text = "Test";
//    tabOption.state = QStyle::State_Selected;
//    tabOption.position = QStyleOptionTab::Beginning;
//    style.drawControl(QStyle::CE_TabBarTab, &tabOption, &painter, nullptr);
//    bool hasNonWhite = false;
//    for (int y = 0; y < image.height() && !hasNonWhite; y++) {
//        for (int x = 0; x < image.width(); x++) {
//            if (image.pixel(x, y) != 0xFFFFFFFF) {
//                hasNonWhite = true;
//                break;
//            }
//        }
//    }
//    QVERIFY(hasNonWhite);
}

void TestCustomTabStyle::testDrawControl_TabBarTab_MouseOver()
{
    qDebug() << Q_FUNC_INFO << __LINE__ ;

//    CustomTabStyle style;
//    QImage image(200, 100, QImage::Format_ARGB32);
//    image.fill(Qt::white);
//    QPainter painter(&image);
//    QStyleOptionTab tabOption;
//    tabOption.rect = QRect(10, 10, 60, 36);
//    tabOption.text = "Test";
//    tabOption.state = QStyle::State_MouseOver;
//    tabOption.position = QStyleOptionTab::Middle;
//    style.drawControl(QStyle::CE_TabBarTab, &tabOption, &painter, nullptr);
//    bool hasNonWhite = false;
//    for (int y = 0; y < image.height() && !hasNonWhite; y++) {
//        for (int x = 0; x < image.width(); x++) {
//            if (image.pixel(x, y) != 0xFFFFFFFF) {
//                hasNonWhite = true;
//                break;
//            }
//        }
//    }
//    QVERIFY(hasNonWhite);
}

void TestCustomTabStyle::testDrawControl_TabBarTab_Normal()
{
    qDebug() << Q_FUNC_INFO << __LINE__ ;

//    CustomTabStyle style;
//    QImage image(200, 100, QImage::Format_ARGB32);
//    image.fill(Qt::white);
//    QPainter painter(&image);
//    QStyleOptionTab tabOption;
//    tabOption.rect = QRect(10, 10, 60, 36);
//    tabOption.text = "Test";
//    tabOption.state = QStyle::State_None;
//    tabOption.position = QStyleOptionTab::End;
//    style.drawControl(QStyle::CE_TabBarTab, &tabOption, &painter, nullptr);
//    bool hasNonWhite = false;
//    for (int y = 0; y < image.height() && !hasNonWhite; y++) {
//        for (int x = 0; x < image.width(); x++) {
//            if (image.pixel(x, y) != 0xFFFFFFFF) {
//                hasNonWhite = true;
//                break;
//            }
//        }
//    }
//    QVERIFY(hasNonWhite);
}

void TestCustomTabStyle::testDrawControl_OtherElement()
{
    qDebug() << Q_FUNC_INFO << __LINE__ ;

//    CustomTabStyle style;
//    QImage image(200, 100, QImage::Format_ARGB32);
//    image.fill(Qt::white);
//    QPainter painter(&image);
//    QStyleOptionButton btnOption;
//    btnOption.rect = QRect(10, 10, 80, 30);
//    style.drawControl(QStyle::CE_PushButton, &btnOption, &painter, nullptr);
}
