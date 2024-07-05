import QtQuick 2.15
import FluentUI 1.0
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15

FluScrollablePage {
    id: root
    title: "设置和关于"
    FluGroupBox {
        title: "设置"
        ColumnLayout {
            spacing: 20
            RowLayout {
                spacing: 20
                FluText {
                    text: "清理缓存文件"
                    Layout.alignment: Qt.AlignHCenter
                }
                FluButton {
                    text: "清理"
                    onClicked: {
                        MainTool.cleanlog()
                        showSuccess("清理完成！")
                    }
                }
            }
            RowLayout {
                spacing: 20
                FluText {
                    text: MainTool.getlatestapplicationversion() > MainTool.getlocalapplicationversion() ? "有新版本更新：v%1 -> v%2".arg(MainTool.getlocalapplicationversion()).arg(MainTool.getlatestapplicationversion()) : "目前已是最新版本：v" + MainTool.getlocalapplicationversion()
                    Layout.alignment: Qt.AlignHCenter
                }
                FluButton {
                    text: "查看更新"
                    enabled: MainTool.getlatestapplicationversion() > MainTool.getlocalapplicationversion()
                    onClicked: {
                        Qt.openUrlExternally("https://gitee.com/wgsylcl/memory/releases")
                    }
                }
            }
        }

    }
}
