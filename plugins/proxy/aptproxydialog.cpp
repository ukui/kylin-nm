/*
 *
 * Copyright (C) 2023, KylinSoft Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *
 */
#include "aptproxydialog.h"
#include <QDebug>
#include <QLineEdit>
#include <QSpacerItem>
#include <QLabel>
#include <QPushButton>
#include <QGSettings/QGSettings>

#include "proxy.h"
#include "klabel.h"

using namespace kdk;
AptProxyDialog::AptProxyDialog(QWidget *parent ):
    QDialog(parent)
{
    initUi();
    setupComponent();
    initConnect();
}

AptProxyDialog::~AptProxyDialog()
{

}

void AptProxyDialog::initUi()
{
    setWindowTitle(tr("Set APT Proxy"));
    this->setFixedSize(480, 300);

    QVBoxLayout *mAptProxyLyt = new QVBoxLayout(this);
    mAptProxyLyt->setContentsMargins(24, 24, 24, 24);
    mAptProxyLyt->setSpacing(16);

    QFrame *mHttpHostFrame = new QFrame(this);
    mHttpHostFrame->setFixedSize(432, 36);
    mHttpHostFrame->setFrameShape(QFrame::NoFrame);

    QHBoxLayout *mLyt_1= new QHBoxLayout(mHttpHostFrame);
    mLyt_1->setContentsMargins(0, 0, 0, 0);
    mLyt_1->setSpacing(8);

    KLabel *mSetHttpHostLabel = new KLabel(mHttpHostFrame);
    mSetHttpHostLabel->setFixedSize(92, 36);
    mSetHttpHostLabel->setText(tr("Http Address"));

    m_pHttpHostEdit = new QLineEdit(mHttpHostFrame);
    m_pHttpHostEdit->setAttribute(Qt::WA_InputMethodEnabled, false);  //限制中文输入法
    m_pHttpHostEdit->setFixedSize(332, 36);
    m_pHttpHostEdit->installEventFilter(this);

    mLyt_1->addWidget(mSetHttpHostLabel);
    mLyt_1->addWidget(m_pHttpHostEdit);

    QFrame *mHttpPortFrame = new QFrame(this);
    mHttpPortFrame->setFixedSize(432, 36);
    mHttpPortFrame->setFrameShape(QFrame::NoFrame);

    QHBoxLayout *mLyt_2= new QHBoxLayout(mHttpPortFrame);
    mLyt_2->setContentsMargins(0, 0, 0, 0);
    mLyt_2->setSpacing(8);

    KLabel *mSetHttpPortLabel = new KLabel(mHttpPortFrame);
    mSetHttpPortLabel->setFixedSize(92, 36);
    mSetHttpPortLabel->setText(tr("Http Port"));

    m_pHttpPortEdit = new QLineEdit(mHttpPortFrame);
    m_pHttpPortEdit->setAttribute(Qt::WA_InputMethodEnabled, false);  //限制中文输入法
    m_pHttpPortEdit->setFixedSize(332, 36);
    m_pHttpPortEdit->installEventFilter(this);

    mLyt_2->addWidget(mSetHttpPortLabel);
    mLyt_2->addWidget(m_pHttpPortEdit);

    QFrame *mHttpsHostFrame = new QFrame(this);
    mHttpsHostFrame->setFixedSize(432, 36);
    mHttpsHostFrame->setFrameShape(QFrame::NoFrame);

    QHBoxLayout *mLyt_3= new QHBoxLayout(mHttpsHostFrame);
    mLyt_3->setContentsMargins(0, 0, 0, 0);
    mLyt_3->setSpacing(8);

    KLabel *mSetHttpsHostLabel = new KLabel(mHttpsHostFrame);
    mSetHttpsHostLabel->setFixedSize(92, 36);
    mSetHttpsHostLabel->setText(tr("Https Address"));

    m_pHttpsHostEdit = new QLineEdit(mHttpsHostFrame);
    m_pHttpsHostEdit->setAttribute(Qt::WA_InputMethodEnabled, false);  //限制中文输入法
    m_pHttpsHostEdit->setFixedSize(332, 36);
    m_pHttpsHostEdit->installEventFilter(this);

    mLyt_3->addWidget(mSetHttpsHostLabel);
    mLyt_3->addWidget(m_pHttpsHostEdit);

    QFrame *mHttpsPortFrame = new QFrame(this);
    mHttpsPortFrame->setFixedSize(432, 36);
    mHttpsPortFrame->setFrameShape(QFrame::NoFrame);

    QHBoxLayout *mLyt_4= new QHBoxLayout(mHttpsPortFrame);
    mLyt_4->setContentsMargins(0, 0, 0, 0);
    mLyt_4->setSpacing(8);

    QLabel *mSetHttpsPortLabel = new KLabel(mHttpsPortFrame);
    mSetHttpsPortLabel->setText(tr("Https Port"));
    mSetHttpsPortLabel->setFixedSize(92, 36);

    m_pHttpsPortEdit = new QLineEdit(mHttpsHostFrame);
    m_pHttpsPortEdit->setAttribute(Qt::WA_InputMethodEnabled, false);  //限制中文输入法
    m_pHttpsPortEdit->setFixedSize(332, 36);
    m_pHttpsPortEdit->installEventFilter(this);

    mLyt_4->addWidget(mSetHttpsPortLabel);
    mLyt_4->addWidget(m_pHttpsPortEdit);

    QFrame *mChooseFrame = new QFrame(this);
    mChooseFrame->setFixedWidth(432);
    mChooseFrame->setFrameShape(QFrame::NoFrame);

    QHBoxLayout *mLyt_6= new QHBoxLayout(mChooseFrame);
    mLyt_6->setContentsMargins(0, 0, 0, 0);
    mLyt_6->setSpacing(16);

    mCancelBtn = new QPushButton(mChooseFrame);
    mCancelBtn->setMinimumWidth(96);
    mCancelBtn->setText(tr("Cancel"));

    mConfirmBtn = new QPushButton(mChooseFrame);
    mConfirmBtn->setMinimumWidth(96);
    mConfirmBtn->setText(tr("Confirm"));
    mConfirmBtn->setProperty("isImportant", true);/* v11引导操作设置高亮，link bug#452573 */

    mLyt_6->addStretch();
    mLyt_6->addWidget(mCancelBtn);
    mLyt_6->addWidget(mConfirmBtn);

    mAptProxyLyt->addWidget(mHttpHostFrame);
    mAptProxyLyt->addWidget(mHttpPortFrame);
    mAptProxyLyt->addWidget(mHttpsHostFrame);
    mAptProxyLyt->addWidget(mHttpsPortFrame);
    mAptProxyLyt->addSpacing(16);
    mAptProxyLyt->addWidget(mChooseFrame);
}

