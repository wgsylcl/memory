import QtQuick
import QtQuick.Layouts
import FluentUI
import timelinehelper

FluScrollablePage {
    title: qsTr("毕业留念")

    ListModel {
        id: timelimelist
    }

    FluFrame {
        Layout.fillWidth: true
        Layout.preferredHeight: 60
        FluText {
            anchors.centerIn: parent
            text: qsTr("我来自武钢实验学校，2021年入学，成为了光荣的14班的一员，在这里，我先后与46名小伙伴一起学习、交流、成长。三年的成长后，我将从这里再度起航！")
            wrapMode: Text.WordWrap
        }
    }

    FluTimeline{
        id: timeline
        Layout.fillWidth: true
        Layout.topMargin: 20
        Layout.bottomMargin: 20
        mode: FluTimelineType.Alternate
        model: timelimelist
    }

    TimelineReader {
        id: timelinereader
    }

    Component.onCompleted: {
        var length = timelinereader.readdata()
        for(var i = 0 ; i < length ; i++) {
            var obj = timelinereader.at(i)
            timelimelist.append({lable:obj["lable"],text:obj["text"]})
        }
    }

}
