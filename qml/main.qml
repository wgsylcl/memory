import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import FluentUI
import Qt.labs.settings

FluWindow {
    width: 668*2
    height: 373*2
    minimumWidth: 668
    minimumHeight: 373
    title: "毕业留念"
    Settings {
        id: settings
        property bool isdeveloper: false
    }
    property alias isdeveloper: settings.isdeveloper
    property string inputpassword: ""
    property string tip: ""

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
            showSuccess("图文数据库更新完成!")
        }
        function onUpdatefailed() {
            showError("网络异常，图文数据库更新失败!")
        }
    }

    Connections {
        target: activityupdater
        function onUpdatefinished(activityname) {
            showSuccess("数据库%1更新成功!".arg('"' + activityname + '"'))
        }
        function onUpdatefailed(activityname) {
            showError("网络异常，数据库%1更新失败!".arg('"' + activityname + '"'))
        }
    }

    FluStatusLayout {
        id: mainstatuslayout
        anchors.fill: parent
        statusMode: FluStatusLayoutType.Loading
        loadingText: "连接到数据库……"
        errorItem: com_loaderror

        Component {
            id:com_loaderror
            FluFrame {
                padding: 0
                border.width: 0
                radius: 0
                color: Qt.rgba(0,0,0,0)
                ColumnLayout{
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
                        text: "网络异常，无法连接到服务器！"
                        font: FluTextStyle.BodyStrong
                        Layout.alignment: Qt.AlignHCenter
                    }

                    FluFilledButton {
                        Layout.alignment: Qt.AlignHCenter
                        text: "试试重新连接"
                        onClicked: {
                            MainTool.restartinitialize()
                            mainstatuslayout.showLoadingView()
                        }
                    }
                }
            }
        }

        FluRectangle {
            z: 1
            id: tiptext
            color: FluColors.White
            opacity: 0.5
            height: 24
            visible: tip.length
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
            FluText {
                anchors.centerIn: parent
                text: tip
            }
            Timer {
                id: changetiptimer
                interval: 3800
                repeat: true
                onTriggered: {
                    tip = MainTool.gettip()
                }
            }
        }

        FluNavigationView {
            z: 1
            buttonBack.visible: false
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
                bottom: tiptext.top
            }
            pageMode: FluNavigationViewType.NoStack
            displayMode: FluNavigationViewType.Auto
            id: navigationview
            items: FluObject {
                id: itemobj
                FluPaneItem {
                    icon: FluentIcons.Home
                    url: "qrc:/qml/homePage.qml"
                    title: "主界面"
                    onTap: navigationview.push(url)
                }
                FluPaneItem {
                    icon: FluentIcons.GuestUser
                    url: "qrc:/qml/teacherPage.qml"
                    title: "我的老师们"
                    onTap: navigationview.push(url)
                }
                FluPaneItem {
                    icon: FluentIcons.PhoneBook
                    url: "qrc:/qml/studentPage.qml"
                    title: "我和我的同学"
                    onTap: navigationview.push(url)
                }
                FluPaneItem{
                    title: "回忆"
                    icon: FluentIcons.Emoji
                    url: "qrc:/qml/activitypage.qml"
                    onTap: navigationview.push(url)
                }
                FluPaneItem{
                    title: "我们的班史"
                    icon: FluentIcons.History
                    url: "qrc:/qml/timelinePage.qml"
                    onTap: navigationview.push(url)
                }
                FluPaneItemExpander {
                    id: databaseexpander
                    icon: FluentIcons.FolderOpen
                    title: "数据库管理"
                    FluPaneItem {
                        icon: FluentIcons.SyncFolder
                        title: "同步数据库"
                        url: "qrc:/qml/datamanagepage.qml"
                        onTap: navigationview.push(url)
                    }
                    FluPaneItem {
                        icon: FluentIcons.OpenFile
                        title: "上传图文"
                        url: "qrc:/qml/dataprovidepage.qml"
                        onTap: navigationview.push(url)
                    }
                    FluPaneItem {
                        icon: isdeveloper ? FluentIcons.Unlock : FluentIcons.Lock
                        title: "更改数据库"
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
                    title: "设置和关于"
                    onTap: navigationview.push(url)
                }
                FluPaneItem {
                    visible: false
                    title: "崩溃测试"
                    onTap: MainTool.testcrash()
                }
            }

            Component.onCompleted: {
                setCurrentIndex(0)
            }
        }
    }

    FluContentDialog {
        id: passworddialog
        title: "需要验证"
        message: "输入 Developer 的密码以授权对数据库的更改"
        contentDelegate: Component {
            FluPasswordBox {
                anchors.margins: 18
                onTextChanged: inputpassword = text
            }
        }
        positiveText: "验证"
        negativeText: "取消"
        onPositiveClicked: {
            if(inputpassword === MainTool.getpassword()) {
                showSuccess("验证成功!")
                isdeveloper = true
            }
            else {
                showError("密码错误!")
            }
        }
        onNegativeClicked: showWarning("您退出了验证")
    }

    Connections {
        target: uploader
        function onPackupfinished(packfileurl) {
            showSuccess("打包完成!",0,"已保存配置文件至系统的下载文件夹，将此文件发送给lcl即可。")
            Qt.openUrlExternally(packfileurl)
        }
    }

    Connections {
        target: MainTool
        function onInitializefinished() {
            mainstatuslayout.showSuccessView()
            tip = MainTool.gettip()
            changetiptimer.start()
            if(MainTool.getlatestapplicationversion() > MainTool.getlocalapplicationversion()) {
                showInfo("有新版本可用噢，请在\"设置和关于\"中查看！",0)
            }
        }
        function onInitializefailed() {
            mainstatuslayout.showErrorView()
        }
        function onDownloadlatestappfinished() {
            showSuccess("下载最新版本安装程序成功！")
            Qt.openUrlExternally(MainTool.getdownloadurl())
        }
        function onDownloadlatestappfailed() {
            showError("网络异常，下载最新版本安装程序失败！")
        }
    }
}
