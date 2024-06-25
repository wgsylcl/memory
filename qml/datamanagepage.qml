import QtQuick 2.15
import QtQuick.Layouts
import FluentUI 1.0

FluScrollablePage {
    id: root
    title: qsTr("数据库同步")
    FluGroupBox {
        title: qsTr("图文数据库更新")
        width: parent.width
        RowLayout {
            spacing: 100
            FluText {
                Layout.alignment: Qt.AlignHCenter
                id: localprofilepictureviewtext
                text: "本地数据库版本："
            }
            FluText {
                Layout.alignment: Qt.AlignHCenter
                id: remoteprofilepictureviewtext
                text: "远程数据库版本："
            }
            FluFilledButton {
                id: syncprofilepicturebotton
                text: "更新本地数据库"
            }
        }
    }
    FluGroupBox {
        title: qsTr("活动数据库更新")
        width: parent.width
    }
}
