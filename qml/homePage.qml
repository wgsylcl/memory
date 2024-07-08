import QtQuick
import QtQuick.Layouts
import QtQuick.Window
import FluentUI
import QtQuick.Controls

FluContentPage {
    title: qsTr("毕业留念")

    FluFrame {
        width: parent.width
        height: 60
        id: frame
        FluText {
            anchors.centerIn: parent
            text: qsTr("我来自武钢实验学校，2021年入学，成为了光荣的14班的一员，在这里，我先后与46名小伙伴一起学习、交流、成长。三年的成长后，我将从这里再度起航！")
            wrapMode: Text.WordWrap
        }
    }

    FluImage {
        source: "qrc:/res/groupphoto.jpg"
        anchors {
            margins: 18
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            top: frame.bottom
        }
        fillMode: Image.PreserveAspectFit

        smooth: true

        MouseArea {
            anchors.fill: parent
            onDoubleClicked: FluRouter.navigate("/viewpicture",{pictureurl:"qrc:/res/groupphoto.jpg"})
        }
    }

}
