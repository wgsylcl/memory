import QtQuick 2.15
import FluentUI 1.0
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15

FluScrollablePage {
    id: root
    title: "设置和关于"
    FluGroupBox {
        title: "设置"
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
    }
}
