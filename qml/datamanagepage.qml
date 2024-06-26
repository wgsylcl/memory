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
            spacing: 180
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
            FluLoadingButton {
                id: syncprofilepictruebotton
                text: "更新本地数据库"
                enabled: (!profilepictureupdater.is_updating()) && datamanagehelper.getlocalprofilepictureversion() < datamanagehelper.getremoteprofilepictureversion()
                loading: profilepictureupdater.is_updating()
                onClicked: {
                    showInfo("开始更新图文数据库")
                    datamanagehelper.startsyncprofilepicture()
                    syncprofilepictruebotton.loading = true
                    syncprofilepictruebotton.enabled = false
                }
                Connections {
                    target: profilepictureupdater
                    function onUpdatefinished()
                    {
                        syncprofilepictruebotton.loading = false
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
                    FluLoadingButton {
                        id: activityupdatebotton
                        text: qsTr("更新本地数据库")
                        loading: activityupdater.is_updating(name)
                        enabled: (!activityupdater.is_updating(name)) && datamanagehelper.getlocalactivityversionbyname(name) < datamanagehelper.getremoteactivityversionbyname(name)
                        onClicked: {
                            showInfo(qsTr("开始更新数据库%1!").arg('"' + name + '"'))
                            datamanagehelper.startsyncactivity(name)
                            activitylocalversiontext.text = datamanagehelper.getlocalactivityversionbyname(name) === "0.0.0" ?
                                        "还未下载这个数据库到本地噢" :
                                        "本地数据库版本：" + datamanagehelper.getlocalactivityversionbyname(name)
                            activityupdatebotton.enabled = (!activityupdater.is_updating(name)) && datamanagehelper.getlocalactivityversionbyname(name) < datamanagehelper.getremoteactivityversionbyname(name)
                            activityupdatebotton.loading = activityupdater.is_updating(name)
                            activitydeletebotton.enabled = (!activityupdater.is_updating(name)) && datamanagehelper.getlocalactivityversionbyname(name) !== "0.0.0"
                        }
                    }
                    FluButton {
                        id: activitydeletebotton
                        text: qsTr("删除本地数据库")
                        enabled: (!activityupdater.is_updating(name)) && datamanagehelper.getlocalactivityversionbyname(name) !== "0.0.0"
                        onClicked: {
                            showInfo(qsTr("已删除数据库%1!").arg('"' + name + '"'))
                            datamanagehelper.removelocalactivitybyname(name)
                            activitylocalversiontext.text = datamanagehelper.getlocalactivityversionbyname(name) === "0.0.0" ?
                                        "还未下载这个数据库到本地噢" :
                                        "本地数据库版本：" + datamanagehelper.getlocalactivityversionbyname(name)
                            activityupdatebotton.enabled = datamanagehelper.getlocalactivityversionbyname(name) < datamanagehelper.getremoteactivityversionbyname(name)
                            activityupdatebotton.loading = activityupdater.is_updating(name)
                            activitydeletebotton.enabled = (!activityupdater.is_updating(name)) && datamanagehelper.getlocalactivityversionbyname(name) !== "0.0.0"
                        }
                    }
                }

                Connections {
                    target: activityupdater
                    function onUpdatefinished() {
                        activitylocalversiontext.text = datamanagehelper.getlocalactivityversionbyname(name) === "0.0.0" ?
                                    "还未下载这个数据库到本地噢" :
                                    "本地数据库版本：" + datamanagehelper.getlocalactivityversionbyname(name)
                        activityupdatebotton.enabled = (!activityupdater.is_updating(name)) && datamanagehelper.getlocalactivityversionbyname(name) < datamanagehelper.getremoteactivityversionbyname(name)
                        activityupdatebotton.loading = activityupdater.is_updating(name)
                        activitydeletebotton.enabled = (!activityupdater.is_updating(name)) && datamanagehelper.getlocalactivityversionbyname(name) !== "0.0.0"
                    }
                }
            }
        }
    }
}
