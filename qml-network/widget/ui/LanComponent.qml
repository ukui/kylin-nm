qweimport QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.1
import QtQuick.Controls.Material 2.3
import org.ukui.shortcut.network 1.0
import org.ukui.quick.items 1.0 as UkuiItems
import org.ukui.quick.platform 1.0 as Platform
import QtQuick.Controls 2.5

ListView {
    id: lanlistView
    visible: lanswitchBtn.checked && lanVisibleButton.visibleState  && !lanNoWiredItem.visible
    model: KInterface.wiredConList
    spacing: 0
    property bool connectMac : false

    function updateMacConnAttr(ipos, status) {
        if(0 === ipos)
            connectMac = status
    }

    // 定义每个项的显示方式
    delegate: ItemDelegate {
        highlighted: enteritem && lanlistView.currentIndex === index
        width: lanlistView.width
        height: 56
        property bool enteritem : false
        property bool conConnected:   modelData.State === 2
        onConConnectedChanged: {
            updateMacConnAttr(index, conConnected)
        }

        Component.onCompleted: {
            updateMacConnAttr(index, conConnected)
        }

        //最外层 MouseArea 最好是放到最顶部
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            acceptedButtons: Qt.AllButtons
            propagateComposedEvents: true  // 关键：允许事件传播

            onReleased: {
                mouse.accepted = true  // 右键菜单自己处理
                nameLabel.visible = false
                nameStateLabel.visible = true
            }

            onEntered: {
                enteritem = true
                connectBtn.visible = true
                speedLabel.visible = false
            }
            onExited: {
                lanlistView.currentIndex = -1
                enteritem = false
                connectBtn.visible = false
                speedLabel.visible = (modelData.State === 2)
                nameLabel.visible = true
                nameStateLabel.visible = false
            }

            onClicked: {
                // 只处理右键，左键允许传播
                if (mouse.button == Qt.RightButton) {
                    propertyMenu.popup()
                    mouse.accepted = true  // 右键菜单自己处理
                } else {
                    mouse.accepted = false  // 左键允许继续传播
                }

                if(connectBtnHandler.containsMouse) {
                    console.log("in connectBtn return")
                    typeicon.visible = false;
                    loadingicon.visible = true;
                    if (modelData.State === 2)
                        KInterface.deActivateConnect(lanDeviceComboBox.currentText, modelData.Uuid, 0);
                    else if (modelData.State === 4 || modelData.State === 0) //状态为0或者4时发起连接
                        KInterface.activateConnect(lanDeviceComboBox.currentText, modelData.Uuid, 0);

                    return
                }
            }
        }

        RowLayout {
            anchors.fill: parent

            Menu {
                id: propertyMenu

                MenuItem {
                    text:(modelData.State === 2)?qsTr("Disconnect network"):qsTr("Connect network")
                    onTriggered: {
                        console.log("connect/disconnect network")

                        if (modelData.State === 2)
                            KInterface.deActivateConnect(lanDeviceComboBox.currentText, modelData.Uuid, 0);
                        else if (modelData.State === 4)
                            KInterface.activateConnect(lanDeviceComboBox.currentText, modelData.Uuid, 0);
                        typeicon.visible = false;
                        loadingicon.visible = true;
                    }
                }

                MenuItem {
                    text:qsTr("Network property")
                    onTriggered: {
                        console.log("network property")
                        console.log("network property",lanDeviceComboBox.currentText,modelData.Name,modelData.Uuid)

                        KInterface.showPropertyWidget(lanDeviceComboBox.currentText, modelData.Uuid)
                    }
                }

                MenuItem {
                    text:qsTr("Delete this network")
                    onTriggered: {
                        console.log("delete this network",modelData.Uuid)
                        KInterface.deleteConnect(0,modelData.Uuid)
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
                    //这里添加拨号的类型
                    visible: modelData.State === 0 || modelData.State === 2 || modelData.State === 4
                    iconSource: {
                        if (modelData.IsDSL) { // 假设modelData中有Type属性表示连接类型
                            ("ukui-dial-up-symbolic")
                        } else {
                            (modelData.State === 2) ? ("network-wired-connected-symbolic") : ("network-wired-disconnected-symbolic")
                        }
                    }
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
            }

         Label {
                id: nameLabel
                Layout.alignment: Qt.AlignLeft
                Layout.leftMargin: 8
                Layout.maximumWidth: 150
                text: modelData.Name
                elide: Text.ElideRight

                // 使用TextMetrics检测文本截断
                TextMetrics {
                    id: textMetrics
                    font: nameLabel.font
                    text: nameLabel.text
                    elide: nameLabel.elide
                    elideWidth: nameLabel.width
                }

                // 根据TextMetrics结果显示ToolTip
                ToolTip.visible: nameLabelHandler.containsMouse && (textMetrics.elidedText !== nameLabel.text)
                ToolTip.text: nameLabel.text
                ToolTip.delay: 500

                MouseArea {
                    id: nameLabelHandler
                    anchors.fill: parent
                    hoverEnabled: true

                    onReleased: {
                        nameLabel.visible = false
                        nameStateLabel.visible = true
                    }
                }
            }

            UkuiItems.DtThemeText {
                id: nameStateLabel
                visible: false
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                Layout.leftMargin: 8
                Layout.topMargin: 8
                Layout.preferredWidth: 150
                text: modelData.Name
                elide: Text.ElideRight
                UkuiItems.DtThemeText {
                    id: stateLabel
                    Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
                    Layout.leftMargin: 8
                    Layout.bottomMargin: 8
                    anchors.top: nameStateLabel.bottom
                    textColor: Platform.GlobalTheme.kFontPlaceholderText
                    text: modelData.State === 2 
                            ? (modelData.Connectivity === 3 
                                ? qsTr("connected") + "，" + qsTr("(network restricted)") 
                                : qsTr("connected"))
                            : qsTr("Not connected")
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
                    anchors.right: upLoadWiredText.left
                    anchors.verticalCenter: parent.verticalCenter
                    source: "file:///usr/share/ukui/widgets/org.ukui.shortcut.network/load-up.png"
                }

                UkuiItems.DtThemeText {
                    id: upLoadWiredText
                    visible: modelData.State === 2
                    anchors.right: downLoadIcon.left
                    anchors.verticalCenter: parent.verticalCenter
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

                UkuiItems.DtThemeText {
                    id: downLoadWiredText
                    visible: modelData.State === 2
                    anchors.rightMargin: 32
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
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
                Layout.preferredWidth: 88  /* 在布局中的首选宽度 */
                Layout.maximumWidth: 88
                height: 36
                Layout.rightMargin: 24
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                text: (modelData.State === 2) ? qsTr("disconnect") : qsTr("connect")
                highlighted: (modelData.State === 2) ? 0 : 1

                /* 在连接按钮文本缩略情况下，添加tooltip显示完整的文本 */
                ToolTip.visible: connectBtnHandler.containsMouse && contentItem.truncated
                ToolTip.text: text
                ToolTip.delay: 500

                contentItem: Text {
                    text: parent.text
                    font: parent.font
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight  /* 文本过长时显示省略号 */
                }

                MouseArea {
                    id: connectBtnHandler
                    propagateComposedEvents: true
                    hoverEnabled: true
                    property bool showButton: containsMouse
                    anchors.fill: parent
                    onEntered: {
                        enteritem = true
                        connectBtn.visible = true
                        speedLabel.visible = false
                    }

                    onExited: {
                        lanlistView.currentIndex = -1
                        enteritem = false
                        connectBtn.visible = false
                        speedLabel.visible = (model.status === 2)
                    }

                    onClicked: {
                        mouse.accepted = false

                        // // 设置当前选中项
                        if (modelData.State === 2)
                            KInterface.deActivateConnect(lanDeviceComboBox.currentText, modelData.Uuid, 0);
                        else if (modelData.State === 4 || modelData.State === 0) //状态为0或者4时发起连接
                            KInterface.activateConnect(lanDeviceComboBox.currentText, modelData.Uuid, 0);


                    }
                }
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

}
