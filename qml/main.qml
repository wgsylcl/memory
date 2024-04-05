import QtQuick
import QtQuick.Window
import FluentUI

FluWindow {
    width: 668*2
    height: 373*2
    minimumWidth: 668
    minimumHeight: 373
    title: qsTr("毕业留念")
    FluImage {
        id: background
        anchors {
            top: parent.top
        }
        width: parent.parent.width
        height: width * 373 / 668
        source: "qrc:/res/school.png"
    }

    FluAcrylic {
        anchors.fill: background
        blurRadius: 20
    }

    FluNavigationView {
        anchors.fill: parent
        pageMode: FluNavigationViewType.NoStack
        displayMode: FluNavigationViewType.Auto
        id: navigationview
        items: FluObject {
            FluPaneItem {
                icon: FluentIcons.Home
                url: "qrc:/qml/homePage.qml"
                title: qsTr("主界面")
                onTap: navigationview.push(url)
            }
            FluPaneItem {
                icon: FluentIcons.GuestUser
                url: "qrc:/qml/studentPage.qml"
                title: qsTr("同学信息")
                onTap: navigationview.push(url)
            }
        }
        Component.onCompleted: setCurrentIndex(0)
    }

}
