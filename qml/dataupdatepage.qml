import QtQuick 2.15
import FluentUI 1.0
import Qt.labs.platform 1.1
import codehelper 1.0

FluScrollablePage {
    id: root
    title: qsTr("更改数据库")
    FluGroupBox {
        title: qsTr("数据库配置")
        Row {
            spacing: 36
            FluText {
                text: qsTr("打开数据文件夹")
            }
            FluTextBox {
                id: encodedirtextbox
                width: 360
                placeholderText: qsTr("输入待加密的文件夹路径")
            }
            FluButton {
                text: qsTr("打开待加密的文件夹")
                onPressed: {
                    openencodedirdialog.open()
                }
            }
            FluFilledButton {
                text: qsTr("加密")
                onClicked: {
                    if(!MainTool.direxist(encodedirtextbox.text)){
                        showError(qsTr("文件夹路径不正确！"))
                    }
                    direncoder.encodedir(encodedirtextbox.text)
                }
            }
        }
    }
    FolderDialog {
        id: openencodedirdialog
        title: qsTr("打开待加密的文件夹")
        onAccepted: encodedirtextbox.text = MainTool.topath(currentFolder)
    }
    Direncoder {
        id: direncoder
    }
}
