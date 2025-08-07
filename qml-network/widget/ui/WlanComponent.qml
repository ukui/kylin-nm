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
    spacing: 0
    property bool connectMac : false
    property int detailShowIndex : -1
    property var currentOpenMenu: null

    function updateMacConnAttr(ipos, status) {
        if (0 === ipos) {
            connectMac = status
        }
    }

    function updateShowDetailIndex(ipos) {
        console.log("detail index: ", ipos)
        detailShowIndex = ipos
        for(var i=0; i<wlanlistView.count; i++) {
            if(i !== detailShowIndex) {
                var d = wlanlistView.itemAtIndex(i);
                if (d) {
                    d.hideDetail()
                }

            }
        }
    }

    // 定义每个项的显示方式
    delegate: ItemDelegate {
        id: listItem
        highlighted: enteritem && wlanlistView.currentIndex === index
        width: parent.width
        height: 56
        property bool enteritem : false
        property bool conConnected:   model.status === 2
        onConConnectedChanged: {
            updateMacConnAttr(index, conConnected)
        }

        Component.onCompleted: {
            updateMacConnAttr(index, conConnected)
        }

        function hideDetail() {
            if (textEditLayout.visible) {
                listItem.height = 56
                textEditLayout.visible = false
            }
            autoConnectCheckBox.visible = false
        }


        //嵌套MouseArea最好统一顶层调度
        MouseArea {
            anchors.fill: listItem
            acceptedButtons: Qt.AllButtons
            hoverEnabled: true
            propagateComposedEvents: true

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

            }

            onWheel: {
                wheel.accepted = false;
                if (wlanlistView.currentOpenMenu && wlanlistView.currentOpenMenu.visible) {
                    wlanlistView.currentOpenMenu.close();
                }
                wlanlistView.currentOpenMenu = null;
            }

            // 点击Item时候焦点聚焦
            onClicked: {
                mouse.accepted = false

                if (model.status !== 2 ) {
                    if (listItem.height == 145 ||  model.Configured || model.security.includes("802.1X")) {
                        listItem.height = 56
                    } else {
                        listItem.height = 145
                    }

                    if(textEditLayout.visible) {
                        listItem.height = 56
                        textEditLayout.visible = false
                        connectBtn.visible = true
                    } else {
                        textEditLayout.visible = (!model.Configured && !model.security.includes("802.1X"))
                        connectBtn.visible = !textEditLayout.visible
                    }
                }

                if(connectBtnHandler.containsMouse) {
                    console.log("in connectBtn return")
                    typeicon.visible = false;
                    loadingicon.visible = true;
                    if (model.status === 2) {
                        KInterface.deActivateConnect(wlanDeviceComboBox.currentText, model.ssid, 1);
                    } else if (model.status === 4) {
                        KInterface.activateConnect(wlanDeviceComboBox.currentText, model.ssid, 1);
                    }

                    return
                }

                if(autoConnectCheckBox.visible) {
                    autoConnectCheckBox.visible = false
                } else {
                    autoConnectCheckBox.visible=(textEditLayout.visible || model.Configured)
                }

                if (mouse.button == Qt.LeftButton) {
                    if(textEditLayout.visible)  textEdit.forceActiveFocus()
                } else if (mouse.button == Qt.RightButton) {
                    propertyMenu.popup()
                    wlanlistView.currentOpenMenu = propertyMenu;
                }
            }
        }
        ColumnLayout {
            anchors.fill: parent
            width: parent.width
            height:parent.height
            RowLayout {
                id: itemRowLayout

                Menu {
                    id: propertyMenu

                    MenuItem {//connect/disconnect
                        text:(model.status === 2)?qsTr("Disconnect network"):qsTr("Connect network")
                        onTriggered: {
                            console.log("connect/disconnect network")
                            typeicon.visible = false;
                            loadingicon.visible = true;
                            if (model.status === 2) {
                                KInterface.deActivateConnect(wlanDeviceComboBox.currentText, model.ssid, 1);
                            } else if (model.status === 4) {
                                KInterface.activateConnect(wlanDeviceComboBox.currentText, model.ssid, 1);
                            }
                        }
                    }

                    MenuItem {//property
                        text:qsTr("Network property")
                        visible: model.status === 2
                        onTriggered: {
                            console.log("network property")
                            console.log("network property",wlanDeviceComboBox.currentText,model.Name,model.ssid)

                            KInterface.showPropertyWidget(wlanDeviceComboBox.currentText, model.ssid)
                        }
                    }

                    MenuItem {
                        text:qsTr("Forget the network")
                        visible: model.status === 2
                        onTriggered: {
                            console.log("Forget the network",model.uuid)
                            KInterface.deleteConnect(1,model.uuid)
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

                ColumnLayout {
                    spacing: 0
                    Label {
                        id: nameLabel
                        Layout.alignment: Qt.AlignLeft
                        Layout.leftMargin: 8
                        Layout.preferredWidth: 150
                        Layout.bottomMargin: 0
                        text: model.ssid
                        font.pixelSize: 14
                    }

                    RowLayout {
                        visible: false
                        id: textEditLayout
                        Layout.topMargin: 10

                        TextField {
                            id: textEdit
                            width: 208
                            height: 36
                            Layout.leftMargin: 10
                            echoMode: TextInput.Password
                            property bool passMode: true
                            property int textLength: textEdit.text.length
                            onTextLengthChanged: {
                                if(textLength>=8) {
                                    pwdConnectBtn.enabled = true
                                } else {
                                    pwdConnectBtn.enabled = false
                                }
                            }
                            onAccepted: {
                                typeicon.visible = false;
                                loadingicon.visible = true;

                                KInterface.passwordConnect(wlanDeviceComboBox.currentText, model.ssid, model.security, textEdit.text, autoConnectCheckBox.checkState)
                            }
                        }
                        Button {
                            id: pwdConnectBtn
                            highlighted: true
                            //enabled: true
                            width: 88
                            height: 36
                            Layout.rightMargin: 24
                            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                            text: qsTr("connect")
                            onClicked: {
                                typeicon.visible = false;
                                loadingicon.visible = true;

                                KInterface.passwordConnect(wlanDeviceComboBox.currentText, model.ssid, model.security, textEdit.text, autoConnectCheckBox.checkState)
                            }

                            Component.onCompleted: {
                                pwdConnectBtn.enabled = (textEdit.textLength>=8 ? true : false)
                            }
                        }
                    }
                    CheckBox {
                        id: autoConnectCheckBox
                        Layout.alignment: Qt.AlignLeft
                        width: 16
                        height: 16
                        visible: false
                        Layout.leftMargin: 8
                        Layout.topMargin: 0
                        text: qsTr("AutoConnect")
                        checked: true
                        onClicked: {
                            mouse.accepted = true
                        }

                        onVisibleChanged: {
                            if (visible) {
                                updateShowDetailIndex(index)
                            }
                        }
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
                        anchors.rightMargin: 32
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
                        id: connectBtnHandler
                        propagateComposedEvents: true
                        hoverEnabled: true
                        property bool showButton: containsMouse
                        anchors.fill: parent
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
                        }

                        onClicked: {
                            mouse.accepted = false
                        }

                    }
                }
            }
        }
    }
    footer: Button {
                id: addOtherBtn
                visible: true
                width: parent.width
                height: 40
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                hoverEnabled: true

                Text {
                        text: qsTr("Add Others...")
                        anchors.top: parent.top
                        anchors.topMargin: (parent.height-height)/2   //垂直居中设置不生效使用边距控制居中
                        anchors.left: parent.left
                        anchors.leftMargin: 26
                    }

                onClicked: {
                     console.log("addOtherBtn onClicked ",parent.verticalCenter,anchors.verticalCenter)
                     KInterface.showAddOtherWlanPage(wlanDeviceComboBox.currentText);
                      mouse.accepted = true
                    }

           }


    }



