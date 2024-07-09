import QtQuick
import FluentUI

FluWindow {
    id: window
    width: 600
    height: 500
    title: "输出结果"

    onInitArgument:
        (argument) => {
            maintext.text = argument.result
        }

    FluText {
        id: maintext
        anchors.centerIn: parent
    }
}
