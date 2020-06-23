/*
 * Copyright (C) 2020 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/&gt;.
 *
 */

#include "utils.h"

#include <sys/syslog.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

///////////////////////////////////////////////////////////////////////////////
// The Utils class

Utils::Utils(){}

int Utils::m_system(char *cmd)
{
    int status = 0;
    pid_t pid;

    if ((pid = vfork()) <0) {
        qDebug()<<"failed to create a subprocess by using vfork";
        syslog(LOG_ERR, "failed to create a subprocess by using vfork");
        status = -1;
    } else if (pid==0) {
        const char *new_argv[4];
        struct sigaction sa_cld;
        sa_cld.sa_handler = SIG_DFL;
        sa_cld.sa_flags = 0;

        // 在子进程中放开SIGINT信号
        sigemptyset(&sa_cld.sa_mask);
        sigaction (SIGINT, &sa_cld, NULL);
        sigaction (SIGQUIT, &sa_cld, NULL);

        new_argv[0] = "sh";
        new_argv[1] = "-c";
        new_argv[2] = cmd;
        new_argv[3] = NULL;

        // execl("/bin/sh","sh","-c" ,cmd,(char *)0);
        if (execve("/bin/sh",(char *const *) new_argv, NULL) <0) {
            qDebug()<<"failed to execve a shell command in function m_system";
            syslog(LOG_ERR, "failed to execve %s! errno: %d\n",cmd, errno);
            exit(1);
        } else {
            exit(0);
        }
    } else {
        waitpid(pid,&status,0);
    }

    return status;
}


///////////////////////////////////////////////////////////////////////////////
// The NetworkSpeed class, used to get network speed

NetworkSpeed::NetworkSpeed(QObject *parent) :QObject(parent){}

int NetworkSpeed::getCurrentDownloadRates(char *netname, long *save_rate, long *tx_rate)
{
    FILE * net_dev_file; //文件指针
    char buffer[1024]; //文件中的内容暂存在字符缓冲区里
    //size_t bytes_read; //实际读取的内容大小
    char * match; //用以保存所匹配字符串及之后的内容
    int counter = 0;
    //int i = 0;
    char tmp_value[128];

    if((NULL == netname)||(NULL == save_rate)||(NULL == tx_rate)) {
        qDebug()<<"parameter pass to function getCurrentDownloadRates() error";
        syslog(LOG_ERR, "parameter pass to function getCurrentDownloadRates() error");
        return -1;
    }

    if ( (net_dev_file=fopen("/proc/net/dev", "r")) == NULL ) { //打开文件/pro/net/dev/，从中读取流量数据
        qDebug()<<"error occurred when try to open file /proc/net/dev/";
        syslog(LOG_ERR, "error occurred when try to open file /proc/net/dev/");
        return -1;
    }
    memset(buffer,0,sizeof(buffer));

    while (fgets(buffer,sizeof(buffer),net_dev_file) != NULL) {
        match = strstr(buffer,netname);

        if (NULL == match) {
            // qDebug()<<"No eth0 keyword to find!";
            continue;
        } else {
            match = match + strlen(netname) + strlen(":"); //地址偏移到冒号
            sscanf(match,"%ld ",save_rate);
            memset(tmp_value,0,sizeof(tmp_value));
            sscanf(match,"%s ",tmp_value);
            match = match + strlen(tmp_value);
            for (size_t i=0;i<strlen(buffer);i++) {
                if (0x20 == *match) {
                    match ++;
                } else {
                    if (8 == counter) {
                        sscanf(match,"%ld ",tx_rate);
                    }
                    memset(tmp_value,0,sizeof(tmp_value));
                    sscanf(match,"%s ",tmp_value);
                    match = match + strlen(tmp_value);
                    counter ++;
                }
            }
        }
    }

    return 0; //返回成功
}


///////////////////////////////////////////////////////////////////////////////
// The CustomStyle class, inherit from QProxyStyle, used to change style of control

