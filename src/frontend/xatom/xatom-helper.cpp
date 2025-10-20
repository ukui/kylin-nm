/*
 * KWin Style UKUI
 *
 * Copyright (C) 2020, KylinSoft Co., Ltd.
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
 * Authors: Yue Lan <lanyue@kylinos.cn>
 *
 */

#include <QGuiApplication>
#include <limits.h>
#include <X11/X.h>
#include <X11/Xatom.h>
#include "xatom-helper.h"

static XAtomHelper *global_instance = nullptr;

XAtomHelper *XAtomHelper::getInstance()
{
    if(!global_instance)
        global_instance = new XAtomHelper;
    return global_instance;
}

bool XAtomHelper::isFrameLessWindow(int winId)
{
    auto hints = getInstance()->getWindowMotifHint(winId);
    if(hints.flags == MWM_HINTS_DECORATIONS && hints.functions == 1) {
        return true;
    }
    return false;
}

bool XAtomHelper::isWindowDecorateBorderOnly(int winId)
{
    return isWindowMotifHintDecorateBorderOnly(getInstance()->getWindowMotifHint(winId));
}

bool XAtomHelper::isWindowMotifHintDecorateBorderOnly(const MotifWmHints &hint) {
    bool isDeco = false;
    if(hint.flags & MWM_HINTS_DECORATIONS && hint.flags != MWM_HINTS_DECORATIONS) {
        if(hint.decorations == MWM_DECOR_BORDER)
            isDeco = true;
    }
    return isDeco;
}

bool XAtomHelper::isUKUICsdSupported()
{
    // fixme:
    return false;
}

bool XAtomHelper::isUKUIDecorationWindow(int winId)
{
    if(m_ukuiDecorationAtion == None)
        return false;

    Atom type;
    int format;
    ulong nitems;
    ulong bytes_after;
    uchar *data;
    Display *display=nullptr;
    bool isUKUIDecoration = false;

    if(auto *x11Application=qGuiApp->nativeInterface<QNativeInterface::QX11Application>()) {
        display=x11Application->display();
        XGetWindowProperty(display, winId, m_ukuiDecorationAtion,
                           0, LONG_MAX, false,
                           m_ukuiDecorationAtion, &type,
                           &format, &nitems,
                           &bytes_after, &data);
    }
    if(type == m_ukuiDecorationAtion) {
        if(nitems == 1) {
            isUKUIDecoration = data[0];
        }
    }

    return isUKUIDecoration;
}

UnityCorners XAtomHelper::getWindowBorderRadius(int winId)
{
    UnityCorners corners;

    Atom type;
    int format;
    ulong nitems;
    ulong bytes_after;
    uchar *data;
    Display *display=nullptr;
    if(m_unityBorderRadiusAtom != None) {
        if(auto *x11Application=qGuiApp->nativeInterface<QNativeInterface::QX11Application>()) {
            display=x11Application->display();
            XGetWindowProperty(display, winId, m_unityBorderRadiusAtom,
                               0, LONG_MAX, false,
                               XA_CARDINAL, &type,
                               &format, &nitems,
                               &bytes_after, &data);

            if(type == XA_CARDINAL) {
                if(nitems == 4) {
                    corners.topLeft = static_cast<ulong>(data[0]);
                    corners.topRight = static_cast<ulong>(data[1 * sizeof(ulong)]);
                    corners.bottomLeft = static_cast<ulong>(data[2 * sizeof(ulong)]);
                    corners.bottomRight = static_cast<ulong>(data[3 * sizeof(ulong)]);
                }
                XFree(data);
            }
        }
    }

    return corners;
}

