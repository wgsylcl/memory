import QtQuick
import FluentUI

FluWindow {
    id: window
    title: qsTr("资料卡片")
    width: 500
    height: 600
    launchMode: FluWindowType.Standard
    property string name: ""
    property int id: 0
    onInitArgument:
        (argument)=>{
            window.name = argument.name
            window.id = Number(argument.id)
        }

    Column{
        anchors.fill: parent
        anchors.margins: 20
        FluText {
            id: nametitle
            font: FluTextStyle.Title
            text: window.name + "(" + window.id.toString() + ")"
        }
    }

}
