import QtQuick 2.15
import QtQuick.Layouts
import FluentUI

FluWindow {
    id: window
    width: 400
    height: 300
    modality: Qt.ApplicationModal
    launchMode: FluWindowType.SingleTask
    title: qsTr("更改生日")

    property string name: ""
    property string birthday: "选择日期"

    onInitArgument:
        (argument) => {
            name = argument.name
        }

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 60
        RowLayout {
            spacing: 10
            FluText {
                text: "更改" + name + "的生日"
                Layout.alignment: Qt.AlignHCenter
            }
            FluCalendarPicker {
                from: new Date(2008,0,1)
                to: new Date(2009,11,31)
                current: new Date(2009,0,1)
                text: birthday
                onAccepted: birthday = current.toLocaleString().split(" ")[0]
            }
        }
        FluFilledButton {
            enabled: birthday != "选择日期"
            Layout.alignment: Qt.AlignHCenter
            text: "提交更改"
            onClicked: {
                setResult({name:window.name,birthday:window.birthday})
                window.close()
            }
        }
    }

}
