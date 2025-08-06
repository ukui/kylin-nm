import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls.Material 2.3
import org.ukui.shortcut.network 1.0
import org.ukui.quick.items 1.0 as UkuiItems
import org.ukui.quick.platform 1.0 as Platform

ListView {
    id: wlanlistView
    visible: true
    model: KInterface.wirelessConList
    spacing: 0

    // 定义每个项的显示方式
    delegate: ItemDelegate {
        id: listItem
        highlighted: enteritem && wlanlistView.currentIndex === index
        width: parent.width
        height: 56
        Layout.leftMargin: 8
        property bool enteritem : false


        ColumnLayout {
            anchors.fill: parent
            width: parent.width
            height:parent.height
            RowLayout {
                id: itemRowLayout

                Item {
                    Layout.alignment: Qt.AlignLeft
                    Layout.leftMargin: 16
                    width: 36
                    height: 36

                    UkuiItems.IconButton {
                        id: typeicon
                        visible: modelData.State === 2 || modelData.State === 4
                        iconSource: KInterface.getWiFiIcon(modelData.Signal, modelData.Security, modelData.isApConn, modelData.Type)
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
                        running: modelData.State === 1 || modelData.State === 3
                        repeat: true
                        onTriggered:{
                            loadingicon.loading_num += 1;
                            loadingicon.iconSource =  "ukui-loading-" + String(loadingicon.loading_num % 8) + "-symbolic";
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
                        text: (modelData.State === 2) ? "已连接" : "未连接"
                        font.pixelSize: 12
                    }
                }

                Item {
                    Layout.fillWidth: true
                }

                Rectangle {
                    id: speedLabel
                    visible: true
                    Layout.preferredWidth: 140
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter


                    Image {
                        id: upLoadicon
                        visible: modelData.State === 2
                        anchors.right: upLoadWirelessText.left
                        anchors.verticalCenter: parent.verticalCenter
                        source: "file:///usr/share/ukui/widgets/org.ukui.shortcut.network/load-up.png"
                    }

                    Label {
                        id: upLoadWirelessText
                        visible: modelData.State === 2
                        anchors.right: downLoadIcon.left
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 12
                        text: "0KB/s"
                        Connections {
                            target: KInterface
                            onUpdateUpLoadWirelessStr : {
                                upLoadWirelessText.text = str
                            }
                        }
                    }

                    Image {
                        id: downLoadIcon
                        visible: modelData.State === 2
                        anchors.right: downLoadWirelessText.left
                        anchors.verticalCenter: parent.verticalCenter
                        source: "file:///usr/share/ukui/widgets/org.ukui.shortcut.network/load-down.png"
                    }

                    Label {
                        id: downLoadWirelessText
                        visible: modelData.State === 2
                        anchors.rightMargin: 24
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 12
                        text: "0KB/s"
                        Connections {
                            target: KInterface
                            onUpdateDownLoadWirelessStr : {
                                downLoadWirelessText.text = str
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
                                KInterface.deActivateConnect(wlanDeviceComboBox.currentText, modelData.Name, 1);
                            else if (modelData.State === 4)
                                KInterface.activateConnect(wlanDeviceComboBox.currentText, modelData.Name, 1);
                            typeicon.visible = false;
                            loadingicon.visible = true;
                        }
                    }
                }

                MouseArea {
                    anchors.fill: itemRowLayout
                    hoverEnabled: true
                    propagateComposedEvents: true
                    onReleased: {
                        if (!textEditLayout.visible) {
                            nameLabel.visible = false
                            nameStateLabel.visible = true
                        }
                        if (modelData.State !== 2) {
                            if (listItem.height == 145)
                                listItem.height = 56
                            else
                                listItem.height = 145
                            textEditLayout.visible = !textEditLayout.visible
                            autoConnectCheckBox.visible = textEditLayout.visible
                            connectBtn.visible = !textEditLayout.visible
                        }
                    }
                    onEntered: {
                        if (textEditLayout.visible)
                            return
                        enteritem = true
                        connectBtn.visible = true
                        speedLabel.visible = false
                    }
                    onExited: {
                        wlanlistView.currentIndex = -1
                        enteritem = false
                        connectBtn.visible = false
                        speedLabel.visible = (modelData.State === 2)
                        nameLabel.visible = true
                        nameStateLabel.visible = false
                    }
                    // 点击Item时候焦点聚焦
                    onClicked: {
                        textEdit.forceActiveFocus()
                    }
                }
            }
            RowLayout {
                visible: false
                id: textEditLayout


                TextField {
                    id: textEdit
                    width: 208
                    height: 36
                    Layout.leftMargin: 68
                    echoMode: TextInput.Password
                    color:"black"
                    focus:true
                }
                Rectangle {
                    id: pwdConnectBtn
                    width: 88
                    height: 36
                    Layout.rightMargin: 24
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    radius: 6
                    color: "#3676F5"
                    Label {
                        Layout.alignment: Qt.AlignCenter
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: "white"
                        text: "连接"
                    }

                    MouseArea {
                        anchors.fill: pwdConnectBtn
                        onClicked: {
                            KInterface.passwordConnect(wlanDeviceComboBox.currentText, modelData.Name, modelData.Security, textEdit.text, autoConnectCheckBox.checkState)
                        }
                    }
                }
            }
            CheckBox {
                id: autoConnectCheckBox
                width: 16
                height: 16
                visible: false
                Layout.leftMargin: 68
                text: "自动连接"
                checked: true
            }
    }



//                RowLayout {
//                    id: autoConnectLayout
//                    anchors.fill: parent
//                    visible: false
//                }

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
                KInterface.openwLanNetworkSetting()
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
