import QtQuick 2.15
import QtQuick.Layouts
import FluentUI
import timelinehelper 1.0

FluScrollablePage {
    title: qsTr("我们的班史")

    FluText {
        text: "由于此页面不便修改，故请将修改请求直接反馈给lcl"
    }

    ListModel {
        id: timelimelist
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
