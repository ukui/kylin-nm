import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.3
import org.ukui.shortcut.network 1.0

import org.ukui.quick.items 1.0 as UkuiItems
import org.ukui.quick.platform 1.0 as Platform

UkuiItems.DtThemeBackground {
    id: root
    backgroundColor: Platform.GlobalTheme.kContainSecondaryAlphaNormal
    radius: Platform.GlobalTheme.kRadiusWindow

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        RowLayout {
            id: nttop
            width: parent.width
            Layout.alignment: Qt.AlignTop
            Layout.preferredHeight: 56

            Item { Layout.fillWidth: true }
        }

        Rectangle {
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            Layout.preferredHeight: 1
            height: 1
            color: Qt.rgba(0, 0, 0, 0.06)
        }
        ScrollView {
            visible: true
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            Layout.fillHeight: true
            ScrollBar.vertical.policy: ScrollBar.AsNeeded
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            clip: true

            Flickable {
                anchors.fill: parent
                contentWidth : parent.width
                contentHeight: wlanTop.height + lanTop.height + (lanDeviceComboBox.visible ? lanDeviceComboBox.height : 0) + (lanContentArea.visible ? lanContentArea.height : 0) + (wlanDeviceComboBox.visible ? wlanDeviceComboBox.height : 0) + (wlanContentArea.visible ? wlanContentArea.height : 0) + 40

                ColumnLayout {
                    width: parent.width

                    RowLayout {
                        id : lanTop
                        width: parent.width
                        Layout.alignment: Qt.AlignTop

                        Label {
                            text: qsTr("有线网络")
                            font.pixelSize: 14
                            Layout.alignment: Qt.AlignLeft
                            Layout.leftMargin: 24
                        }

                        UkuiItems.Icon {
                            id: lanVisibleButton
                            source: "file:///usr/share/ukui/widgets/org.ukui.shortcut.network/ukui-up-symbolic.svg"
                            width: 16
                            height: 16
                            mode: UkuiItems.Icon.AutoHighlight
                            property bool visibleState : true
                            MouseArea {
                                anchors.fill: lanVisibleButton
                                hoverEnabled: true
                                onClicked: {
                                    lanVisibleButton.visibleState = !lanVisibleButton.visibleState;
                                    if (lanVisibleButton.visibleState) {
                                        lanVisibleButton.source = "file:///usr/share/ukui/widgets/org.ukui.shortcut.network/ukui-up-symbolic.svg"
                                    } else {
                                        lanVisibleButton.source = "file:///usr/share/ukui/widgets/org.ukui.shortcut.network/arrow-down.svg"
                                    }
                                    if (!lanswitchBtn.checked)
                                        return
                                    lanContentArea.visible = lanVisibleButton.visibleState
                                    lanDeviceComboBox.visible = lanVisibleButton.visibleState && !lanDeviceComboBox.ishide && (lanDeviceComboBox.count >= 2)
                                    lanNoWiredItem.visible = lanVisibleButton.visibleState && (lanDeviceComboBox.count === 0)
                                }
                            }
                        }

                        Item {
                            Layout.fillWidth: true
                        }

                        SwitchDelegate {
                            id: lanswitchBtn
                            checked: KInterface.wiredMainSwitch
                            enabled: (lanDeviceComboBox.count !== 0)
                            onClicked: {
                                KInterface.wiredMainSwitch = lanswitchBtn.checked

                                if (lanVisibleButton.visibleState) {
                                    lanContentArea.visible = lanswitchBtn.checked
                                    lanDeviceComboBox.visible = lanswitchBtn.checked && !lanDeviceComboBox.ishide && (lanDeviceComboBox.count >= 2)
                                } else {
                                    lanContentArea.visible = false
                                    lanDeviceComboBox.visible = false
                                }
                            }
                            Layout.alignment: Qt.AlignRight
                            Layout.rightMargin: 10
                        }
                    }



                    Rectangle {
                        id: lanNoWiredItem
                        Layout.fillWidth: true
                        Layout.preferredHeight: 60
                        Layout.alignment: Qt.AlignTop
                        visible: lanDeviceComboBox.count === 0
                        Image {
                            width: 16
                            height: 16
                            anchors.right: lanNoWiredText.left
                            Layout.rightMargin: 5
                            anchors.verticalCenter: parent.verticalCenter
                            source: "file:///usr/share/ukui/widgets/org.ukui.shortcut.network/dialog-warning.svg"
                        }
                        Label {
                            id: lanNoWiredText
                            anchors.leftMargin: 20
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: qsTr("未检测到有线网卡")
                        }
                    }

                    DeviceComboBox {
                        id: lanDeviceComboBox
                        width: parent.width
                        spacing: 0
                        model: KInterface.wiredDevList
                        visible: lanswitchBtn.checked && lanVisibleButton.visibleState && !lanNoWiredItem.visible && (lanDeviceComboBox.count>=2)
                        property bool ishide : false
                        onCurrentTextChanged: {
                            KInterface.getWiredDevConnList(lanDeviceComboBox.currentText);
                        }
                        Connections {
                            target: KInterface
                            onUpdateWiredDeviceList: {
                                if (lanDeviceComboBox.count <= 0) {
                                    lanDeviceComboBox.ishide = true
                                    lanContentArea.visible = false
                                    lanNoWiredItem.visible = true
                                    lanDeviceComboBox.enabled = false;
                                } else {
                                    lanNoWiredItem.visible = false
                                    lanDeviceComboBox.enabled = true;
                                }
                                if (lanDeviceComboBox.count <= 1) {
                                    lanDeviceComboBox.ishide = true
                                    lanDeviceComboBox.visible = false
                                } else {
                                    lanDeviceComboBox.ishide = false
                                    lanContentArea.visible = true && lanswitchBtn.checked && lanVisibleButton.visibleState  && !lanNoWiredItem.visible
                                    lanDeviceComboBox.visible = true && lanswitchBtn.checked && lanVisibleButton.visibleState  && !lanNoWiredItem.visible
                                }
                                KInterface.getWiredDevConnList(lanDeviceComboBox.currentText);
                            }
                        }
                    }

                    LanComponent{
                        id: lanContentArea
                        Layout.alignment: Qt.AlignTop
                        interactive: false
                        implicitHeight: lanContentArea.contentHeight
                        width: 396
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.leftMargin: 16
                        Layout.rightMargin: 16
                        height: 1
                        color: Qt.rgba(0, 0, 0, 0.06)
                    }

                    RowLayout {
                        id : wlanTop
                        width: parent.width
                        Layout.alignment: Qt.AlignTop

                        Label {
                            text: qsTr("无线网络")
                            font.pixelSize: 14
                            Layout.alignment: Qt.AlignLeft
                            Layout.leftMargin: 24
                        }

                        UkuiItems.Icon {
                            id: wlanRescanButton
                            source: "file:///usr/share/ukui/widgets/org.ukui.shortcut.network/view-wrapped-symbolic.svg"
                            width: 16
                            height: 16
                            mode: UkuiItems.Icon.AutoHighlight
                            visible: wlanswitchBtn.checked && (wlanDeviceComboBox.count >= 1)

                            MouseArea {
                                anchors.fill: parent
                                hoverEnabled: true
                                onClicked: {
                                    srotation.start()
                                    animationTimer.start()
                                    KInterface.rescanWirelessConn()
                                }
                            }
                        }

                        RotationAnimator {
                            id: srotation
                            target: wlanRescanButton
                            from: 0
                            to: 360
                            duration: 2000 // 旋转一圈需要的时间，单位为毫秒
                            loops: Animation.Infinite // 无限循环
                        }

                        Timer {
                            id: animationTimer
                            interval: 5000 // 计时器间隔时间，单位为毫秒（1000毫秒 = 1秒）
                            repeat: false // 是否重复计时
                            running: false // 默认不启动计时器
                            onTriggered: {
                                // 这里定义计时器触发时执行的代码
                                srotation.stop()
                            }
                        }

                        Item { Layout.fillWidth: true }

                        SwitchDelegate {
                            id: wlanswitchBtn
                            checked: KInterface.wirelessSwitch
                            enabled: (wlanDeviceComboBox.count >= 1)
                            onClicked: {
                                if(!enabled)
                                {
                                    wlanswitchBtn.checked=false
                                    KInterface.wirelessSwitch = wlanswitchBtn.checked
                                    return
                                }
                                KInterface.wirelessSwitch = wlanswitchBtn.checked
                                wlanDeviceComboBox.visible = !wlanDeviceComboBox.ishide && wlanswitchBtn.checked && (wlanDeviceComboBox.count >= 2)
                                wlanContentArea.visible = wlanswitchBtn.checked
                            }
                            Layout.alignment: Qt.AlignRight
                            Layout.rightMargin: 10
                        }
                    }

                    Item { Layout.fillWidth: true }

                    DeviceComboBox {
                        id: wlanDeviceComboBox
                        spacing: 0
                        visible: wlanswitchBtn.checked && wlanDeviceComboBox.count > 1
                        model: KInterface.wirelessDevList
                        property bool ishide : wlanDeviceComboBox.count > 1
                        onCurrentTextChanged: {
                            KInterface.getWirelessDevConnList(wlanDeviceComboBox.currentText);
                        }
                        Connections {
                            target: KInterface
                            onUpdateWirelessDeviceList: {
                                if (wlanDeviceComboBox.count <= 1) {
                                    wlanDeviceComboBox.ishide = true
                                    wlanDeviceComboBox.visible = false
                                } else {
                                    wlanDeviceComboBox.ishide = false
                                    wlanContentArea.visible = true && wlanswitchBtn.checked
                                    wlanDeviceComboBox.visible = true && wlanswitchBtn.checked
                                }
                                if (wlanDeviceComboBox.count <= 0) {
                                    wlanDeviceComboBox.ishide = true
                                    wlanContentArea.visible = false
                                }
                                KInterface.getWirelessDevConnList(wlanDeviceComboBox.currentText);
                            }
                        }
                    }

                    WlanComponent{
                        id: wlanContentArea
                        Layout.alignment: Qt.AlignBottom
                        interactive: false
                        implicitHeight: wlanContentArea.contentHeight
                        width: 396
                    }
                }
            }
        }

        Rectangle {
            Layout.alignment: Qt.AlignBottom
            Layout.fillWidth: true
            Layout.bottomMargin: 1
            height: 1
            color: Qt.rgba(0, 0, 0, 0.06)
        }
       Item {
            Layout.fillWidth: true
            Layout.preferredHeight: 56
            Layout.alignment: Qt.AlignBottom
            opacity: Theme.windowOpacity

            Rectangle {
                id: networkSettingRec
                width: parent.width
                height: parent.height
                color: "#00000000"
                opacity: Theme.windowOpacity

                Label {
                    id: networkSettingText
                    height: 22
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 24
                    text: qsTr("更多网络设置")
                    font.pixelSize: 14

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            console.log("NetworkSettingArea.qml onClicked")
                            KInterface.openNetworkSetting()
                        }
                    }
                }
            }
		}
        //UkuiItems.DtThemeBackground {
        //    Layout.fillWidth: true
        //    Layout.preferredHeight: 56
        //    Layout.alignment: Qt.AlignBottom
        //    backgroundColor: Platform.GlobalTheme.baseActive
        //    alpha: 0.6
        //    Label {
        //        anchors.left: parent.left
        //        anchors.leftMargin: 20
        //        anchors.verticalCenter: parent.verticalCenter
        //        text: qsTr("更多网络设置")
        //    }

        //    MouseArea {
        //        anchors.fill: parent
        //        onClicked: {
        //            KInterface.openNetworkSetting()
        //        }
        //    }
        // }

//        focus: true

//        Keys.onEscapePressed: {
//            close() // 按下Escape键时关闭
//        }
    }
}

