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
    model: KInterface.wirelessConLists
    //model: KInterface.wirelessConList
    spacing: 0
    property bool connectMac : false

    function updateMacConnAttr(ipos, status) {
        if(0 === ipos)
            connectMac = status
    }

    // 定义每个项的显示方式
    delegate: ItemDelegate {
        id: listItem
        highlighted: enteritem && wlanlistView.currentIndex === index
        width: parent.width
        height: 56
        Layout.leftMargin: 8
        property bool enteritem : false
        property bool conConnected:   model.status === 2
        onConConnectedChanged: {
            updateMacConnAttr(index, conConnected)
        }

        Component.onCompleted: {
            updateMacConnAttr(index, conConnected)
        }

        // Binding{
        //  when:!textEdit.activeFocus
        // }

        ColumnLayout {
            anchors.fill: parent
            width: parent.width
            height:parent.height
            RowLayout {
                id: itemRowLayout

                Menu {
                    id: propertyMenu
                    MenuItem {
                        text: qsTr("Network settings")
                        onTriggered:
                        {
                            console.log("Network settings")
                            KInterface.openwLanNetworkSetting()
                        }
                    }
                }

                Item {
                    Layout.alignment: Qt.AlignLeft
                    Layout.leftMargin: 26
                    width: 36
                    height: 36

                    UkuiItems.IconButton {
                        id: typeicon
                        visible: model.status === 2 || model.status === 4
                        iconSource: KInterface.getWiFiIcon(model.signal, model.security, model.isApConn, model.category)
                        anchors.fill: parent
                        radius: 19
                        isHighLight: model.status === 2
                        width: parent.width
                        height: parent.height
                    }

                    UkuiItems.IconButton {
                        id: loadingicon
                        visible: model.status === 1 || model.status === 3
                        iconSource: "ukui-loading-" + String(loadingicon.loading_num % 8) + "-symbolic"
                        anchors.fill: parent
                        radius: 19
                        isHighLight: model.status === 3
                        width: parent.width
                        height: parent.height
                        property int  loading_num : 0
                    }
                    Timer {
                        interval: 100
                        running: model.status === 1 || model.status === 3
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
                    text: model.ssid
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
                    text: model.ssid
                    font.pixelSize: 14
                    Label {
                        id: stateLabel
                        Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
                        Layout.leftMargin: 8
                        Layout.bottomMargin: 8
                        anchors.top: nameStateLabel.bottom
                        text: (model.status === 2) ? qsTr("connected") : qsTr("Not connected")
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
                        visible: model.status === 2
                        anchors.right: upLoadWirelessText.left
                        anchors.verticalCenter: parent.verticalCenter
                        source: "file:///usr/share/ukui/widgets/org.ukui.shortcut.network/load-up.png"
                    }

                    Label {
                        id: upLoadWirelessText
                        visible: model.status === 2
                        anchors.right: downLoadIcon.left
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 12
                        text: "0KB/s"
                        Connections {
                            target: KInterface
                           function onUpdateUpLoadWirelessStr(str) {
                                upLoadWirelessText.text = str
                            }
                        }
                    }

                    Image {
                        id: downLoadIcon
                        visible: model.status === 2
                        anchors.right: downLoadWirelessText.left
                        anchors.verticalCenter: parent.verticalCenter
                        source: "file:///usr/share/ukui/widgets/org.ukui.shortcut.network/load-down.png"
                    }

                    Label {
                        id: downLoadWirelessText
                        visible: model.status === 2
                        anchors.rightMargin: 24
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 12
                        text: "0KB/s"
                        Connections {
                            target: KInterface
                            function onUpdateDownLoadWirelessStr(str)  {
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
                    text: (model.status === 2) ? qsTr("disconnect") : qsTr("connect")
                    highlighted: (model.status === 2) ? 0 : 1
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            // // 设置当前选中项
                            if (model.status === 2)
                                KInterface.deActivateConnect(wlanDeviceComboBox.currentText, model.ssid, 1);
                            else if (model.status === 4)
                                KInterface.activateConnect(wlanDeviceComboBox.currentText, model.ssid, 1);
                            typeicon.visible = false;
                            loadingicon.visible = true;
                        }
                    }
                }

                MouseArea {
                    anchors.fill: itemRowLayout //anchors与Layout不能联用，qml会报错但是实际好像对界面没影响，待解
                    acceptedButtons: Qt.AllButtons
                    hoverEnabled: true
                    propagateComposedEvents: true
                    onReleased: {

                        if (!textEditLayout.visible) {
                            nameLabel.visible = false
                            nameStateLabel.visible = true
                        }
                        if (model.status !== 2 ) {
                            if (listItem.height == 145 ||  model.Configured || model.security.includes("802.1X"))
                                listItem.height = 56
                            else
                                listItem.height = 145
                            //textEditLayout.visible = !textEditLayout.visible
                            textEditLayout.visible = (!model.Configured && !model.security.includes("802.1X"))
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
                        speedLabel.visible = (model.status === 2)
                        nameLabel.visible = true
                        nameStateLabel.visible = false
                    }
                    // 点击Item时候焦点聚焦
                    onClicked: {
                        mouse.accepted = false
                        if (mouse.button == Qt.LeftButton) {
                            textEdit.forceActiveFocus()
                        } else if (mouse.button == Qt.RightButton) {
                            propertyMenu.popup()
                        }
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
                    property bool passMode: true
                    color:"black"
                    focus:false //必须是false，界面重建时会导致崩溃，焦点丢失。编辑时切换窗口也会导致焦点丢失崩溃老问题待解，可能是qt bug
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
                        text: qsTr("connect")
                    }

                    MouseArea {
                        anchors.fill: pwdConnectBtn
                        onClicked: {
                            KInterface.passwordConnect(wlanDeviceComboBox.currentText, model.ssid, model.security, textEdit.text, autoConnectCheckBox.checkState)
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
                text: qsTr("AutoConnect")
                checked: true
            }
        }
    }
}
