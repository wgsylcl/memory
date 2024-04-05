import QtQuick
import FluentUI
import QtQuick.Layouts

FluScrollablePage {
    title: qsTr("我的老师们")
    FluText {
        text: qsTr("您的教诲如同明灯，照亮我们前行的道路，感谢您陪伴我们度过这段美好的求学时光，愿您在新的旅程中，继续发光发热，教学事业蒸蒸日上，身体健康，生活幸福美满。")
        wrapMode: Text.WordWrap
    }

    Column {
        spacing: 10
        width: parent.width
        FluFrame {
            width: parent.width

        }
    }
}
