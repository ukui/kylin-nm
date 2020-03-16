#include "kylinheadfile.h"

MyQss::MyQss()
{
    labelQss = "QLabel{border:0px;color:rgba(255,255,255,0.97);background-color:transparent;}";
    cbxQss = "QComboBox{padding-left:20px;font-size:13px;color:rgba(255,255,255,0.91);border:1px solid rgba(255, 255, 255, 0.05);background:rgba(255,255,255,0.08);}"
             "QComboBox::drop-down{border:0px;width:30px;}"
             "QComboBox::down-arrow{image:url(:/res/g/down_arrow.png);}"
             "QComboBox QAbstractItemView {border:1px solid rgba(255, 255, 255, 0.05);background-color: transparent;}"
             "QComboBox QAbstractItemView::item{font-size:13px;color:rgba(255,255,255,0.91);height: 32px;background-color: rgba(19,19,20,0.95);}"
             "QComboBox QAbstractItemView::item:selected{font-size:13px;color:rgba(0,0,0,0.91);background-color:lightgray;}";
    leQss = "QLineEdit{padding-left:20px;color:rgba(255,255,255,0.97);background:rgba(255,255,255,0.08);}";
    checkBoxQss = "QCheckBox {border:none;background:transparent;}"
                  "QCheckBox::indicator {width: 18px; height: 9px;}"
                  "QCheckBox::indicator:checked {image: url(:/res/h/show-pwd.png);}"
                  "QCheckBox::indicator:unchecked {image: url(:/res/h/hide-pwd.png);}";
    checkBoxCAQss = "QCheckBox{color:rgba(255,255,255,0.97);background-color:transparent;}";
    btnCancelQss = "QPushButton{border:0px;border-radius:4px;background-color:rgba(255,255,255,0.12);color:white;font-size:14px;}"
                   "QPushButton:Hover{border:0px solid rgba(255,255,255,0.2);border-radius:4px;background-color:rgba(255,255,255,0.2);}"
                   "QPushButton:Pressed{border-radius:4px;background-color:rgba(255,255,255,0.08);}";
    btnConnQss = "QPushButton{border:0px;border-radius:4px;background-color:rgba(61,107,229,1);color:white;font-size:14px;}"
                 "QPushButton:Hover{border:0px solid rgba(255,255,255,0.2);border-radius:4px;background-color:rgba(107,142,235,1);}"
                 "QPushButton:Pressed{border-radius:4px;background-color:rgba(50,87,202,1);}";
    lineQss = "background:rgba(255,255,255,0.08);";
}
