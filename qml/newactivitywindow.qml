import QtQuick
import QtQuick.Layouts
import FluentUI
import dataupdatehelper

FluWindow {
    id: window
    width: 600
    height: 400
    title: "迁移"
    modality: Qt.ApplicationModal
    property string activitytitle: ""
    property alias factivityname: activitynamebox.text
    property alias factivitypath: activitypathbox.text

    onInitArgument:
        (argument) => {
            activitytitle = argument.activitytitle
            factivityname = argument.activityname
            var activitynames = [] , itemdatas = []
            activitynames = dataupdatehelper.getAllRemoteActivityNames()
            for(var i=0;i<activitynames.length;i++) {
                itemdatas.push({title:activitynames[i]})
            }
            activitynamebox.items = itemdatas
        }

    ColumnLayout {
        anchors.margins: 20
        anchors.centerIn: parent
        spacing: 30
        FluText {
            text: "对请求\"" + activitytitle + "\"进行活动迁移"
            font: FluTextStyle.BodyStrong
        }
        RowLayout {
            FluText {
                text: "活动名"
            }
            FluAutoSuggestBox {
                id: activitynamebox
                width: 200
                Layout.preferredWidth: 200
                Layout.fillWidth: true
                onTextChanged: {
                    factivitypath = dataupdatehelper.getrepopath(factivityname)
                }
            }
        }
        RowLayout {
            FluText {
                text: "存储路径"
            }
            FluTextBox {
                id: activitypathbox
            }
        }
        FluFilledButton {
            text: "提交"
            enabled: factivityname.length > 0 && factivitypath.length > 0
            onClicked: {
                window.setResult({activityname:factivityname,activitypath:factivitypath});
                window.close()
            }
        }
    }
    DataUpdateHelper {
        id: dataupdatehelper
    }
}
