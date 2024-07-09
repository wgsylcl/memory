import QtQuick
import QtQuick.Layouts
import FluentUI
import Qt.labs.platform
import codehelper
import dataupdatehelper

FluContentPage {
    id: root
    title: qsTr("更改数据库")

    property var uploadfilepaths: []

    ColumnLayout {

        spacing: 10

        FluText {
            text: "数据库配置"
        }

        FluFrame {
            id: setdatabasebox
            padding: 20
            RowLayout {
                spacing: 50
                FluText {
                    text: qsTr("打开数据文件夹")
                    Layout.alignment: Qt.AlignHCenter
                }
                FluTextBox {
                    id: encodedirtextbox
                    width: 360
                    placeholderText: qsTr("输入待加密的文件夹路径")
                }
                FluButton {
                    text: qsTr("打开待加密的文件夹")
                    onPressed: {
                        openencodedirdialog.open()
                    }
                }
                FluFilledButton {
                    text: qsTr("加密")
                    onClicked: {
                        if(!MainTool.direxist(encodedirtextbox.text)){
                            showError(qsTr("文件夹路径不正确！"))
                            return
                        }
                        direncoder.encodedir(encodedirtextbox.text)
                    }
                }
            }
        }
        FolderDialog {
            id: openencodedirdialog
            title: qsTr("打开待加密的文件夹")
            onAccepted: encodedirtextbox.text = MainTool.topath(currentFolder)
        }
        FluText {
            text: "配置仓库路径"
        }
        ListModel {
            id: repolistmodel
            ListElement {
                name: "students"
            }
            ListElement {
                name: "pictures"
            }
            Component.onCompleted: {
                var activitynames = []
                activitynames = ActivityReader.getAllRemoteActivities()
                for(var i=0;i<activitynames.length;i++)
                    repolistmodel.append({name:activitynames[i]})
            }
        }

        Component {
            id: repobox
            FluFrame {
                property string reponame: repolistmodel.get(index).name
                Layout.preferredWidth: root.width
                Layout.fillWidth: true
                Layout.preferredHeight: 25
                Layout.fillHeight: true
                padding: 10
                RowLayout {
                    spacing: 50
                    FluText {
                        Layout.alignment: Qt.AlignHCenter
                        Layout.preferredWidth: 100
                        Layout.fillWidth: true
                        text: reponame
                    }
                    FluTextBox {
                        Layout.preferredWidth: 600
                        Layout.fillWidth: true
                        text: dataupdatehelper.getrepopath(reponame)
                        onTextChanged: dataupdatehelper.setrepopath(reponame,text)
                    }
                    FluFilledButton {
                        text: "在此处打开终端并运行git"
                        onClicked: {
                            var result = CommandRunner.rungitcode(dataupdatehelper.getrepopath(reponame))
                            FluRouter.navigate("/showrunningresult",{result:result})
                        }
                    }
                }

            }
        }

        ListView {
            id: repolistview
            model: repolistmodel
            delegate: repobox
            spacing: 10
            clip: true
            Layout.preferredWidth: root.width
            Layout.fillWidth: true
            Layout.preferredHeight: 400
            Layout.fillHeight: true
        }

        FileDialog {
            id: openuploadfiledialog
            fileMode: FileDialog.OpenFiles
            nameFilters: ["上传配置文件 (*.muf)"]
            onAccepted: {
                uploadfilepaths = []
                for(var i=0;i<files.length;i++)
                    uploadfilepaths.push(MainTool.topath(files[i]))
                FluRouter.navigate("/checkupload",{taskstack:uploadfilepaths})
            }
        }

        FluFilledButton {
            text: "打开上传配置文件"
            onClicked: openuploadfiledialog.open()
        }

    }

    Direncoder {
        id: direncoder
    }

    DataUpdateHelper {
        id: dataupdatehelper
    }
}
