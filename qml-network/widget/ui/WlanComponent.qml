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
    property int baseItemHeight: 56
    property int expandedItemHeight: 145
    Component.onCompleted: {
        // map font size to heights (simple scaling)
        function updateHeights(fontSize) {
            var size = parseInt(fontSize)
            if (!isNaN(size)) {
                baseItemHeight = Math.round(56 * size / 10)
                expandedItemHeight = Math.round(145 * size / 10)
            }
        }
        updateHeights(KInterface.fontSize)
    }
    Connections {
        target: KInterface
        function onFontSizeChanged(fontSize) {
            var size = parseInt(fontSize)
            if (!isNaN(size)) {
                wlanlistView.baseItemHeight = Math.round(56 * size / 10)
                wlanlistView.expandedItemHeight = Math.round(145 * size / 10)
            }
        }
    }
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
        height: wlanlistView.baseItemHeight
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
                listItem.height = wlanlistView.baseItemHeight
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
                    console.log("model.security:", model.security, model.security.length)
                    if (listItem.height == wlanlistView.expandedItemHeight ||  model.Configured || model.security.includes("802.1X") || !model.security) {
                        listItem.height = wlanlistView.baseItemHeight
                    } else {
                        listItem.height = wlanlistView.expandedItemHeight
                    }

                    if(textEditLayout.visible) {
                        listItem.height = wlanlistView.baseItemHeight
                        textEditLayout.visible = false
                        connectBtn.visible = true
                    } else {
                        textEditLayout.visible = (!model.Configured && !model.security.includes("802.1X") && model.security)
                        connectBtn.visible = !textEditLayout.visible
                    }
                } else if ((model.status === 2) && (mouse.button == Qt.LeftButton) ) {
                    console.log("onClicked return")
                    KInterface.deActivateConnect(wlanDeviceComboBox.currentText, model.ssid, 1);
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
                } else if (model.status !== 2){
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

                    RowLayout {
                        Label {
                            id: nameLabel
                            Layout.alignment: Qt.AlignLeft
                            Layout.leftMargin: 8
                            Layout.bottomMargin: 0
                            text: model.ssid
                            elide: Text.ElideRight
                        }

                        Rectangle {
                            id: roundedRect
                            Layout.bottomMargin: 0

                            // 0 = 2.4G/5G, 1 = 5G, 2 = 2.4G
                            property int wlan_type : model.isMix ? 0 : model.frequency > 5000 ? 1 : 2;

                            color: "transparent"
                            width: textMetrics.tightBoundingRect.width + 6
                            height: textMetrics.tightBoundingRect.height + 6
                            radius: 4

                            border {
                                color: Platform.GlobalTheme.kFontPlaceholderText.pureColor
                                width: 1
                            }

                            TextMetrics {
                                id: textMetrics
                                font: dtThemeText.font
                                text: dtThemeText.text
                            }

                            UkuiItems.DtThemeText {
                                id: dtThemeText
                                anchors.centerIn: parent
                                property int wlan_type : model.isMix ? 0 : model.frequency > 5000 ? 1 : 2;
                                text: wlan_type === 0 ? "2.4G/5G" : wlan_type === 1 ? "5G" : "2.4G"
                                textColor: Platform.GlobalTheme.kFontPlaceholderText
                            }
                        }
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
                    RowLayout {
                        UkuiItems.DtThemeText {
                            visible: model.status === 2
                            anchors.centerIn: parent
                            Layout.topMargin: 0
                            Layout.leftMargin: 12

                            text: {
                                const signal = parseInt(model.signal);
                                return signal > 80 ? qsTr("Connected,network is very good") :
                                    signal > 55 ? qsTr("Connected,network is good") :
                                    signal > 30 ? qsTr("Connected,network is average") :
                                    signal > 5  ? qsTr("Connected,network weak") :
                                                    qsTr("Connected,network is weak");
                            }

                            textColor: Platform.GlobalTheme.kFontPlaceholderText
                            height: 16
                            elide: Text.ElideRight
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

                    UkuiItems.DtThemeText {
                        id: upLoadWirelessText
                        visible: model.status === 2
                        anchors.right: downLoadIcon.left
                        anchors.verticalCenter: parent.verticalCenter
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

                    UkuiItems.DtThemeText {
                        id: downLoadWirelessText
                        visible: model.status === 2
                        anchors.rightMargin: 32
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
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
        visible: wlanDeviceComboBox.count >= 1
        width: parent.width
        height: 40
        Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
        hoverEnabled: true

        UkuiItems.DtThemeText {
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



