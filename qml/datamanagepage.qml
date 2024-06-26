import QtQuick 2.15
import QtQuick.Layouts
import FluentUI 1.0
import datamanagehelper 1.0

FluContentPage {
    id: root
    title: qsTr("数据库同步")

    property var remoteactivities : []

    FluText {
        id: text2
        text: qsTr("图文数据库更新")
        font: FluTextStyle.Subtitle
    }

    FluFrame {
        anchors.top: text2.bottom
        anchors.topMargin: 10
        id: profilepictureupdateview
        width: parent.width
        height: 50
        RowLayout {
            anchors.centerIn: parent
            spacing: 200
            FluText {
                Layout.alignment: Qt.AlignHCenter
                id: localprofilepictrueviewtext
                text: "本地数据库版本：" + datamanagehelper.getlocalprofilepictureversion()
                Connections {
                    target: profilepictureupdater
                    function onUpdatefinished()
                    {
                        localprofilepictrueviewtext.text = "本地数据库版本：" + datamanagehelper.getlocalprofilepictureversion()
                    }
                }
            }
            FluText {
                Layout.alignment: Qt.AlignHCenter
                id: remoteprofilepictrueviewtext
                text: "远程数据库版本：" + datamanagehelper.getremoteprofilepictureversion()
            }
            FluFilledButton {
                id: syncprofilepictruebotton
                text: "更新本地数据库"
                enabled: datamanagehelper.getlocalprofilepictureversion() < datamanagehelper.getremoteprofilepictureversion()
                onClicked: {
                    showInfo("开始更新")
                    datamanagehelper.startsyncprofilepicture()
                }
                Connections {
                    target: profilepictureupdater
                    function onUpdatefinished()
                    {
                        syncprofilepictruebotton.enabled = false
                    }
                }
            }
        }
    }

    DataManageHelper {
        id: datamanagehelper
    }

    ListModel {
        id: activitylistmodel
        Component.onCompleted: {
            remoteactivities = datamanagehelper.getallactivitiynames()
            for(var i=0;i<remoteactivities.length;i++) {
                activitylistmodel.append({name:remoteactivities[i],
                                          localversion:datamanagehelper.getlocalactivityversionbyname(remoteactivities[i]),
                                          remoteversion:datamanagehelper.getremoteactivityversionbyname(remoteactivities[i])})
            }
        }
    }

    FluText {
        id: text1
        anchors.topMargin: 25
        anchors {
            top: profilepictureupdateview.bottom
        }
        text: qsTr("活动数据库更新")
        font: FluTextStyle.Subtitle
    }

    ListView {
        anchors.topMargin: 10
        id: activitylistview
        model: activitylistmodel
        delegate: activitycard
        spacing: 10
        anchors {
            top: text1.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
    }

    Component {
        id: activitycard
        FluFrame {
            width: activitylistview.width
            height: 50
            RowLayout {
                spacing: 120
                anchors.centerIn: parent
                FluText {
                    id: activitynametext
                    text: name
                    Layout.preferredWidth: 100
                    Layout.fillWidth: true
                    font: FluTextStyle.BodyStrong
                    Layout.alignment: Qt.AlignHCenter
                }
                FluText {
                    id: activitylocalversiontext
                    Layout.preferredWidth: 100
                    Layout.fillWidth: true
                    text: datamanagehelper.getlocalactivityversionbyname(name) === "0.0.0" ?
                          "还未下载这个数据库到本地噢" :
                          "本地数据库版本：" + datamanagehelper.getlocalactivityversionbyname(name)
                    Layout.alignment: Qt.AlignHCenter
                }
                FluText {
                    id: activityremoteversiontext
                    Layout.preferredWidth: 100
                    Layout.fillWidth: true
                    text: "远程数据库版本：" + datamanagehelper.getremoteactivityversionbyname(name)
                    Layout.alignment: Qt.AlignHCenter
                }

                RowLayout {
                    spacing: 10
                    FluFilledButton {
                        id: activityupdatebotton
                        text: qsTr("更新本地数据库")
                    }
                    FluButton {
                        id: activitydeletebotton
                        text: qsTr("删除本地数据库")
                        enabled: datamanagehelper.getlocalactivityversionbyname(name) !== "0.0.0"
                    }
                }
            }
        }
    }
}