CustomStyle::CustomStyle(const QString &proxyStyleName, QObject *parent) : QProxyStyle (proxyStyleName)
{

}

void CustomStyle::drawComplexControl(QStyle::ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
    return QProxyStyle::drawComplexControl(control, option, painter, widget);
}

void CustomStyle::drawControl(QStyle::ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    return QProxyStyle::drawControl(element, option, painter, widget);
}

void CustomStyle::drawItemPixmap(QPainter *painter, const QRect &rectangle, int alignment, const QPixmap &pixmap) const
{
    return QProxyStyle::drawItemPixmap(painter, rectangle, alignment, pixmap);
}

void CustomStyle::drawItemText(QPainter *painter, const QRect &rectangle, int alignment, const QPalette &palette, bool enabled, const QString &text, QPalette::ColorRole textRole) const
{
    return QProxyStyle::drawItemText(painter, rectangle, alignment, palette, enabled, text, textRole);
}

void CustomStyle::drawPrimitive(QStyle::PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    return QProxyStyle::drawPrimitive(element, option, painter, widget);
}

QPixmap CustomStyle::generatedIconPixmap(QIcon::Mode iconMode, const QPixmap &pixmap, const QStyleOption *option) const
{
    return QProxyStyle::generatedIconPixmap(iconMode, pixmap, option);
}

QStyle::SubControl CustomStyle::hitTestComplexControl(QStyle::ComplexControl control, const QStyleOptionComplex *option, const QPoint &position, const QWidget *widget) const
{
    return QProxyStyle::hitTestComplexControl(control, option, position, widget);
}

QRect CustomStyle::itemPixmapRect(const QRect &rectangle, int alignment, const QPixmap &pixmap) const
{
    return QProxyStyle::itemPixmapRect(rectangle, alignment, pixmap);
}

QRect CustomStyle::itemTextRect(const QFontMetrics &metrics, const QRect &rectangle, int alignment, bool enabled, const QString &text) const
{
    return QProxyStyle::itemTextRect(metrics, rectangle, alignment, enabled, text);
}

int CustomStyle::pixelMetric(QStyle::PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
{
    return QProxyStyle::pixelMetric(metric, option, widget);
}

void CustomStyle::polish(QWidget *widget)
{
    return QProxyStyle::polish(widget);
}

void CustomStyle::polish(QApplication *application)
{
    return QProxyStyle::polish(application);
}

void CustomStyle::polish(QPalette &palette)
{
    return QProxyStyle::polish(palette);
}

void CustomStyle::unpolish(QWidget *widget)
{
    return QProxyStyle::unpolish(widget);
}

void CustomStyle::unpolish(QApplication *application)
{
    return QProxyStyle::unpolish(application);
}

QSize CustomStyle::sizeFromContents(QStyle::ContentsType type, const QStyleOption *option, const QSize &contentsSize, const QWidget *widget) const
{
    return QProxyStyle::sizeFromContents(type, option, contentsSize, widget);
}

QIcon CustomStyle::standardIcon(QStyle::StandardPixmap standardIcon, const QStyleOption *option, const QWidget *widget) const
{
    return QProxyStyle::standardIcon(standardIcon, option, widget);
}

QPalette CustomStyle::standardPalette() const
{
    return QProxyStyle::standardPalette();
}

int CustomStyle::styleHint(QStyle::StyleHint hint, const QStyleOption *option, const QWidget *widget, QStyleHintReturn *returnData) const
{
    return QProxyStyle::styleHint(hint, option, widget, returnData);
}

QRect CustomStyle::subControlRect(QStyle::ComplexControl control, const QStyleOptionComplex *option, QStyle::SubControl subControl, const QWidget *widget) const
{
    return QProxyStyle::subControlRect(control, option, subControl, widget);
}

QRect CustomStyle::subElementRect(QStyle::SubElement element, const QStyleOption *option, const QWidget *widget) const
{
    return QProxyStyle::subElementRect(element, option, widget);
}
