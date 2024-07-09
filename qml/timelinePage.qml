import QtQuick
import QtQuick.Layouts
import FluentUI
import timelinehelper
import QtQuick.Controls

FluContentPage {
    title: "我们的班史"

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

        FluScrollablePage {
            anchors.fill: parent
            FluText {
                text: "由于此页面不便修改，故请将修改请求直接反馈给lcl"
            }

            FluTimeline{
                id: timeline
                Layout.fillWidth: true
                Layout.topMargin: 20
                Layout.bottomMargin: 20
                mode: FluTimelineType.Alternate
                model: timelimelist
            }

            TimelineReader {
                id: timelinereader
            }

            Component.onCompleted: {
                var length = timelinereader.readdata()
                for(var i = 0 ; i < length ; i++) {
                    var obj = timelinereader.at(i)
                    timelimelist.append({lable:obj["lable"],text:obj["text"]})
                }
            }
        }

    }

    ListModel {
        id: timelimelist
    }
}
