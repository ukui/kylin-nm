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

#include "kylinheadfile.h"

MyQss::MyQss()
{
    labelQss = "QLabel{border:0px;color:rgba(255,255,255,0.97);background-color:transparent;}";
    cbxQss = "QComboBox{padding-left:20px;font-size:13px;color:rgba(255,255,255,0.91);"
                        "border:1px solid rgba(255, 255, 255, 0.05);border-radius:4px;background:rgba(255,255,255,0.08);}"
             "QComboBox::drop-down{border:0px;width:30px;}"
             "QComboBox::down-arrow{image:url(:/res/g/down_arrow.png);}"
             "QComboBox QAbstractItemView {margin:0px 0px 0px 0px;padding: 0px 0px;border-radius:0px;background-color:#48484C;outline:0px;}"
             "QComboBox QAbstractItemView::item{border-radius:0px;font-size:13px;color:rgba(255,255,255,0.91);height: 32px;background-color:#48484C;outline:0px;}"
             "QComboBox QAbstractItemView::item:hover{border-radius:0px;font-size:13px;color:rgba(255,255,255,0.91);background-color:#3D6BE5;outline:0px;}";
    leQss = "QLineEdit{padding-left:20px;color:rgba(255,255,255,0.97);background:rgba(255,255,255,0.08);}";
    checkBoxQss = "QCheckBox {border:none;background:transparent;}"
                  "QCheckBox::indicator {width: 18px; height: 9px;}"
                  "QCheckBox::indicator:checked {image: url(:/res/h/show-pwd.png);}"
                  "QCheckBox::indicator:unchecked {image: url(:/res/h/hide-pwd.png);}";
    checkBoxCAQss = "QCheckBox{boder:none;color:rgba(255,255,255,0.97);background-color:transparent;}";
    btnCancelQss = "QPushButton{border:0px;border-radius:4px;background-color:rgba(255,255,255,0.12);color:white;font-size:14px;}"
                   "QPushButton:Hover{border:0px solid rgba(255,255,255,0.2);border-radius:4px;background-color:rgba(255,255,255,0.2);}"
                   "QPushButton:Pressed{border-radius:4px;background-color:rgba(255,255,255,0.08);}";
    btnConnQss = "QPushButton{border:0px;border-radius:4px;background-color:rgba(61,107,229,1);color:white;font-size:14px;}"
                 "QPushButton:Hover{border:0px solid rgba(255,255,255,0.2);border-radius:4px;background-color:rgba(107,142,235,1);}"
                 "QPushButton:Pressed{border-radius:4px;background-color:rgba(50,87,202,1);}";
    lineQss = "background:rgba(255,255,255,0.08);";
}
