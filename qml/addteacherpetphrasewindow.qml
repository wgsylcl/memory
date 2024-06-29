import QtQuick 2.15
import FluentUI
import QtQuick.Layouts

FluWindow {
    id: window
    width: 400
    height: 300
    modality: Qt.ApplicationModal
    launchMode: FluWindowType.SingleTask
    title: qsTr("留言")

    property string name: ""

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
                text: qsTr("给%1添加新的口头禅").arg(name)
                font: FluTextStyle.BodyStrong
            }
            FluMultilineTextBox {
                id: petphrasetextbox
                placeholderText: "口头禅的内容"
                Layout.preferredWidth: 200
                Layout.fillWidth: true
            }
            FluFilledButton {
                text: "提交留言"
                Layout.alignment: Qt.AlignHCenter
                onClicked: {
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
