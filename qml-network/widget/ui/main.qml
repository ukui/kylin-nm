import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls.Material 2.3
import QtGraphicalEffects 1.0
import org.ukui.quick.widgets 1.0

WidgetItem {
    Layout.fillWidth: true
    Layout.fillHeight: true
    clip: true

    NetScrollViewArea {
        anchors.fill: parent
    }
}
