import QtQuick
import FluentUI
import QtQuick.Layouts

FluScrollablePage {
    title: qsTr("我的老师们")
    FluText {
        text: qsTr("您的教诲如同明灯，照亮我们前行的道路，感谢您陪伴我们度过这段美好的求学时光，愿您在新的旅程中，继续发光发热，教学事业蒸蒸日上，身体健康，生活幸福美满。")
        wrapMode: Text.WordWrap
    }

    TeacherCard {
        width: parent.width
        key: 0
    }
    TeacherCard {
        width: parent.width
        key: 1
    }
    TeacherCard {
        width: parent.width
        key: 2
    }
    TeacherCard {
        width: parent.width
        key: 3
    }
    TeacherCard {
        width: parent.width
        key: 4
    }
    TeacherCard {
        width: parent.width
        key: 5
    }
    TeacherCard {
        width: parent.width
        key: 6
    }
    TeacherCard {
        width: parent.width
        key: 7
    }
    TeacherCard {
        width: parent.width
        key: 8
    }
    TeacherCard {
        width: parent.width
        key: 9
    }
    TeacherCard {
        width: parent.width
        key: 10
    }
    TeacherCard {
        width: parent.width
        key: 11
    }

}
