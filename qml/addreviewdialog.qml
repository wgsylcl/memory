import QtQuick
import FluentUI
import QtQuick.Layouts

FluWindow {
    id: window
    width: 400
    height: 300
    modality: Qt.ApplicationModal
    launchMode: FluWindowType.SingleTask
    title: qsTr("留言")

    property string sendto: ""

    onInitArgument:
        (argument) => {
            sendto = argument.sendto
        }

    FluScrollablePage {
        anchors.fill: parent
        anchors.margins: 30
        spacing: 20
        ColumnLayout {
            spacing: 20
            anchors.centerIn: parent
            FluText {
                text: qsTr("给%1留言").arg(sendto)
                font: FluTextStyle.BodyStrong
            }
            RowLayout {
                spacing: 10
                FluText {
                    text: "留言者"
                    Layout.alignment: Qt.AlignHCenter
                }
                FluTextBox {
                    id: sendertextbox
                    placeholderText: "留空则为匿名"
                    text: uploader.getlastsender()
                }
            }
            RowLayout {
                spacing: 10
                FluText {
                    text: "留言内容"
                    Layout.alignment: Qt.AlignHCenter
                }
                FluMultilineTextBox {
                    id: sendtextbox
                    placeholderText: "写上你要给ta留言的内容吧！"
                }
            }
            FluFilledButton {
                text: "提交留言"
                Layout.alignment: Qt.AlignHCenter
                onClicked: {
                    if(MainTool.is_empty(sendtextbox.text)) {
                        showWarning("至少留几个字吧！")
                        return
                    }
                    setResult({sender:MainTool.trimmed(sendertextbox.text),text:sendtextbox.text})
                    window.close()
                }
            }
        }

    }
}
