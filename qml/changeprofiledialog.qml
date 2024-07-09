import QtQuick
import FluentUI
import QtQuick.Layouts

FluWindow {
    id: window
    width: 400
    height: 300
    launchMode: FluWindowType.SingleTask
    modality: Qt.ApplicationModal
    title: "更改自我介绍"

    property string name: ""
    property bool packing: uploader.is_packing()

    Connections {
        target: uploader
        function onPackupfinished() {
            packing = false
        }
    }

    onInitArgument:
        (argument) => {
            name = argument.name
        }

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 30
        FluText {
            text: "更改" + name + "的自我介绍"
            font: FluTextStyle.BodyStrong
        }
        FluTextBox {
            id: profiletextbox
            placeholderText: "新介绍（留空就没有了噢）"
        }
        FluFilledButton {
            text: "提交更改"
            Layout.alignment: Qt.AlignHCenter
            onClicked: {
                if(packing) {
                    showWarning("打包中不要提交噢")
                    return
                }

                setResult({newprofile:MainTool.trimmed(profiletextbox.text)})
                window.close()
            }
        }
    }
}
