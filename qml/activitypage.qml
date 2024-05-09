import QtQuick 2.15
import QtQuick.Controls 2.15
import FluentUI 1.0

FluContentPage {
    title: qsTr("回忆")
    property var activitynames: []
    // property var history: []
    property int index: 0
    property int mediasize: 0

    id: root

    ListModel {
        id: activitiymodel
    }

    Row {
        z: 1
        spacing: 20

        id: chooseactivityrow

        FluText {
            text: qsTr("选择要查看的活动")
        }

        FluComboBox {
            id: activitybox
            model: activitiymodel
            width: 400
            onCurrentIndexChanged: {
                mediasize = ActivityReader.readAllMedia(activitynames[currentIndex])
                for(var i=0;i<mediasize;i++) {
                    console.log(ActivityReader.getMediaPath(i))
                }

                stack.clear()

                index = 1
                var url = ActivityReader.getMediaPath((index-1)%mediasize)
                if(MainTool.isVedio(url)) {
                    stack.push(videoview)
                }
                else {
                    stack.push(imageview)
                }
            }
        }

        FluButton {
            text: qsTr("上一张")
            enabled: index > 1
            onClicked: popmedia()
        }

        FluFilledButton {
            text: qsTr("下一张")
            enabled: index
            onClicked: pushmedia()
        }
    }

    StackView {
        id: stack
        z: 0
        anchors {
            left: parent.left
            right: parent.right
            top: chooseactivityrow.bottom
            bottom: parent.bottom
        }


        pushEnter: Transition {
            // 定义淡入效果
            NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; duration: 100 }
        }

        pushExit: Transition {
            // 定义淡出效果
            // NumberAnimation { property: "opacity"; from: 1.0; to: 0.0; duration: 500 }
        }

        popEnter: Transition {
            // 定义淡入效果
            NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; duration: 100 }
        }

        popExit: Transition {
            // 定义淡出效果
            // NumberAnimation { property: "opacity"; from: 1.0; to: 0.0; duration: 500 }
        }
    }
    Component {
        id: imageview
        MouseArea {
            anchors.fill: parent
            propagateComposedEvents: true
            onWheel: function(wheel){
                if(stack.busy) return;
                if(wheel.angleDelta.y < 0) {
                    pushmedia()
                }
                else {
                    popmedia()
                }
            }
            FluImage {
                z: 0
                source: ""
                width: root.width
                height: parent.height
                fillMode: Image.PreserveAspectFit
                Component.onCompleted: {
                    source = ActivityReader.getMediaPath((index-1)%mediasize)
                    console.log("construct image!")
                }
            }
            focus: true
        }
    }
    Keys.onPressed: function(event) {
        console.log("press key!")
        if(stack.busy) return;
        switch(event.key) {
        case Qt.Key_Right :
            pushmedia()
            break
        case Qt.Key_Left :
            popmedia()
            break
        }
    }
    Component {
        id: videoview
        MouseArea {
            anchors.fill: parent
            propagateComposedEvents: true
            onWheel: function(wheel){
                if(stack.busy) return;
                if(wheel.angleDelta.y < 0) {
                    player.pause()
                    pushmedia()
                }
                else {
                    player.pause()
                    popmedia()
                }
            }
            FluMediaPlayer {
                id: player
                width: root.width
                height: parent.height
                Component.onCompleted: {
                    vediosource = ActivityReader.getMediaPath((index-1)%mediasize)
                    reset()
                }
                Component.onDestruction: console.log("1destruction!")
            }
            Component.onDestruction: console.log("2destruction!")
        }
    }
    Component.onCompleted: {
        activitynames = ActivityReader.getAllActivities()
        for(var i=0;i<activitynames.length;i++) {
            activitiymodel.append({text:activitynames[i]})
        }
    }

    function pushmedia(){
        console.log("push!")
        stack.pop()
        index++
        var url = ActivityReader.getMediaPath((index-1)%mediasize)
        if(MainTool.isVedio(url)) {
            stack.push(videoview)
        }
        else {
            stack.push(imageview)
        }
    }

    function popmedia(){
        console.log("pop!")
        if(index < 2) {
            showWarning(qsTr("已经到顶端了噢~"))
            return
        }
        stack.pop()
        index--
        var url = ActivityReader.getMediaPath((index-1)%mediasize)
        if(MainTool.isVedio(url)) {
            stack.push(videoview)
        }
        else {
            stack.push(imageview)
        }
    }
}