void AptProxyDialog::initConnect()
{
    connect(m_pHttpHostEdit, &QLineEdit::textEdited, this, [=]() {
        if (m_pHttpHostEdit->text().isEmpty() && m_pHttpsHostEdit->text().isEmpty()) {
             mConfirmBtn->setEnabled(false);
        } else {
            mConfirmBtn->setEnabled(true);
        }
    });
    connect(m_pHttpsHostEdit, &QLineEdit::textEdited, this, [=]() {
        if (m_pHttpsHostEdit->text().isEmpty() && m_pHttpHostEdit->text().isEmpty()) {
             mConfirmBtn->setEnabled(false);
        } else {
            mConfirmBtn->setEnabled(true);
        }
    });

    connect(mCancelBtn, &QPushButton::clicked, this, [=]() {
        this->close();
    });

    connect(mConfirmBtn, &QPushButton::clicked, this, [=]() {
        Proxy::setAptProxy(m_pHttpHostEdit->text(), m_pHttpPortEdit->text(), m_pHttpsHostEdit->text(), m_pHttpsPortEdit->text(), true);
        this->close();
    });
}

void AptProxyDialog::setupComponent()
{
    QString httpHost = Proxy::getAptProxy()["http_ip"].toString();
    QString httpPort = Proxy::getAptProxy()["http_port"].toString();
    QString httpsHost = Proxy::getAptProxy()["https_ip"].toString();
    QString httpsPort = Proxy::getAptProxy()["https_port"].toString();

    m_pHttpHostEdit->setText(httpHost);
    m_pHttpPortEdit->setText(httpPort);
    m_pHttpsHostEdit->setText(httpsHost);
    m_pHttpsPortEdit->setText(httpsPort);

    if (httpHost.isEmpty() || httpsHost.isEmpty()) {
        mConfirmBtn->setEnabled(false);
    }
}
