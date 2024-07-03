import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls
import FluentUI
import dataupdatehelper 1.0

FluWindow {
    id: window
    width: 800
    height: 600
    title: qsTr("审核")
    launchMode: FluWindowType.SingleInstance

    property var taskstack: []
    property string taskname: ""
    property var taskids: []
    property var allactivitynames: []
    property var taskneednewactivity: []

    DataUpdateHelper {
        id: dataupdatehelper
    }

    onInitArgument:
        (argument) => {
            taskstack = argument.taskstack
            taskids = dataupdatehelper.readuploadfile(taskstack[taskstack.length - 1])
            taskstack.pop()
            taskname = dataupdatehelper.gettaskfilebasename()
            allactivitynames = dataupdatehelper.getAllRemoteActivityNames()
            for(var i=0;i<taskids.length;i++) {
                var task = dataupdatehelper.gettask(taskids[i])
                var taskobj = {
                    taskid: taskids[i],
                    sender: String(task["sender"]),
                    sendto: String(task["sendto"]),
                    context: String(task["text"]),
                    type: Number(task["type"])
                }
                taskmodel.append(taskobj)
            }
        }

    FluText {
        id: windowtitle
        font: FluTextStyle.Title
        text: taskname
        anchors {
            left: parent.left
            top: parent.top
            margins: 40
        }
    }

    RowLayout {
        spacing: 25
        anchors {
            right: parent.right
            top: parent.top
            margins: 40
        }
        FluFilledButton {
            id: acceptbutton
            text: "通过"
            onClicked: {
                dataupdatehelper.acceptalltask()
                if(taskstack.length)
                    FluRouter.navigate("/checkupload",{taskstack:taskstack})
                else
                    window.close()
            }
        }
        FluButton {
            id: refusebutton
            text: "拒绝"
            onClicked: {
                if(taskstack.length)
                    FluRouter.navigate("/checkupload",{taskstack:taskstack})
                else
                    window.close()
            }
        }
    }

    ListModel {
        id: taskmodel
    }

    Component {
        id: taskcard
        FluContentPage {
            property var typeset: ["留言","留言","添加图片","添加图片","添加口头禅","添加班史","添加图片","修改自我介绍","修改生日"]
            ColumnLayout {
                spacing: 0
                Layout.preferredWidth: 500
                Layout.fillWidth: true

                property string taskid: taskmodel.get(index).taskid

                FluFrame {
                    Layout.preferredWidth: 500
                    Layout.fillWidth: true
                    padding: 20

                    RowLayout {
                        spacing: 20
                        Layout.preferredWidth: 500
                        Layout.fillWidth: true
                        FluText {
                            id: titletext
                            text: "给" + sendto + typeset[type]
                            font: FluTextStyle.BodyStrong
                            Layout.alignment: Qt.AlignHCenter
                            Component.onCompleted: {
                                if(type !== 6) return
                                for(var j=0;j<allactivitynames.length;j++)
                                    if(allactivitynames[j] === sendto)
                                        return
                                taskneednewactivity.push(taskid)
                                acceptbutton.enabled = false
                            }
                        }
                        FluButton {
                            text: "拒绝"
                            onClicked: {
                                taskmodel.remove(index)
                                dataupdatehelper.removetask(taskid)
                                var ntaskneednewactivity = []
                                for(var i=0;i<taskneednewactivity.length;i++) {
                                    var actname = taskneednewactivity[i]
                                    if(actname !== taskid)
                                        ntaskneednewactivity.push(actname)
                                }
                                taskneednewactivity = ntaskneednewactivity
                                if(!taskneednewactivity.length)
                                    acceptbutton.enabled = true
                                showSuccess("删除成功！")
                            }
                        }

                        FluWindowResultLauncher {
                            id: newactivitywindow
                            path: "/newactivity"
                            onResult:
                                (data) => {
                                    var ntaskneednewactivity = []
                                    for(var i=0;i<taskneednewactivity.length;i++) {
                                        var actname = taskneednewactivity[i]
                                        if(actname !== taskid)
                                            ntaskneednewactivity.push(actname)
                                    }
                                    taskneednewactivity = ntaskneednewactivity
                                    if(!taskneednewactivity.length)
                                        acceptbutton.enabled = true
                                    sender = data.activityname
                                    sendto = data.activityname
                                    dataupdatehelper.resetactivitytask(taskid,data.activityname,data.activitypath)
                                }
                        }

                        FluButton {
                            text: "迁移"
                            visible: type == 6
                            onClicked: {
                                newactivitywindow.launch({activitytitle:"给" + sendto + typeset[type],activityname:sendto})
                            }
                        }

                    }

                }

                FluExpander {
                    Layout.preferredWidth: 500
                    Layout.fillWidth: true
                    id: taskexpander
                    headerText: "详细信息"
                    Item{
                        anchors.fill: parent
                        visible: type !== 2 && type !== 3 && type !== 6
                        Flickable{
                            id:scrollview
                            width: parent.width
                            height: parent.height
                            contentWidth: width
                            boundsBehavior: Flickable.StopAtBounds
                            contentHeight: reviewtext.height
                            ScrollBar.vertical: FluScrollBar {}
                            FluText{
                                id:reviewtext
                                width: scrollview.width
                                wrapMode: Text.WrapAnywhere
                                text: qsTr("sender:%1\nsendto:%2\ntext:%3").arg(sender).arg(sendto).arg(context)
                                padding: 14
                            }
                        }
                    }
                    FluCarousel {
                        anchors.fill: parent
                        visible: !(type !== 2 && type !== 3 && type !== 6)
                        id: taskcarousel
                        delegate: Component {
                            Image {
                                id: taskpicture
                                z: 1
                                anchors.fill: parent
                                source: model.url
                                asynchronous: true
                                width: parent.parent.width
                                fillMode:Image.PreserveAspectFit
                                anchors.margins: 10
                                MouseArea {
                                    anchors.fill: parent
                                    onDoubleClicked: {
                                        if(MainTool.isvideo(parent.source)) FluRouter.navigate("/playvideo",{videourl:parent.source})
                                        else FluRouter.navigate("/viewpicture",{pictureurl:parent.source})
                                    }
                                }
                            }
                        }

                        Component.onCompleted: {
                            var datas = []
                            var paths = dataupdatehelper.getpaths(taskid)
                            for(var i=0;i<paths.length;i++) {
                                datas.push({url:paths[i]})
                            }
                            taskcarousel.model = datas
                        }

                        Layout.topMargin: 20
                        Layout.leftMargin: 5
                    }
                }
            }
        }
    }

    ListView {
        id: tasklistview
        anchors {
            top: windowtitle.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        model: taskmodel
        delegate: taskcard
        clip: true
        ScrollBar.vertical: FluScrollBar {}
    }

}
