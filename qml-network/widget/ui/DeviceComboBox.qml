import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls.Material 2.3
import org.ukui.quick.items 1.0 as UkuiItems
import org.ukui.quick.platform 1.0 as Platform
import org.ukui.shortcut.network 1.0

ComboBox {
    id: comboBoxOrigin
    Layout.alignment: Qt.AlignVCenter
    visible: true
    Layout.maximumHeight: 50
    Layout.minimumHeight: 36
    Layout.fillWidth: true
    Layout.leftMargin: 24
    Layout.rightMargin: 24
    property bool currentConnect: false

    property var delegates: []

    onCurrentConnectChanged: {
        for(var i=0; i<model.length; i++) {
            if(delegates[i])
                delegates[i].update()
        }
    }

    delegate: ItemDelegate { // 使用ItemDelegate来自定义每个选项的显示方式
        id: delegateItem
        width: parent.width
        property bool connect: KInterface.getNetMacConnectStatus(modelData)
        function update() {
            connect = KInterface.getNetMacConnectStatus(modelData)
        }
        Component.onCompleted: {
            console.log("ukui shortcut bluetooth start")
            delegates[index] = delegateItem
        }

        RowLayout {
            anchors.fill: parent 
            UkuiItems.DtThemeText {
                Layout.leftMargin: 16
                Layout.alignment: Qt.AlignLeft
                Layout.maximumWidth: parent.width * 0.6
                text: modelData
                elide: Text.ElideMiddle
            }
            UkuiItems.DtThemeText {
                Layout.alignment: Qt.AlignLeft
                Layout.leftMargin: 1
                Layout.rightMargin: 15
                text:  connect ? qsTr("(Connected)") : qsTr("(Not Connected)")
                textColor: connect ? Platform.GlobalTheme.kGreen1 : Platform.GlobalTheme.kFontSecondary
                elide: Text.ElideRight
            }
            Item {
                Layout.fillWidth: true
            }     
        }
    }

    contentItem: ItemDelegate {
        highlighted: false
        hoverEnabled: false
        RowLayout {
            anchors.fill: parent
            UkuiItems.DtThemeText {
                Layout.alignment: Qt.AlignLeft
                Layout.leftMargin: 10
                text: qsTr("Current Network Card")
                textColor: Platform.GlobalTheme.kFontSecondary
                elide: Text.ElideRight
            }

            UkuiItems.DtThemeText {
                Layout.alignment: Qt.AlignLeft
                Layout.maximumWidth: parent.width * 0.4
                text: comboBoxOrigin.currentText
                elide: Text.ElideMiddle
            }

            UkuiItems.DtThemeText {
                Layout.alignment: Qt.AlignLeft
                Layout.rightMargin: 15
                text: currentConnect ? qsTr("(Connected)") : qsTr("(Not Connected)")
                textColor: currentConnect ? Platform.GlobalTheme.kGreen1 : Platform.GlobalTheme.kFontSecondary
                elide: Text.ElideRight
            }
            
            Item {
                Layout.fillWidth: true
            }
        }
        background: Rectangle{
            color: "transparent"
        }

        onClicked: {
            comboBoxOrigin.popup.visible ? comboBoxOrigin.popup.close() : comboBoxOrigin.popup.open();
        }
    }
}
