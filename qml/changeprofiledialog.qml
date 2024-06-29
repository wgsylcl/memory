import QtQuick 2.15
import FluentUI
import QtQuick.Layouts

FluWindow {
    id: window
    width: 400
    height: 300
    launchMode: FluWindowType.SingleTask
    modality: Qt.ApplicationModal
    title: qsTr("更改自我介绍")

    property string name: ""

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
            placeholderText: qsTr("新介绍（留空就没有了噢）")
        }
        FluFilledButton {
            text: qsTr("提交更改")
            Layout.alignment: Qt.AlignHCenter
            onClicked: {
                setResult({newprofile:MainTool.trimmed(profiletextbox.text)})
                window.close()
            }
        }
    }
}
