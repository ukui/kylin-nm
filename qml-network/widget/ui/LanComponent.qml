import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls.Material 2.3
import org.ukui.shortcut.network 1.0
import org.ukui.quick.items 1.0 as UkuiItems
import org.ukui.quick.platform 1.0 as Platform

ListView {
    id: lanlistView
    visible: lanswitchBtn.checked && lanVisibleButton.visibleState  && !lanNoWiredItem.visible
    model: KInterface.wiredConList
    spacing: 0

    // 定义每个项的显示方式
    delegate: ItemDelegate {
        highlighted: enteritem && lanlistView.currentIndex === index
        width: lanlistView.width
        height: 56
        Layout.leftMargin: 8
        property bool enteritem : false

        RowLayout {
            anchors.fill: parent

            Item {
                Layout.alignment: Qt.AlignLeft
                Layout.leftMargin: 16
                width: 36
                height: 36

                UkuiItems.IconButton {
                    id: typeicon
                    visible: modelData.State === 2 || modelData.State === 4
                    iconSource: (modelData.State === 2) ? ("network-wired-connected-symbolic") : ("network-wired-disconnected-symbolic")
                    anchors.fill: parent
                    radius: 19
                    isHighLight: modelData.State === 2
                    width: parent.width
                    height: parent.height
                }

                UkuiItems.IconButton {
                    id: loadingicon
                    visible: modelData.State === 1 || modelData.State === 3
                    iconSource: "ukui-loading-" + String(loadingicon.loading_num % 8) + "-symbolic"
                    anchors.fill: parent
                    radius: 19
                    isHighLight: modelData.State === 3
                    width: parent.width
                    height: parent.height
                    property int  loading_num : 0
                }
                Timer {
                    interval: 100
                    running: true//modelData.State === 1 || modelData.State === 3
                    repeat: true
                    onTriggered:{
                        loadingicon.loading_num += 1;
                        loadingicon.iconSource =  "ukui-loading-" + String(loadingicon.loading_num % 8) + "-symbolic";
                    }
                }
                MouseArea {
                    onClicked: {
                        nameLabel.visible = false
                        nameStateLabel.visible = true
                    }
                }
            }
            Label {
                id: nameLabel
                Layout.alignment: Qt.AlignLeft
                Layout.leftMargin: 8
                Layout.preferredWidth: 150
                text: modelData.Name
                font.pixelSize: 14
                MouseArea {
                    onClicked: {
                        nameLabel.visible = false
                        nameStateLabel.visible = true
                    }
                }
            }

            Label {
                id: nameStateLabel
                visible: false
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                Layout.leftMargin: 8
                Layout.topMargin: 8
                Layout.preferredWidth: 150
                text: modelData.Name
                font.pixelSize: 14
                Label {
                    id: stateLabel
                    Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
                    Layout.leftMargin: 8
                    Layout.bottomMargin: 8
                    anchors.top: nameStateLabel.bottom
                    text: (modelData.State === 2) ? qsTr("已连接") : qsTr("未连接")
                    font.pixelSize: 12
                }
            }

            Item {
                Layout.fillWidth: true
            }

            Rectangle {
                id: speedLabel
                visible: false
                Layout.preferredWidth: 140
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter


                Image {
                    id: upLoadicon
                    visible: modelData.State === 2
                    anchors.right: upLoadWiredText.left
                    anchors.verticalCenter: parent.verticalCenter
                    source: "file:///usr/share/ukui/widgets/org.ukui.shortcut.network/load-up.png"
                }

                Label {
                    id: upLoadWiredText
                    visible: modelData.State === 2
                    anchors.right: downLoadIcon.left
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 12
                    text: "0KB/s"
                    Connections {
                        target: KInterface
                        onUpdateUpLoadWiredStr : {
                            upLoadWiredText.text = KInterface.upwareRate
                        }
                    }
                }

                Image {
                    id: downLoadIcon
                    visible: modelData.State === 2
                    anchors.right: downLoadWiredText.left
                    anchors.verticalCenter: parent.verticalCenter
                    source: "file:///usr/share/ukui/widgets/org.ukui.shortcut.network/load-down.png"
                }

                Label {
                    id: downLoadWiredText
                    visible: modelData.State === 2
                    anchors.rightMargin: 24
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 12
                    text: "0KB/s"
                    Connections {
                        target: KInterface
                        onUpdateDownLoadWiredStr : {
                            downLoadWiredText.text = KInterface.downwareRate
                        }
                    }
                }
            }

            Button {
                id: connectBtn
                visible: false
                width: 88
                height: 36
                Layout.rightMargin: 24
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                text: (modelData.State === 2) ? "断开" : "连接"
                highlighted: (modelData.State === 2) ? 0 : 1
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        // // 设置当前选中项
                        if (modelData.State === 2)
                            KInterface.deActivateConnect(lanDeviceComboBox.currentText, modelData.Uuid, 0);
                        else if (modelData.State === 4)
                            KInterface.activateConnect(lanDeviceComboBox.currentText, modelData.Uuid, 0);
                        typeicon.visible = false;
                        loadingicon.visible = true;
                    }
                }
            }
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            propagateComposedEvents: true
            onReleased: {
                nameLabel.visible = false
                nameStateLabel.visible = true
            }
            onEntered: {
                //console.log("index: " , index)
                enteritem = true
                connectBtn.visible = true
                speedLabel.visible = false
            }
            onExited: {
                //console.log("index: " , index)
                lanlistView.currentIndex = -1
                enteritem = false
                connectBtn.visible = false
                speedLabel.visible = (modelData.State === 2)
                nameLabel.visible = true
                nameStateLabel.visible = false
            }
        }

        Connections {
            target: KInterface
//                    onUpdatePairedDevice : {
//                        if(device === modelData.Addr) {
//                            if(attrs.hasOwnProperty("Connecting")) {
//                                typeicon.visible = !attrs.Connecting;
//                                loadingicon.visible = attrs.Connecting;
//                            }
//                            if(attrs.hasOwnProperty("Battery")) {
//                                batteryicon.source = KInterface.getBluetoothBatteryIcon(attrs.Battery);
//                                batterytext.text = attrs.Battery + "%"
//                            }
//                        }
//                    }
        }
    }
    Menu {
        id: propertyMenu

        // MenuItem {
        //     text: (modelData.State === 2) ? qsTr("Disconnect") : qsTr("Connect")
        //     onTriggered:
        //     {
        //         console.log("connect 操作")
        //         KInterface.openNetworkSetting()
        //     }
        // }

        MenuItem {
            text: qsTr("网络配置")
            onTriggered:
            {
                console.log("网络配置")
                KInterface.openNetworkSetting()
            }
        }

        // MenuSeparator {}

        // MenuItem {
        //     text: qsTr("Delete the network")
        //     onTriggered: console.log("Delete the network 操作")
        // }
    }
    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.AllButtons
        onReleased: {
            if (mouse.button === Qt.RightButton) {
                console.log("右键释放")
                propertyMenu.popup()
            }
        }
    }
}
