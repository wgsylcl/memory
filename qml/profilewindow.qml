import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import FluentUI
import profilehelper

FluWindow {
    id: window
    title: qsTr("资料卡片")
    width: 500
    height: 600
    launchMode: FluWindowType.Standard
    property string name: ""
    property int id: 0
    property int key: 0
    onInitArgument:
        (argument)=>{
            window.name = argument.name
            window.id = Number(argument.id)
            window.key = argument.key
            selfintroduction.text = argument.profile
        }

    Column{
        anchors.fill: parent
        anchors.margins: 30

        spacing: 20

        FluText {
            id: nametitle
            font: FluTextStyle.Title
            text: window.name + "(" + window.id.toString() + ")"
        }

        FluText {
            id: selfintroduction
            wrapMode: Text.WordWrap
        }

        FluExpander {
            headerText: qsTr("有关ta的一刻")
            Layout.topMargin: 20
            width: parent.width
            FluCarousel {
                id: carousel
                anchors.fill: parent
                delegate: Component {
                    Image {
                        anchors.fill: parent
                        source: model.url
                        asynchronous: true
                        width: parent.parent.width
                        fillMode:Image.PreserveAspectFit
                        anchors.margins: 10
                        MouseArea {
                            anchors.fill: parent
                            onDoubleClicked: {
                                FluRouter.navigate("/viewpicture",{pictureurl:parent.source})
                            }
                        }
                    }
                }
                Layout.topMargin: 20
                Layout.leftMargin: 5
            }
        }

        FluExpander{
            Layout.topMargin: 20
            width: parent.width
            headerText: qsTr("留言栏")
            Item{
                anchors.fill: parent
                Flickable{
                    id:scrollview
                    width: parent.width
                    height: parent.height
                    contentWidth: width
                    boundsBehavior: Flickable.StopAtBounds
                    contentHeight: reviewtext.height
                    ScrollBar.vertical: FluScrollBar {}
                    FluText{
                        id:reviewtext
                        width: scrollview.width
                        wrapMode: Text.WrapAnywhere
                        padding: 14
                        text: ""
                    }
                }
            }
        }
    }

    ProfileReader {
        id: reader
    }

    Component.onCompleted: {
        reader.readfile(window.key)
        var datas = []
        var picpaths = []
        picpaths = reader.getpicpaths()
        for(var i=0;i<picpaths.length;i++){
            var path = picpaths[i]
            datas.push({url:path})
        }
        carousel.model = datas
        var reviews = []
        reviews = reader.getreviews()
        for(var j=0;j<reviews.length;j++) {
            reviewtext.text += reviews[j]
            reviewtext.text += '\n'
        }
    }
}
