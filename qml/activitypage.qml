import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import FluentUI 1.0

FluContentPage {
    title: qsTr("回忆")
    property var activitynames: []
    // property var history: []
    property int index: 0
    property int mediasize: 0
    property string currentactivityname: ""

    id: root

    ListModel {
        id: activitiymodel
    }

    RowLayout {
        z: 1
        spacing: 20

        id: chooseactivityrow

        FluText {
            text: qsTr("选择要查看的活动")
            Layout.alignment: Qt.AlignHCenter
        }

        FluComboBox {
            id: activitybox
            model: activitiymodel
            width: 400
            Layout.preferredWidth: 400
            onCurrentIndexChanged: {
                currentactivityname = activitynames[currentIndex]
                mediasize = ActivityReader.readAllMedia(currentactivityname)
                for(var i=0;i<mediasize;i++) {
                    console.log(ActivityReader.getMediaPath(i))
                }

                stack.clear()

                index = 1
                var url = ActivityReader.getMediaPath((index-1)%mediasize)
                if(MainTool.isvideo(url)) {
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

        FluButton {
            Layout.leftMargin: 50
            text: qsTr("给活动添加图片/视频")
            onClicked: addactivitypicturewindow.launch({name:currentactivityname})
        }
    }

    FluWindowResultLauncher {
        id: addactivitypicturewindow
        path: "/addactivitypicture"
        onResult:
            (data) => {
                uploader.addactivitypicture(data.activityname,data.addfilepaths)
                showSuccess("提交成功，可在\"数据库管理 -> 上传图文\"处查看！")
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
            NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; duration: 50 }
        }

        pushExit: Transition {

        }

        popEnter: Transition {
            // 定义淡入效果
            NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; duration: 50 }
        }

        popExit: Transition {

        }
    }

    Action {
        shortcut: "Down"
        onTriggered: pushmedia()
    }

    Action {
        shortcut: "Up"
        onTriggered: popmedia()
    }

    Component {
        id: imageview
        MouseArea {
            anchors.fill: parent
            propagateComposedEvents: true
            onWheel: (wheel) => {
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
                }
            }
            focus: true
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
            }
            Action {
                shortcut: "space"
                onTriggered: {
                    if(player.isplaying())
                        player.pause()
                    else
                        player.start()
                }
            }
        }
    }
    Component.onCompleted: {
        activitynames = ActivityReader.getAllActivities()
        for(var i=0;i<activitynames.length;i++) {
            activitiymodel.append({text:activitynames[i]})
        }
    }

    function pushmedia(){
        if(stack.busy) return;
        stack.pop()
        index++
        var url = ActivityReader.getMediaPath((index-1)%mediasize)
        if(MainTool.isvideo(url)) {
            stack.push(videoview)
        }
        else {
            stack.push(imageview)
        }
    }

    function popmedia(){
        if(stack.busy) return;
        if(index < 2) {
            showWarning(qsTr("已经到顶端了噢~"))
            return
        }
        stack.pop()
        index--
        var url = ActivityReader.getMediaPath((index-1)%mediasize)
        if(MainTool.isvideo(url)) {
            stack.push(videoview)
        }
        else {
            stack.push(imageview)
        }
    }
}
