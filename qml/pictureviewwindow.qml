import QtQuick
import QtQuick.Window
import QtQuick.Controls
import FluentUI

FluWindow {

    width: 1200.00
    height: 800.00
    id: rootwindow
    title: qsTr("查看图片")

    property real scale: 1.00
    property real picwidth: 0.00
    property real picheight: 0.00
    property int intscale: scale*100

    onInitArgument:
        (argument)=>{
            mainimage.source = argument.pictureurl
        }

    Timer {
        id: showscaletimer
        interval: 1800
        onTriggered: showscaleframe.opacity = 0
    }

    FluFrame {
        id: showscaleframe
        opacity: 0
        padding: 13
        z: 18
        anchors.centerIn: parent

        FluText {
            anchors.centerIn: parent
            text: intscale + "%"
        }

        Behavior on opacity {
            SequentialAnimation {
                NumberAnimation { duration: 88 }
            }
        }
    }

    MouseArea {
        anchors.fill: parent

        drag.target: mainimage

        onWheel:
            (wheel) => {
                if(wheel.angleDelta.y > 0) {
                    scaleup()
                }
                else {
                    scaledown()
                }
            }

        FluImage {
            id: mainimage
            x: 600.00
            y: 400.00
            scale: rootwindow.scale
            cache: true
            smooth: true

            onStatusChanged: {
                if(status === Image.Ready) {
                    picwidth = mainimage.width
                    picheight = mainimage.height
                    while(picwidth*scale >= rootwindow.width - 18 || picheight*scale >= rootwindow.height - 18)
                        scale /= 1.1
                    x -= mainimage.width/2
                    y -= mainimage.height/2
                    if(intscale === 100) return
                    showscaleframe.opacity = 0.8
                    showscaletimer.restart()
                }
            }
        }
    }

    Action {
        shortcut: "Ctrl+="
        onTriggered: scaleup()
    }

    Action {
        shortcut: "Ctrl+-"
        onTriggered: scaledown()
    }

    Action {
        shortcut: "Up"
        onTriggered: mainimage.y -= 18
    }

    Action {
        shortcut: "Down"
        onTriggered: mainimage.y += 18
    }

    Action {
        shortcut: "Left"
        onTriggered: mainimage.x -= 18
    }

    Action {
        shortcut: "Right"
        onTriggered: mainimage.x += 18
    }

    function scaleup() {
        scale = getmin(10,scale*1.1)
        mainimage.x -= (picwidth * scale - mainimage.width)/2
        mainimage.y -= (picheight * scale - mainimage.height)/2
        mainimage.width = picwidth * scale
        mainimage.height = picheight * scale
        showscaleframe.opacity = 0.8
        showscaletimer.restart()
    }
    function scaledown() {
        scale = getmax(0.1,scale/1.1)
        mainimage.x -= (picwidth * scale - mainimage.width)/2
        mainimage.y -= (picheight * scale - mainimage.height)/2
        mainimage.width = picwidth * scale
        mainimage.height = picheight * scale
        showscaleframe.opacity = 0.8
        showscaletimer.restart()
    }
    function getmax(a,b) {
        return a > b ? a : b
    }
    function getmin(a,b) {
        return a < b ? a : b
    }
}
