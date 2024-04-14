import QtQuick 2.15
import FluentUI 1.0
import QtQuick.Layouts 1.15

FluScrollablePage {

    Column {
        spacing: 20

        FluText {
            text: qsTr("设置")
            font: FluTextStyle.Title
        }

        FluFrame {
            Layout.fillWidth: true
            Layout.topMargin: 20
            Layout.preferredHeight: 60
            padding: 10
            Row{
                spacing: 20
                anchors.verticalCenter: parent.verticalCenter
                FluText{
                    text: qsTr("数据库更新（暂未完成）")
                    font: FluTextStyle.Body
                    anchors.verticalCenter: parent.verticalCenter
                }
                FluLoadingButton{
                    id: btn_checkupdate
                    text: qsTr("检查更新")
                    anchors.verticalCenter: parent.verticalCenter
                    onClicked: {
                        loading = true
                    }
                    enabled: false
                }
            }
        }

        FluText {
            text: qsTr("关于")
            font: FluTextStyle.Title
        }
    }


}
