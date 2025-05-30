import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls.Material 2.3

ComboBox {
    id: comboBoxOrigin
    Layout.alignment: Qt.AlignVCenter
    visible: true
    Layout.maximumHeight: 36
    Layout.fillWidth: true
    Layout.leftMargin: 24
    Layout.rightMargin: 24
    delegate: ItemDelegate { // 使用ItemDelegate来自定义每个选项的显示方式
        contentItem: Text { text: modelData} // modelData是当前项的数据
    }
}
