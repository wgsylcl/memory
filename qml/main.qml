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
            centerIn: parent
        }
        source: "qrc:/res/school.png"
    }

    onWidthChanged: {
        if(width*373 <= height*668){
            background.height = height
            background.width = height*668/373
        }
        else{
            background.width = width
            background.height = width*373/668
        }
    }

    onHeightChanged: {
        if(width*373 <= height*668){
            background.height = height
            background.width = height*668/373
        }
        else{
            background.width = width
            background.height = width*373/668
        }
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
                url: "qrc:/qml/teacherPage.qml"
                title: qsTr("我的老师们")
                onTap: navigationview.push(url)
            }
            FluPaneItem {
                icon: FluentIcons.PhoneBook
                url: "qrc:/qml/studentPage.qml"
                title: qsTr("我和我的同学")
                onTap: navigationview.push(url)
            }
            FluPaneItemExpander{
                title: qsTr("回忆")
                icon: FluentIcons.Emoji
                FluPaneItem {

                }
            }
        }
        footerItems: FluObject {
            FluPaneItem {
                icon: FluentIcons.Settings
                url: "qrc:/qml/settingsPage.qml"
                title: qsTr("设置和关于")
                onTap: navigationview.push(url)
            }
        }

        Component.onCompleted: setCurrentIndex(0)
    }

}
