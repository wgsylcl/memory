import QtQuick
import FluentUI
import QtQuick.Layouts
import QtQuick.Window
import Qt.labs.settings

FluScrollablePage {
    id: root
    title: "设置和关于"

    Settings {
        id: settings
        property alias autoplay: autoplayswitch.checked
    }

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
                        Qt.openUrlExternally("https://gitee.com/wgsylcl/memory/releases/latest")
                    }
                }
                FluLoadingButton {
                    id: downloadlatestappbutton
                    text: "下载安装程序"
                    loading: MainTool.is_downloadinglatestapp()
                    enabled: !loading && MainTool.getlatestapplicationversion() > MainTool.getlocalapplicationversion()
                    onClicked: {
                        MainTool.downloadlatestapp()
                        showInfo("开始下载安装程序到本地！")
                        downloadlatestappbutton.loading = MainTool.is_downloadinglatestapp()
                        downloadlatestappbutton.enabled = !downloadlatestappbutton.loading && MainTool.getlatestapplicationversion() > MainTool.getlocalapplicationversion()
                    }
                }
            }
            FluToggleSwitch {
                id: autoplayswitch
                text: "自动播放视频"
                textRight: false
            }
        }
    }

    Connections {
        target: MainTool
        function onDownloadlatestappfinished() {
            downloadlatestappbutton.loading = MainTool.is_downloadinglatestapp()
            downloadlatestappbutton.enabled = !downloadlatestappbutton.loading && MainTool.getlatestapplicationversion() > MainTool.getlocalapplicationversion()
        }
        function onDownloadlatestappfailed() {
            downloadlatestappbutton.loading = MainTool.is_downloadinglatestapp()
            downloadlatestappbutton.enabled = !downloadlatestappbutton.loading && MainTool.getlatestapplicationversion() > MainTool.getlocalapplicationversion()
        }
    }

    FluGroupBox {
        title: "关于"
        ColumnLayout {
            spacing: 20
            Layout.preferredWidth: 914
            Layout.fillWidth: true
            FluText {
                width: 914
                Layout.preferredWidth: 914
                Layout.fillWidth: true
                wrapMode: Text.WrapAnywhere
                textFormat: Text.MarkdownText
                text: MainTool.getterms()
                onLinkActivated: Qt.openUrlExternally(link)
            }
            FluText {
                width: 914
                Layout.preferredWidth: 914
                Layout.fillWidth: true
                wrapMode: Text.WrapAnywhere
                textFormat: Text.MarkdownText
                text: MainTool.getpreviews()
                onLinkActivated: Qt.openUrlExternally(link)
            }
        }
    }
}