void XAtomHelper::setWindowBorderRadius(int winId, const UnityCorners &data)
{
    Display *display=nullptr;
    if(m_unityBorderRadiusAtom == None)
        return;

    ulong corners[4] = {data.topLeft, data.topRight, data.bottomLeft, data.bottomRight};
    if(auto *x11Application=qGuiApp->nativeInterface<QNativeInterface::QX11Application>()) {
        display=x11Application->display();
        XChangeProperty(display, winId, m_unityBorderRadiusAtom, XA_CARDINAL,
                        32, XCB_PROP_MODE_REPLACE, (const unsigned char *) &corners, sizeof(corners) / sizeof(corners[0]));
    }
}

void XAtomHelper::setWindowBorderRadius(int winId, int topLeft, int topRight, int bottomLeft, int bottomRight)
{
    Display *display=nullptr;
    if(m_unityBorderRadiusAtom == None)
        return;

    ulong corners[4] = {(ulong)topLeft, (ulong)topRight, (ulong)bottomLeft, (ulong)bottomRight};
    if(auto *x11Application=qGuiApp->nativeInterface<QNativeInterface::QX11Application>()) {
        display=x11Application->display();
        XChangeProperty(display, winId, m_unityBorderRadiusAtom, XA_CARDINAL,
                        32, XCB_PROP_MODE_REPLACE, (const unsigned char *) &corners, sizeof(corners) / sizeof(corners[0]));
    }
}

void XAtomHelper::setUKUIDecoraiontHint(int winId, bool set)
{
    Display *display=nullptr;
    if(m_ukuiDecorationAtion == None)
        return;
    if(auto *x11Application=qGuiApp->nativeInterface<QNativeInterface::QX11Application>()) {
        display=x11Application->display();
        XChangeProperty(display, winId, m_ukuiDecorationAtion, m_ukuiDecorationAtion, 32, XCB_PROP_MODE_REPLACE, (const unsigned char *) &set, 1);
    }
}

void XAtomHelper::setWindowMotifHint(int winId, const MotifWmHints &hints)
{
    Display *display=nullptr;
    if(m_unityBorderRadiusAtom == None)
        return;
    if(auto *x11Application=qGuiApp->nativeInterface<QNativeInterface::QX11Application>()) {
        display=x11Application->display();
        XChangeProperty(display, winId, m_motifWMHintsAtom, m_motifWMHintsAtom,
                        32, XCB_PROP_MODE_REPLACE, (const unsigned char *)&hints, sizeof(MotifWmHints) / sizeof(ulong));
    }
}

MotifWmHints XAtomHelper::getWindowMotifHint(int winId)
{
    MotifWmHints hints;
    Display *display=nullptr;
    if(m_unityBorderRadiusAtom == None)
        return hints;

    uchar *data;
    Atom type;
    int format;
    ulong nitems;
    ulong bytes_after;
    if(auto *x11Application=qGuiApp->nativeInterface<QNativeInterface::QX11Application>()) {
        display=x11Application->display();
        XGetWindowProperty(display, winId, m_motifWMHintsAtom,
                           0, sizeof(MotifWmHints) / sizeof(long), false, AnyPropertyType, &type,
                           &format, &nitems, &bytes_after, &data);
    }

    if(type == None) {
        return hints;
    } else {
        hints = *(MotifWmHints *)data;
        XFree(data);
    }
    return hints;
}

XAtomHelper::XAtomHelper(QObject *parent) : QObject(parent)
{
    Display *display=nullptr;
    if(auto *x11Application=qGuiApp->nativeInterface<QNativeInterface::QX11Application>()) {
        display=x11Application->display();
    m_motifWMHintsAtom = XInternAtom(display, "_MOTIF_WM_HINTS", true);
    m_unityBorderRadiusAtom = XInternAtom(display, "_UNITY_GTK_BORDER_RADIUS", false);
    m_ukuiDecorationAtion = XInternAtom(display, "_KWIN_UKUI_DECORAION", false);
    }
}

Atom XAtomHelper::registerUKUICsdNetWmSupportAtom()
{
    // fixme:
    return None;
}

void XAtomHelper::unregisterUKUICsdNetWmSupportAtom()
{
    // fixme:
}
