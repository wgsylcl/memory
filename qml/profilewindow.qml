import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import Qt.labs.platform
import FluentUI 1.0

FluWindow {
    id: window
    title: qsTr("资料卡片")
    width: 500
    height: 600
    launchMode: FluWindowType.Standard
    property string name: ""
    property int id: 0
    property int key: 0
    property var addfiles: []
    property string newprofile: ""
    property string sender: ""
    property string sendtext: ""
    property var picpaths: []
    property var reviews: []

    onInitArgument:
        (argument)=>{
            window.name = argument.name
            window.id = Number(argument.id)
            window.key = argument.key
            selfintroduction.text = argument.profile
            signimage.source = argument.sign
            picpaths = argument.picpaths
            reviews = argument.reviews
        }

    FluImage {
        id: background
        source: "qrc:/res/door.png"
        anchors.centerIn: parent
        fillMode: Image.PreserveAspectCrop
        width: parent.width
        height: parent.height
    }

    FluAcrylic {
        anchors.fill: background
        blurRadius: 20
    }

    FluWindowResultLauncher {
        id: changeprofilewindow
        path: "/changeprofile"
        onResult:
            (data) => {
                uploader.addstudentprofile(window.name,data.newprofile)
                showSuccess("提交成功，可在\"数据库管理 -> 上传图文\"处查看！")
            }
    }

    FluWindowResultLauncher {
        id: addreviewwindow
        path: "/addreview"
        onResult:
            (data) => {
                uploader.addreviewtostudent(data.sender,window.name,data.text)
                showSuccess("提交成功，可在\"数据库管理 -> 上传图文\"处查看！")
            }
    }

    FluWindowResultLauncher {
        id: addpicturewindow
        path: "/addpicture"
        onResult:
            (data) => {
                uploader.addstudentpicture(window.name,data.addfilepaths)
                showSuccess("提交成功，可在\"数据库管理 -> 上传图文\"处查看！")
            }
    }

    FluScrollablePage {
        anchors.fill: parent
        anchors.margins: 20

        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true

            FluText {
                id: nametitle
                font: FluTextStyle.Title
                text: window.name + "(" + window.id.toString() + ")"
            }

            FluText {
                id: selfintroduction
                wrapMode: Text.WordWrap
            }

            RowLayout {
                spacing: 20
                FluFilledButton {
                    text: qsTr("修改自我介绍")
                    onClicked: changeprofilewindow.launch({name:window.name})
                }
                FluFilledButton {
                    text: qsTr("给ta留言")
                    onClicked: addreviewwindow.launch({sendto:window.name})
                }
                FluFilledButton {
                    text: qsTr("添加ta的一刻")
                    onClicked: addpicturewindow.launch({name:window.name})
                }
            }

            FluExpander {
                headerText: qsTr("ta的亲笔签名")
                Layout.topMargin: 20
                width: parent.width
                FluImage {
                    id: signimage
                    z: 1
                    anchors.fill: parent
                    asynchronous: true
                    width: parent.width
                    fillMode:Image.PreserveAspectFit
                    anchors.margins: 10
                    MouseArea {
                        anchors.fill: parent
                        onDoubleClicked: {
                            if(MainTool.isvideo(MainTool.toLocalMediaUrl(parent.source))) FluRouter.navigate("/playvideo",{videourl:MainTool.toLocalMediaUrl(parent.source)})
                            else FluRouter.navigate("/viewpicture",{pictureurl:MainTool.toLocalMediaUrl(parent.source)})
                        }
                    }
                }
            }

            FluExpander {
                headerText: qsTr("有关ta的一刻")
                Layout.topMargin: 20
                width: parent.width
                FluCarousel {
                    id: carousel
                    anchors.fill: parent
                    delegate: Component {
                        FluImage {
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
                                    if(MainTool.isvideo(MainTool.toLocalMediaUrl(parent.source))) FluRouter.navigate("/playvideo",{videourl:MainTool.toLocalMediaUrl(parent.source)})
                                    else FluRouter.navigate("/viewpicture",{pictureurl:MainTool.toLocalMediaUrl(parent.source)})
                                }
                            }
                        }
                    }
                    Layout.topMargin: 20
                    Layout.leftMargin: 5
                }
            }

            FluExpander{
                Layout.topMargin: 20
                width: parent.width
                headerText: qsTr("留言栏")
                Item{
                    anchors.fill: parent
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
                            text: ""
                            padding: 14
                        }
                    }
                }
            }
        }

    }

    Component.onCompleted: {
        var datas = []
        for(var i=0;i<picpaths.length;i++){
            var path = picpaths[i]
            datas.push({url:path})
        }
        carousel.model = datas
        for(var j=0;j<reviews.length;j++) {
            reviewtext.text += reviews[j]
            reviewtext.text += '\n'
        }
    }

}
