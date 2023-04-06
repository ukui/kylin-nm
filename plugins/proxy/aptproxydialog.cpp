#include "aptproxydialog.h"
#include <QDebug>
#include <QLineEdit>
#include <QSpacerItem>
#include <QLabel>
#include <QPushButton>
#include "proxy.h"

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
    setWindowTitle(tr("Set Apt Proxy"));
    this->setFixedSize(480, 200);

    QVBoxLayout *mAptProxyLyt = new QVBoxLayout(this);
    mAptProxyLyt->setContentsMargins(24, 24, 24, 24);
    mAptProxyLyt->setSpacing(16);

    QFrame *mHostFrame = new QFrame(this);
    mHostFrame->setFixedSize(432, 36);
    mHostFrame->setFrameShape(QFrame::NoFrame);

    QHBoxLayout *mLyt_1= new QHBoxLayout(mHostFrame);
    mLyt_1->setContentsMargins(0, 0, 0, 0);
    mLyt_1->setSpacing(8);

    FixLabel *mSetHostLabel = new FixLabel(mHostFrame);
    mSetHostLabel->setFixedSize(92, 36);
    mSetHostLabel->setText(tr("Server Address"));

    mHostEdit = new QLineEdit(mHostFrame);
    mHostEdit->setAttribute(Qt::WA_InputMethodEnabled, false);  //限制中文输入法
    mHostEdit->setFixedSize(332, 36);
    mHostEdit->installEventFilter(this);

    mLyt_1->addWidget(mSetHostLabel);
    mLyt_1->addWidget(mHostEdit);

    QFrame *mPortFrame = new QFrame(this);
    mPortFrame->setFixedSize(432, 36);
    mPortFrame->setFrameShape(QFrame::NoFrame);

    QHBoxLayout *mLyt_2= new QHBoxLayout(mPortFrame);
    mLyt_2->setContentsMargins(0, 0, 0, 0);
    mLyt_2->setSpacing(8);

    QLabel *mSetPortLabel = new QLabel(tr("Port") ,mPortFrame);
    mSetPortLabel->setFixedSize(92, 36);

    mPortEdit = new QLineEdit(mPortFrame);
    mPortEdit->setAttribute(Qt::WA_InputMethodEnabled, false);  //限制中文输入法
    mPortEdit->setFixedSize(332, 36);
    mPortEdit->installEventFilter(this);

    mLyt_2->addWidget(mSetPortLabel);
    mLyt_2->addWidget(mPortEdit);

    QFrame *mChooseFrame = new QFrame(this);
    mChooseFrame->setFixedWidth(432);
    mChooseFrame->setFrameShape(QFrame::NoFrame);

    QHBoxLayout *mLyt_3= new QHBoxLayout(mChooseFrame);
    mLyt_3->setContentsMargins(0, 0, 0, 0);
    mLyt_3->setSpacing(16);

    mCancelBtn = new QPushButton(mChooseFrame);
    mCancelBtn->setMinimumWidth(96);
    mCancelBtn->setText(tr("Cancel"));

    mConfirmBtn = new QPushButton(mChooseFrame);
    mConfirmBtn->setMinimumWidth(96);
    mConfirmBtn->setText(tr("Confirm"));

    mLyt_3->addStretch();
    mLyt_3->addWidget(mCancelBtn);
    mLyt_3->addWidget(mConfirmBtn);

    mAptProxyLyt->addWidget(mHostFrame);
    mAptProxyLyt->addWidget(mPortFrame);
    mAptProxyLyt->addSpacing(16);
    mAptProxyLyt->addWidget(mChooseFrame);
}

void AptProxyDialog::initConnect()
{
    connect(mHostEdit, &QLineEdit::textEdited, this, [=]() {
        if (mHostEdit->text().isEmpty()) {
             mConfirmBtn->setEnabled(false);
        } else {
            mConfirmBtn->setEnabled(true);
        }
    });

    connect(mCancelBtn, &QPushButton::clicked, this, [=]() {
        this->close();
    });

    connect(mConfirmBtn, &QPushButton::clicked, this, [=]() {
        Proxy::setAptProxy(mHostEdit->text() , mPortEdit->text() , true);
        this->close();
    });
}

void AptProxyDialog::setupComponent()
{
    QString host = Proxy::getAptProxy()["ip"].toString();
    QString port = Proxy::getAptProxy()["port"].toString();;

    mHostEdit->setText(host);
    mPortEdit->setText(port);

    if (host.isEmpty()) {
        mConfirmBtn->setEnabled(false);
    }
}
