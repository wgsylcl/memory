import QtQuick 2.15
import QtQuick.Window 2.15
import FluentUI 1.0

FluWindow {

    width: 800
    height: 600
    id: rootwindow
    title: qsTr("查看图片")

    property real scale: 1.0
    property real picwidth: 0
    property real picheight: 0

    onInitArgument:
        (argument)=>{
            mainimage.source = argument.pictureurl
        }

    MouseArea {
        anchors.fill: parent

        drag.target: mainimage

        onWheel: {
            if(wheel.angleDelta.y > 0) {
                scale *= 1.1
            }
            else {
                scale = Math.max(0.000000001,scale/1.1)
            }
            mainimage.x -= (picwidth * scale - mainimage.width)/2
            mainimage.y -= (picheight * scale - mainimage.height)/2
            mainimage.width = picwidth * scale
            mainimage.height = picheight * scale
        }

        FluImage {
            id: mainimage
            x: 400
            y: 300
            onStatusChanged: {
                if(status == Image.Ready) {
                    picwidth = mainimage.width
                    picheight = mainimage.height
                    x -= picwidth/2
                    y -= picheight/2
                }
            }

        }
    }

}
