import QtQuick 2.15
import FluentUI
import QtQuick.Layouts
import Qt.labs.platform

FluWindow {
    id: window
    width: 400
    height: 300
    modality: Qt.ApplicationModal
    launchMode: FluWindowType.SingleTask
    title: qsTr("添加ta的一刻")
    property string name: ""
    property var filepaths: []

    onInitArgument:
        (argument) => {
            name = argument.name
        }

    FileDialog {
        id: openaddfilesdialog
        title: "选择要添加的新图片/视频"
        nameFilters: ["图片/视频文件 (*.mp4 *.mkv *.jpg *.jpeg *.png *.svg)"]
        fileMode: FileDialog.OpenFiles
        onAccepted: {
            for(var i=0;i<files.length;i++) {
                var filepath = MainTool.topath(files[i])
                filepaths.push(filepath)
                openaddfilestextbox.text += filepath
                openaddfilestextbox.text += '\n'
            }
        }
    }

    FluScrollablePage {
        anchors.fill: parent
        anchors.margins: 30
        spacing: 20
        ColumnLayout {
            spacing: 20
            Layout.alignment: Qt.AlignVCenter
            FluText {
                text: "给" + name + "添加ta的一刻"
                font: FluTextStyle.BodyStrong
            }
            FluButton {
                text: "浏览文件夹"
                onClicked: openaddfilesdialog.open()
            }
            FluFrame {
                Layout.preferredWidth: parent.parent.width
                Layout.fillWidth: true
                Layout.preferredHeight: openaddfilestextbox.height
                Layout.fillHeight: true
                FluText {
                    id: openaddfilestextbox
                    padding: 20
                    width: parent.width
                    wrapMode: Text.WrapAnywhere
                }
            }
            FluFilledButton {
                Layout.alignment: Qt.AlignHCenter
                text: "提交图片/视频"
                onClicked: {
                    if(!filepaths.length) {
                        showWarning("至少添加一个文件吧！")
                        return
                    }
                    setResult({addfilepaths:filepaths})
                    window.close()
                }
            }
        }

    }
}
