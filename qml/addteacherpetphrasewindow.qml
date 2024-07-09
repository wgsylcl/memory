import QtQuick
import FluentUI
import QtQuick.Layouts

FluWindow {
    id: window
    width: 400
    height: 300
    modality: Qt.ApplicationModal
    launchMode: FluWindowType.SingleTask
    title: "留言"

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

    FluScrollablePage {
        anchors.fill: parent
        anchors.margins: 30
        spacing: 20
        ColumnLayout {
            spacing: 20
            anchors.centerIn: parent
            FluText {
                text: "给%1添加新的口头禅".arg(name)
                font: FluTextStyle.BodyStrong
            }
            FluMultilineTextBox {
                id: petphrasetextbox
                placeholderText: "口头禅的内容"
                Layout.preferredWidth: 200
                Layout.fillWidth: true
            }
            FluFilledButton {
                text: "提交"
                Layout.alignment: Qt.AlignHCenter
                onClicked: {
                    if(packing) {
                        showWarning("打包中不要提交噢")
                        return
                    }

                    if(MainTool.is_empty(petphrasetextbox.text)) {
                        showWarning("口头禅至少得有几个字吧！")
                        return
                    }
                    setResult({text:petphrasetextbox.text})
                    window.close()
                }
            }
        }

    }
}
