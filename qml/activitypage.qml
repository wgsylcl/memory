import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import FluentUI
import Qt.labs.settings

FluContentPage {
    title: qsTr("回忆")
    property var activitynames: []
    property int index: 0
    property int mediasize: 0
    property string currentactivityname: ""

    signal mediachanged

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

                mediachanged()
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
            NumberAnimation { property: "opacity"; from: 0.0; to:1.0; duration: 50 }
        }

        pushExit: Transition {

        }

        popEnter: Transition {
            // 定义淡入效果
            NumberAnimation { property: "opacity"; from: 0.0; to:1.0; duration: 50 }
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
            anchors.margins: 18
            anchors.fill: parent
            propagateComposedEvents: true
            onWheel: (wheel) => {
                         if(wheel.angleDelta.y < 0) {
                             pushmedia()
                         }
                         else {
                             popmedia()
                         }
                     }
            onDoubleClicked: FluRouter.navigate("/viewpicture",{pictureurl:image.source})

            FluImage {
                id: image
                z: 0
                width: parent.width
                height: parent.height
                fillMode: Image.PreserveAspectFit
                cache: true
                smooth: true
                source: ActivityReader.getMediaPath((index-1)%mediasize)
            }

            focus: true
        }
    }
    Settings {
        id: playsettings
        property bool loopplay: true
        property bool autoplay: true
    }
    Component {
        id: videoview
        MouseArea {
            anchors.margins: 18
            anchors.fill: parent
            propagateComposedEvents: true
            onWheel: (wheel) => {
                         if(wheel.angleDelta.y < 0) {
                             pushmedia()
                         }
                         else {
                             popmedia()
                         }
                     }

            MMediaPlayer {
                id: player
                width: parent.width
                height: parent.height
                source: ActivityReader.getMediaPath((index-1)%mediasize)
                loopplay: playsettings.loopplay
                autoplay: playsettings.autoplay

                Connections {
                    target: root
                    function onMediachanged() {
                        player.pause()
                    }
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
        mediachanged()
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
        mediachanged()
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
