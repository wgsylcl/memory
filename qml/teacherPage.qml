import QtQuick
import FluentUI
import QtQuick.Layouts
import QtQuick.Controls
import teacherfilehelper

FluContentPage {
    id: root
    title: "我的老师们"

    FluStatusLayout {
        anchors.fill: parent
        errorItem: com_loaderror
        statusMode: MainTool.haveprofilepicture() ? FluStatusLayoutType.Success : FluStatusLayoutType.Error

        Component {
            id:com_loaderror
            FluFrame {
                padding: 0
                border.width: 0
                radius: 0
                color: Qt.rgba(0,0,0,0)
                ColumnLayout{
                    anchors.centerIn: parent
                    spacing: 20

                    FluImage {
                        visible: true
                        source: "qrc:/res/erroregg.png"
                        Layout.fillHeight: true
                        fillMode: Image.PreserveAspectFit
                        Layout.preferredHeight: 120
                    }

                    FluText {
                        text: "啊噢……"
                        font: FluTextStyle.BodyStrong
                        Layout.alignment: Qt.AlignHCenter
                    }

                    FluText {
                        text: "貌似图文数据库还没下载~"
                        font: FluTextStyle.BodyStrong
                        Layout.alignment: Qt.AlignHCenter
                    }

                    FluText {
                        text: "点击导航栏中的\"数据库管理 -> 数据库更新\"下载图文数据库后再来看看吧！"
                        font: FluTextStyle.BodyStrong
                        Layout.alignment: Qt.AlignHCenter
                    }
                }
            }
        }

        FluText {
            text: "您的教诲如同明灯，照亮我们前行的道路，感谢您陪伴我们度过这段美好的求学时光，愿您在新的旅程中，继续发光发热，教学事业蒸蒸日上，身体健康，生活幸福美满。"
            wrapMode: Text.WordWrap
            id: text1
        }

        ListView {
            id: teacherlistview
            Layout.fillWidth: true
            Layout.preferredWidth: parent.width
            spacing: 10
            anchors {
                top: text1.bottom
                topMargin: 5
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
            model: ListModel {
                Component.onCompleted: {
                    for(var i=0;i<12;i++)
                        append({})
                }
            }
            delegate: teachercard
            clip: true
        }

        TeacherFileReader {
            id: reader
        }

    }


    Component {
        id: teachercard
        FluContentPage {
            property string name: ""
            ColumnLayout {
                Layout.preferredWidth: teacherlistview.width - 10
                Layout.fillWidth: true
                spacing: 0
                FluWindowResultLauncher {
                    id: addteacherpetphrasewindow
                    path: "/addteacherpetphrase"
                    onResult:
                        (data) => {
                            uploader.addteacherpetphrase(name,data.text)
                            showSuccess("提交成功，可在\"数据库管理 -> 上传图文\"处查看！")
                        }
                }

                FluWindowResultLauncher {
                    id: addreviewwindow
                    path: "/addreview"
                    onResult:
                        (data) => {
                            uploader.addreviewtoteacher(data.sender,name,data.text)
                            showSuccess("提交成功，可在\"数据库管理 -> 上传图文\"处查看！")
                        }
                }

                FluWindowResultLauncher {
                    id: addpicturewindow
                    path: "/addpicture"
                    onResult:
                        (data) => {
                            uploader.addteacherpicture(name,data.addfilepaths)
                            showSuccess("提交成功，可在\"数据库管理 -> 上传图文\"处查看！")
                        }
                }

                FluFrame {
                    Layout.fillWidth: true
                    Layout.preferredWidth: teacherlistview.width - 10
                    padding: 10
                    width: teacherlistview.width
                    Layout.topMargin: 20
                    Column {
                        spacing: 10
                        FluText {
                            id: cardtitle
                            font: FluTextStyle.Subtitle
                            text: ""
                        }
                        FluText {
                            id: petphraseview
                            text: ""
                        }
                        RowLayout {
                            spacing: 10
                            FluFilledButton {
                                text: "给ta留言"
                                onClicked: addreviewwindow.launch({sendto:name})
                            }
                            FluFilledButton {
                                text: "添加ta的一刻"
                                onClicked: addpicturewindow.launch({name:name})
                            }
                            FluFilledButton {
                                text: "添加ta的口头禅"
                                onClicked: addteacherpetphrasewindow.launch({name:name})
                            }
                        }
                    }
                }

                FluExpander {
                    headerText: "ta的亲笔签名"
                    width: parent.width
                    Layout.fillWidth: true
                    Layout.preferredWidth: teacherlistview.width - 10
                    FluImage {
                        id: signimage
                        z: 1
                        anchors.fill: parent
                        asynchronous: true
                        width: parent.width
                        fillMode:Image.PreserveAspectFit
                        anchors.margins: 10

                        smooth: true

                        MouseArea {
                            anchors.fill: parent
                            onDoubleClicked: {
                                if(MainTool.isvideo(MainTool.toLocalMediaUrl(parent.source))) FluRouter.navigate("/playvideo",{videourl:MainTool.toLocalMediaUrl(parent.source)})
                                else FluRouter.navigate("/viewpicture",{pictureurl:parent.source})
                            }
                        }
                    }
                }

                FluExpander {
                    headerText: "有关ta的一刻"
                    width: parent.width
                    Layout.fillWidth: true
                    Layout.preferredWidth: teacherlistview.width - 10
                    FluCarousel {
                        id: carousel
                        anchors.fill: parent
                        delegate: Component {
                            FluImage {
                                anchors.fill: parent
                                source: model.url
                                asynchronous: true
                                width: parent.parent.width
                                fillMode: Image.PreserveAspectFit
                                anchors.margins: 10

                                smooth: true

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
                    width: parent.width
                    Layout.fillWidth: true
                    Layout.preferredWidth: teacherlistview.width - 10
                    headerText: "留言栏"
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

            Component.onCompleted: {
                reader.readdata(index)
                signimage.source = reader.getsign()
                name = reader.getname()
                cardtitle.text = "我的%1老师 —— %2".arg(reader.getsubject()).arg(reader.getname())
                var petphrase = []
                petphrase = reader.getpetphrase()
                for(var k=0;k<petphrase.length;k++) {
                    petphraseview.text += "\"%1\"\n".arg(petphrase[k])
                }
                var datas = []
                var picpaths = []
                picpaths = reader.getpicpaths()
                for(var i=0;i<picpaths.length;i++){
                    var path = picpaths[i]
                    datas.push({url:path})
                }
                carousel.model = datas
                var reviews = []
                reviews = reader.getreviews()
                for(var j=0;j<reviews.length;j++) {
                    reviewtext.text += reviews[j]
                    reviewtext.text += '\n'
                }
            }
        }
    }
}
