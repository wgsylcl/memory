import QtQuick 2.15
import QtQuick.Window 2.15
import FluentUI 1.0

FluWindow {

    width: 1200.00
    height: 800.00
    id: rootwindow
    title: qsTr("查看图片")

    property real scale: 1.00
    property real picwidth: 0.00
    property real picheight: 0.00

    onInitArgument:
        (argument)=>{
            mainimage.source = argument.pictureurl
            console.log("view "+mainimage.source)
        }

    MouseArea {
        anchors.fill: parent

        drag.target: mainimage

        onWheel:
            (wheel) => {
                if(wheel.angleDelta.y > 0) {
                    scale *= 1.1
                }
                else {
                    scale = Math.max(0.001,scale/1.1)
                }
                mainimage.x -= (picwidth * scale - mainimage.width)/2
                mainimage.y -= (picheight * scale - mainimage.height)/2
                mainimage.width = picwidth * scale
                mainimage.height = picheight * scale
            }

        FluImage {
            id: mainimage
            x: 600.00
            y: 400.00
            onStatusChanged: {
                if(status === Image.Ready) {
                    picwidth = mainimage.width
                    picheight = mainimage.height
                    while(picwidth*scale >= rootwindow.width || picheight*scale >= rootwindow.height)
                        scale /= 1.1
                    scale = scale
                    mainimage.width *= scale
                    mainimage.height *= scale
                    x -= mainimage.width/2
                    y -= mainimage.height/2
                }
            }

        }
    }

    // Component.onCompleted: rootwindow.showMaximized()

}
