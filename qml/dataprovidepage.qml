import QtQuick
import FluentUI
import QtQuick.Controls
import QtQuick.Layouts

FluContentPage {
    id: root
    title: "上传图文"

    property bool packing: uploader.is_packing()
    property bool havetask: uploader.gettaskcount() > 0

    ListModel {
        id: taskmodel
    }

    FluLoadingButton {
        text: "打包所有任务"
        loading: packing
        id: packbotton
        enabled: (!packing) && havetask
        onClicked: {
            showInfo("开始打包!")
            packing = true
            uploader.packup()
            havetask = false
            taskmodel.clear()
        }
        anchors {
            top: parent.top
        }
    }

    FluText {
        id: text1
        text: "上传任务列表"
        font: FluTextStyle.BodyStrong
        anchors {
            top: packbotton.bottom
            left: parent.left
            right: parent.right
            topMargin: 20
        }
    }

    Connections {
        target: uploader
        function onPackupfinished() {
            packing = false
        }
    }

    function gettaskexpanderhreadertext(index) {
        let rets = ""
        const retbase = [
                          "给%1留言".arg(uploader.getsendto(index)),
                          "给%1留言".arg(uploader.getsendto(index)),
                          "给%1添加ta的一刻".arg(uploader.getsendto(index)),
                          "给%1添加ta的一刻".arg(uploader.getsendto(index)),
                          "添加%1的口头禅".arg(uploader.getsendto(index)),
                          "增添班史",
                          "给活动\"%1\"添加图片/视频".arg(uploader.getsendto(index)),
                          "修改%1的自我介绍".arg(uploader.getsendto(index)),
                          "修改%1的生日".arg(uploader.getsendto(index))
                      ]
        return retbase[uploader.gettasktype(index)]
    }

    function gettaskimagedata(index) {
        var datas = [] , temps = []
        temps = uploader.geturls(index)
        for(var i = 0 ; i < temps.length ; i++) {
            datas.push({url:temps[i]})
        }
        return datas
    }

    function gettasktext(index) {
        const retbase = [
                          "%1:%2".arg(uploader.getsender(index)).arg(uploader.gettext(index)),
                          "%1:%2".arg(uploader.getsender(index)).arg(uploader.gettext(index)),
                          "",
                          "",
                          "\"%1\"".arg(uploader.gettext(index)),
                          "%1:%2".arg(uploader.getsender(index)).arg(uploader.gettext(index)),
                          "",
                          "%1".arg(uploader.gettext(index)),
                          "%1的生日是：%2".arg(uploader.getsendto(index)).arg(uploader.gettext(index))
                      ]
        return retbase[uploader.gettasktype(index)]
    }

    function isimagetask(index) {
        const retbase = [false,false,true,true,false,false,true,false,false]
        return retbase[uploader.gettasktype(index)]
    }

    Component {
        id: taskcard
        FluContentPage {
            property string taskid: taskmodel.get(index).taskid
            ColumnLayout {
                spacing: 0
                Layout.preferredWidth: 500
                Layout.fillWidth: true

                FluFrame {
                    Layout.preferredWidth: 500
                    Layout.fillWidth: true
                    padding: 20

                    RowLayout {
                        spacing: 20
                        Layout.preferredWidth: 500
                        Layout.fillWidth: true
                        FluText {
                            text: gettaskexpanderhreadertext(taskid)
                            font: FluTextStyle.BodyStrong
                            Layout.alignment: Qt.AlignHCenter
                        }
                        FluButton {
                            text: "撤销上传"
                            Layout.alignment: Qt.RightEdge
                            enabled: !packing
                            onClicked: {
                                taskmodel.remove(index)
                                uploader.removetask(taskid)
                                havetask = uploader.gettaskcount() > 0
                                showSuccess("删除成功！")
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
                        visible: !isimagetask(taskid)
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
                                text: gettasktext(taskid)
                                padding: 14
                            }
                        }
                    }
                    FluCarousel {
                        anchors.fill: parent
                        visible: isimagetask(taskid)
                        id: taskcarousel
                        delegate: Component {
                            FluImage {
                                id: taskpicture
                                z: 1
                                anchors.fill: parent
                                source: uploader.tourl(model.url)
                                asynchronous: true
                                width: parent.parent.width
                                fillMode:Image.PreserveAspectFit
                                anchors.margins: 10

                                smooth: true

                                MouseArea {
                                    anchors.fill: parent
                                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                                    onDoubleClicked:
                                        (mouse) => {
                                            if(mouse.button === Qt.RightButton) return
                                            if(MainTool.isvideo(MainTool.toLocalMediaUrl(parent.source))) FluRouter.navigate("/playvideo",{videourl:uploader.getlocalmediaurl(parent.source)})
                                            else FluRouter.navigate("/viewpicture",{pictureurl:parent.source})
                                        }
                                    onClicked:
                                        (mouse) => {
                                            if(mouse.button === Qt.LeftButton) return
                                            d.deleteitem = parent.source
                                            rightbuttonmenu.popup()
                                        }
                                }
                            }
                        }
                        model: gettaskimagedata(taskid)
                        Layout.topMargin: 20
                        Layout.leftMargin: 5
                    }

                    QtObject {
                        id: d
                        property string deleteitem
                    }

                    FluMenu {
                        id: rightbuttonmenu
                        FluMenuItem {
                            text: "删除此图片/视频"
                            onClicked: {
                                uploader.removepicture(taskid,d.deleteitem)
                                taskcarousel.model = gettaskimagedata(taskid)
                                if(!taskcarousel.model.length) {
                                    taskmodel.remove(index)
                                    uploader.removetask(taskid)
                                    havetask = uploader.gettaskcount() > 0
                                    showWarning("图片/视频已经清空，自动删除该任务！")
                                }
                            }
                        }
                    }
                }
            }
        }
    }


    ListView {
        anchors {
            top: text1.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            topMargin: 10
        }
        ScrollBar.vertical: FluScrollBar {}
        model: taskmodel
        delegate: taskcard
        spacing: 10
        clip: true
        id: taskview
    }

    Component.onCompleted: {
        var taskids = []
        taskids = uploader.getalltaskids()
        for(var i=0;i<taskids.length;i++) {
            taskmodel.append({taskid:taskids[i]})
        }
    }

}
