import QtQuick
import QtQuick.Layouts
import FluentUI

FluWindow {
    id: window
    width: 450
    height: 400
    title: "发生崩溃"
    ColumnLayout {
        anchors.centerIn: parent
        spacing: 20
        FluImage {
            visible: true
            source: "qrc:/res/erroregg.png"

            Layout.fillHeight: true
            fillMode: Image.PreserveAspectFit
            Layout.preferredHeight: 120
        }

        FluText {
            text: "啊噢……"
            font: FluTextStyle.BodyStrong
            Layout.alignment: Qt.AlignHCenter
        }

        FluText {
            text: "程序崩溃了噢~"
            font: FluTextStyle.BodyStrong
            Layout.alignment: Qt.AlignHCenter
        }

        FluText {
            text: "把数据文件和日志文件保存到下载文件夹并发送给lcl，帮助他改进程序！"
            font: FluTextStyle.BodyStrong
            Layout.alignment: Qt.AlignHCenter
        }

        FluFilledButton {
            text: "保存这两个文件"
            Layout.alignment: Qt.AlignHCenter
            onClicked: {
                MainTool.savecrashfile();
                Qt.openUrlExternally(MainTool.getdownloadurl())
                showSuccess("保存成功！",0,"将这两个文件发送给lcl，剩下的交给他就行了。")
            }
        }

        FluButton {
            text: "重启应用程序"
            Layout.alignment: Qt.AlignHCenter
            onClicked: {
                MainTool.restartapplication()
                FluRouter.exit(0)
            }
        }

    }
}
