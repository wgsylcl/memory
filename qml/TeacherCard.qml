import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import teacherfilehelper 1.0
import FluentUI 1.0

FluContentPage {
    id: root
    property int key: 0

    TeacherFileReader {
        id: reader
    }

    Column {
        width: parent.width
        FluFrame {
            Layout.fillWidth: true
            Layout.preferredHeight: 100
            padding: 10
            Layout.topMargin: 20
            Column {
                spacing: 0
                FluText {
                    id: cardtitle
                    font: FluTextStyle.Subtitle
                    text: ""
                }
                FluText { text: "                                                                                                                                                                                                                                                   " }
                FluText {
                    id: petphraseview
                    text: ""
                }
            }
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
                        text: ""
                        padding: 14
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        reader.readdata(key)
        cardtitle.text = qsTr("我的%1老师 —— %2").arg(reader.getsubject()).arg(reader.getname())
        var petphrase = []
        petphrase = reader.getpetphrase()
        for(var k=0;k<petphrase.length;k++) {
            petphraseview.text += qsTr("\"%1\"\n").arg(petphrase[k])
        }
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
