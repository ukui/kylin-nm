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
            }
        }
    }
    Connections {
        target: KInterface
        function onTriggerButtonRequested(index) {
                console.log("Triggering button for index:", index)
                const delegate = wlanlistView.itemAtIndex(index)
                if (delegate) {
                    // 滚动到视图中间
                    Qt.callLater(function() {

                               // 计算在外部滚动视图中的位置
                               var itemPos = delegate.mapToItem(outerFlickable, 0, 0);

                               // 计算目标位置
                               var targetY = itemPos.y-(outerFlickable.height - delegate.height) / 2;

                               // 边界检查
                               targetY = Math.max(0, Math.min(targetY, outerFlickable.contentHeight - outerFlickable.height));

                               // 应用滚动位置
                               outerFlickable.contentY = targetY;
                        })
                    delegate.triggerButtonClick();
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
        property int freqForDisplay: (model.frequency === 0 && model.ssid && model.ssid.toUpperCase().indexOf("5G") !== -1) ? 5200 : model.frequency
        property int signalForDisplay: (isNaN(parseInt(model.signal)) ? -1 : (parseInt(model.signal) === 0 ? (model.status === 2 ? 100 : -1) : parseInt(model.signal)))
        property bool menuLoaded: false
        property bool textEditLoaded: false
        property bool pwdConnectBtnLoaded: false
        property bool autoConnectCheckBoxLoaded: false
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
            if (autoConnectCheckBoxLoader.item) {
                autoConnectCheckBoxLoader.item.visible = false
            }
        }

        function loadMenu() {
            if (!menuLoaded) {
                menuLoader.active = true
                menuLoaded = true
            }
        }

        function loadTextEdit() {
            if (!textEditLoaded) {
                textEditLoader.active = true
                textEditLoaded = true
            }
        }

        function loadPwdConnectBtn() {
            if (!pwdConnectBtnLoaded) {
                pwdConnectBtnLoader.active = true
                pwdConnectBtnLoaded = true
            }
        }

        function loadAutoConnectCheckBox() {
            if (!autoConnectCheckBoxLoaded) {
                autoConnectCheckBoxLoader.active = true
                autoConnectCheckBoxLoaded = true
            }
        }

        function updateItemPos() {
            Qt.callLater(function() {
                var itemPos = listItem.mapToItem(outerFlickable, 0, 0);
            
                // 计算目标位置
                var targetY = outerFlickable.height - itemPos.y ;

                console.log("updateItemPos itemPos.y:", itemPos.y, " outerFlickable.contentY:",  outerFlickable.contentY, 
                            " outerFlickable.height:", outerFlickable.height, " targetY:", targetY, " listItem.height:", listItem.height,
                            "listItem.contentHeight:", outerFlickable.contentHeight);

                if (targetY < listItem.height) {
                    outerFlickable.contentY += (listItem.height - targetY);
                    console.log("updateItemPos adjust outerFlickable.contentY to :", outerFlickable.contentY)
                }
            });
            
        }

        //改函数逻辑待梳理优化点击事件后可合并为公共方法 暂不处理
        function triggerButtonClick(){

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
                    // 动态加载textEdit和pwdConnectBtn
                    if (!textEditLoaded) {
                        loadTextEdit()
                        loadPwdConnectBtn()
                    }
                    textEditLayout.visible = (!model.Configured && !model.security.includes("802.1X") && model.security)
                    connectBtn.visible = !textEditLayout.visible
                }
            }

            if(!textEditLayout.visible) {
                if (model.status === 2) {
                    KInterface.deActivateConnect(wlanDeviceComboBox.currentText, model.ssid, 1);
                } else if (model.status === 4) {
                    KInterface.activateConnect(wlanDeviceComboBox.currentText, model.ssid, 1);
                    connectBtn.visible=false
                }
                return
            } else {
                if(textEditLoader.item) textEditLoader.item.forceActiveFocus()
            }

            if(autoConnectCheckBoxLoader.item && autoConnectCheckBoxLoader.item.visible) {
                autoConnectCheckBoxLoader.item.visible = false
            } else if (model.status !== 2){
                if (!autoConnectCheckBoxLoaded) {
                    loadAutoConnectCheckBox()
                }
                autoConnectCheckBoxLoader.item.visible = (textEditLayout.visible || model.Configured)
            }
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

                if (mouse.button == Qt.RightButton && model.status !== 2) {
                    if (!menuLoaded) {
                        loadMenu()
                    }
                    if (menuLoader.item) {
                        menuLoader.item.popup()
                        wlanlistView.currentOpenMenu = menuLoader.item;
                    }
                    mouse.accepted = true
                    return
                }

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
                        // 动态加载textEdit和pwdConnectBtn
                        if (!textEditLoaded) {
                            loadTextEdit()
                            loadPwdConnectBtn()
                        }
                        textEditLayout.visible = (!model.Configured && !model.security.includes("802.1X") && model.security)
                        connectBtn.visible = !textEditLayout.visible
                    }
                } else if ((model.status === 2) && (mouse.button == Qt.LeftButton) ) {
                    console.log("onClicked return")
                    KInterface.deActivateConnect(wlanDeviceComboBox.currentText, model.ssid, 1);
                }

                if(connectBtnHandler.containsMouse) {
                    if (model.status === 2) {
                        KInterface.deActivateConnect(wlanDeviceComboBox.currentText, model.ssid, 1);
                    } else if (model.status === 4) {
                        KInterface.activateConnect(wlanDeviceComboBox.currentText, model.ssid, 1);
                    }
                    return
                }

                if(autoConnectCheckBoxLoader.item && autoConnectCheckBoxLoader.item.visible) {
                    autoConnectCheckBoxLoader.item.visible = false
                } else if (model.status !== 2){
                    if (!autoConnectCheckBoxLoaded) {
                        loadAutoConnectCheckBox()
                    }
                    autoConnectCheckBoxLoader.item.visible = (textEditLayout.visible || model.Configured)
                }

                if (mouse.button == Qt.LeftButton) {
                    if(textEditLayout.visible && textEditLoader.item) textEditLoader.item.forceActiveFocus()
                } else if (mouse.button == Qt.RightButton) {
                    if (!menuLoaded) {
                        loadMenu()
                    }
                    if (menuLoader.item) {
                        menuLoader.item.popup()
                        wlanlistView.currentOpenMenu = menuLoader.item;
                    }
                }
            }
        }
        ColumnLayout {
            anchors.fill: parent
            width: parent.width
            height:parent.height
            RowLayout {
                id: itemRowLayout

                // 动态加载Menu
                Loader {
                    id: menuLoader
                    active: false
                    sourceComponent: Menu {
                        id: propertyMenu

                        MenuItem {//connect/disconnect
                            text:(model.status === 2)?qsTr("Disconnect network"):qsTr("Connect network")
                            onTriggered: {
                                console.log("connect/disconnect network")
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
                }


                Item {
                    Layout.alignment: Qt.AlignLeft
                    enabled: false
                    Layout.leftMargin: 26
                    width: 36
                    height: 36

                    UkuiItems.IconButton {
                        id: typeicon
                        visible: model.status === 2 || model.status === 4
                        // use signalForDisplay for icon selection; when unknown (-1) fall back to 0
                        iconSource: KInterface.getWiFiIcon(signalForDisplay === -1 ? 0 : signalForDisplay, model.security, model.isApConn, model.category)
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
                        Layout.leftMargin: 7
                        Label {
                            id: nameLabel
                            Layout.alignment: Qt.AlignLeft
                            Layout.bottomMargin: 0
                            Layout.preferredWidth: Math.min(implicitWidth, listItem.width - 250)  // 限制最大宽度，为其他元素预留空间
                            Layout.maximumWidth: listItem.width - 250  // 确保不会挤压其他元素
                            text: model.ssid
                            elide: Text.ElideRight
                        }

                        Rectangle {
                            id: roundedRect
                            Layout.alignment: Qt.AlignLeft
                            Layout.bottomMargin: 0
                            width: textMetrics.tightBoundingRect.width + 6
                            height: textMetrics.tightBoundingRect.height + 6

                            // 0 = 2.4G/5G, 1 = 5G, 2 = 2.4G
                            property int wlan_type : model.isMix ? 0 : freqForDisplay > 5000 ? 1 : 2;

                            color: "transparent"
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
                                property int wlan_type : model.isMix ? 0 : freqForDisplay > 5000 ? 1 : 2;
                                text: wlan_type === 0 ? "2.4G/5G" : wlan_type === 1 ? "5G" : "2.4G"
                                textColor: Platform.GlobalTheme.kFontPlaceholderText
                            }
                        }
                    }

                    RowLayout {
                        visible: false
                        id: textEditLayout
                        Layout.topMargin: 10
                        Layout.leftMargin: 6

                        // 动态加载TextField
                        Loader {
                            id: textEditLoader
                            active: false
                            sourceComponent: TextField {
                                Layout.leftMargin: 0
                                id: textEdit
                                width: 208
                                echoMode: TextInput.Password
                                property bool passMode: true
                                property int textLength: textEdit.text.length
                                validator: RegExpValidator { regExp: /^[a-zA-Z0-9\s\!\"\#\$\%\&\'\(\)\*\+\,\-\.\/\:\;\<\=\>\?\@\[\\\]\^_\`\{\|\}\~]*$/}

                                onTextLengthChanged: {
                                    if(textLength>=8) {
                                        if (pwdConnectBtnLoader.item) {
                                            pwdConnectBtnLoader.item.enabled = true
                                        }
                                    } else {
                                        if (pwdConnectBtnLoader.item) {
                                            pwdConnectBtnLoader.item.enabled = false
                                        }
                                    }
                                }
                                onAccepted: {

                                    KInterface.passwordConnect(wlanDeviceComboBox.currentText, model.ssid, model.security, textEdit.text,
                                                               autoConnectCheckBoxLoader.item ? autoConnectCheckBoxLoader.item.checkState : true)
                                }
                            }
                        }
                        // 动态加载Button
                        Loader {
                            id: pwdConnectBtnLoader
                            active: false
                            sourceComponent: Button {
                                id: pwdConnectBtn
                                highlighted: true
                                width: 88
                                height: 36
                                Layout.rightMargin: 24
                                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                                text: qsTr("connect")

                                contentItem: Label {
                                    text: parent.text
                                    font: parent.font
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                    elide: Text.ElideRight
                                    property var labelDTColor: pwdConnectBtn.enabled ?  "kfont-white" : "kfont-white-disable"
                                }

                                onClicked: {

                                    KInterface.passwordConnect(wlanDeviceComboBox.currentText, model.ssid, model.security,
                                                               textEditLoader.item ? textEditLoader.item.text : "",
                                                               autoConnectCheckBoxLoader.item ? autoConnectCheckBoxLoader.item.checkState : true)
                                }

                                Component.onCompleted: {
                                    pwdConnectBtn.enabled = (textEditLoader.item && textEditLoader.item.textLength>=8 ? true : false)
                                }
                            }
                        }
                    }
                    RowLayout {
                        UkuiItems.DtThemeText {
                            visible: model.status === 2
                            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                            Layout.topMargin: 0
                            Layout.leftMargin: 7
                            Layout.preferredWidth: Math.min(implicitWidth, listItem.width - 250)

                            text: qsTr("connected");

                            textColor: Platform.GlobalTheme.kFontPlaceholderText
                            height: 16
                            elide: Text.ElideRight
                        }
                        // 动态加载CheckBox
                        Loader {
                            id: autoConnectCheckBoxLoader
                            active: false
                            sourceComponent: CheckBox {
                                id: autoConnectCheckBox
                                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                                width: Math.min(implicitWidth, listItem.width - 250)
                                visible: false
                                Layout.topMargin: 0
                                text: qsTr("AutoConnect")
                                checked: true

                                // 使用TextMetrics来准确测量文本宽度
                                TextMetrics {
                                    id: checkboxTextMetrics
                                    font: autoConnectCheckBox.font
                                    text: autoConnectCheckBox.text
                                }

                                // 自定义contentItem以支持省略号
                                contentItem: Label {
                                    text: autoConnectCheckBox.text
                                    font: autoConnectCheckBox.font
                                    verticalAlignment: Text.AlignVCenter
                                    leftPadding: autoConnectCheckBox.indicator.width + autoConnectCheckBox.spacing
                                    elide: Text.ElideRight  // 文本省略号
                                }

                                onClicked: {
                                    mouse.accepted = true
                                }

                                onVisibleChanged: {
                                    if (visible) {
                                        updateShowDetailIndex(index)
                                        updateItemPos()
                                    }
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
                    Layout.preferredWidth: 88  /* 在布局中的首选宽度 */
                    Layout.maximumWidth: 88
                    height: 36
                    Layout.rightMargin: 24
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    text: (model.status === 2) ? qsTr("disconnect") : qsTr("connect")
                    highlighted: (model.status === 2) ? 0 : 1

                    /* 在连接按钮文本缩略情况下，添加tooltip显示完整的文本 */
                    ToolTip.visible: connectBtnHandler.containsMouse && contentItem.truncated
                    ToolTip.text: text
                    ToolTip.delay: 500

                    contentItem: Label {
                        text: parent.text
                        font: parent.font
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                        property var labelDTColor: connectBtn.enabled ?  "kfont-white" : "kfont-white-disable"
                    }

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
        flat: true

        UkuiItems.DtThemeText {
            text: qsTr("Add Others...")
            anchors.top: parent.top
            anchors.topMargin: (parent.height-height)/2   //垂直居中设置不生效使用边距控制居中
            anchors.left: parent.left
            anchors.leftMargin: 26
            elide: Text.ElideRight
        }

        onClicked: {
            console.log("addOtherBtn onClicked ",parent.verticalCenter,anchors.verticalCenter)
            KInterface.showAddOtherWlanPage(wlanDeviceComboBox.currentText);
            mouse.accepted = true
        }

    }


}



