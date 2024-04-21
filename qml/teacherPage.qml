import QtQuick 2.15
import FluentUI 1.0
import QtQuick.Layouts 1.15

FluScrollablePage {
    title: qsTr("我的老师们")
    FluText {
        text: qsTr("您的教诲如同明灯，照亮我们前行的道路，感谢您陪伴我们度过这段美好的求学时光，愿您在新的旅程中，继续发光发热，教学事业蒸蒸日上，身体健康，生活幸福美满。")
        wrapMode: Text.WordWrap
    }

    TeacherCard {
        key: 0
    }
    TeacherCard {
        key: 1
    }
    TeacherCard {
        key: 2
    }
    TeacherCard {
        key: 3
    }
    TeacherCard {
        key: 4
    }
    TeacherCard {
        key: 5
    }
    TeacherCard {
        key: 6
    }
    TeacherCard {
        key: 7
    }
    TeacherCard {
        key: 8
    }
    TeacherCard {
        key: 9
    }
    TeacherCard {
        key: 10
    }
    TeacherCard {
        key: 11
    }

}
