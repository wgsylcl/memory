import QtQuick 2.15
import QtQuick.Window 2.15
import FluentUI 1.0

FluWindow {
    width: 668*2
    height: 373*2
    minimumWidth: 668
    minimumHeight: 373
    title: qsTr("毕业留念")
    property bool isdeveloper: true
    property string inputpassword: ""

    FluImage {
        id: background
        anchors.centerIn: parent
        width: parent.width
        height: parent.height
        fillMode: Image.PreserveAspectCrop
        source: "qrc:/res/school.png"
    }

    FluAcrylic {
        anchors.fill: background
        blurRadius: 20
    }

    Component.onDestruction: {
        FluRouter.exit(0)
    }

    Connections {
        target: profilepictureupdater
        function onUpdatefinished() {
            showSuccess(qsTr("图文数据库更新完成!"))
        }
    }

    Connections {
        target: activityupdater
        function onUpdatefinished(activityname) {
            showSuccess(qsTr("数据库%1更新成功!").arg('"' + activityname + '"'))
        }
    }

    FluNavigationView {
        z: 1
        anchors.fill: parent
        pageMode: FluNavigationViewType.NoStack
        displayMode: FluNavigationViewType.Auto
        id: navigationview
        items: FluObject {
            id: itemobj
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
            FluPaneItem{
                title: qsTr("回忆")
                icon: FluentIcons.Emoji
                url: "qrc:/qml/activitypage.qml"
                onTap: navigationview.push(url)
            }
            FluPaneItem{
                title: qsTr("我们的班史")
                icon: FluentIcons.History
                url: "qrc:/qml/timelinePage.qml"
                onTap: navigationview.push(url)
            }
            FluPaneItemExpander {
                icon: FluentIcons.FolderOpen
                title: qsTr("数据库管理")
                FluPaneItem {
                    icon: FluentIcons.SyncFolder
                    title: qsTr("同步数据库")
                    url: "qrc:/qml/datamanagepage.qml"
                    onTap: navigationview.push(url)
                }
                FluPaneItem {
                    icon: FluentIcons.OpenFile
                    title: qsTr("上传图文")
                    url: "qrc:/qml/dataprovidepage.qml"
                    onTap: navigationview.push(url)
                }
                FluPaneItem {
                    icon: isdeveloper ? FluentIcons.Unlock : FluentIcons.Lock
                    title: qsTr("更改数据库")
                    url: "qrc:/qml/dataupdatepage.qml"
                    onTap: {
                        if(!isdeveloper) passworddialog.open()
                        if(isdeveloper) navigationview.push(url)
                    }
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
            // FluPaneItem {
            //     icon: FluentIcons.TaskView
            //     url: "qrc:/qml/taskpage.qml"
            //     title: qsTr("任务面板")
            //     onTap: navigationview.push(url)
            // }
        }

        Component.onCompleted: {
            setCurrentIndex(0)
        }
    }

    FluContentDialog {
        id: passworddialog
        title: qsTr("需要验证")
        message: qsTr("输入 Developer 的密码以授权对数据库的更改")
        contentDelegate: Component {
            FluPasswordBox {
                anchors.margins: 18
                onTextChanged: inputpassword = text
            }
        }
        positiveText: qsTr("验证")
        negativeText: qsTr("取消")
        onPositiveClicked: {
            if(inputpassword === MainTool.getpassword()) {
                showSuccess(qsTr("验证成功!"))
                isdeveloper = true
            }
            else {
                showError(qsTr("密码错误!"))
            }
        }
        onNegativeClicked: showWarning(qsTr("您退出了验证"))
    }

    // onDestroyed: FluApp.deleteLater()

}
