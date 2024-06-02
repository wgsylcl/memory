import QtQuick 2.15
import QtQuick.Controls 2.15
import FluentUI 1.0
import taskloghelper 1.0

FluContentPage {
    id: root
    title: qsTr("任务面板")
    ScrollView {
        clip: true
        id: scrollview
        anchors.fill: parent
        Flickable {
            width: parent.width
            height: parent.height
            contentWidth: width
            contentHeight: textarea.height
            onContentHeightChanged: contentY = contentHeight - height
            TextArea {
                id: textarea
                width: parent.width
                readOnly: true
                font.pointSize: 12
                text: ""
            }
        }
    }
    TasklogHelper {
        id: taskloghelper
        Component.onCompleted: {
            textarea.text = taskloghelper.getlog()
        }
    }
    Connections {
        target: taskloghelper
        function onLogupdatesignal(newlog) {
            textarea.text += newlog
        }
    }
}
