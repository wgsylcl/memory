import QtQuick
import QtQuick.Window
import QtQuick.Controls
import FluentUI

FluWindow {

    width: 1200.00
    height: 800.00
    id: rootwindow
    title: "查看图片"

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
                    scaleup(wheel.x,wheel.y)
                }
                else {
                    scaledown(wheel.x,wheel.y)
                }
            }

        FluImage {
            id: mainimage
            x: 600.00
            y: 400.00
            scale: scale

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
        onTriggered: scaleup(rootwindow.width/2,rootwindow.height/2)
    }

    Action {
        shortcut: "Ctrl+-"
        onTriggered: scaledown(rootwindow.width/2,rootwindow.height/2)
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

    Action {
        shortcut: "Ctrl+Shift+R"
        onTriggered: {
            scale = 1.0
            mainimage.width = picwidth
            mainimage.height = picheight
            mainimage.x = (rootwindow.width - picwidth)/2
            mainimage.y = (rootwindow.height - picheight)/2
            mainimage.width = picwidth*scale
            mainimage.height = picheight*scale
            showscaleframe.opacity = 0.8
            showscaletimer.restart()
        }
    }

    Action {
        shortcut: "Ctrl+R"
        onTriggered: {
            mainimage.x = (rootwindow.width - mainimage.width)/2
            mainimage.y = (rootwindow.height - mainimage.height)/2
        }
    }

    function scaleup(x,y) {
        var tscale = scale
        scale = getmin(100,scale*1.1)
        var s = scale/tscale,tx = mainimage.x,ty = mainimage.y
        mainimage.x -= (picwidth * scale - mainimage.width)/2
        mainimage.y -= (picheight * scale - mainimage.height)/2
        mainimage.x += (1-s)*(x-tx-mainimage.width/2)
        mainimage.y += (1-s)*(y-ty-mainimage.height/2)
        mainimage.width = picwidth * scale
        mainimage.height = picheight * scale
        showscaleframe.opacity = 0.8
        showscaletimer.restart()
    }
    function scaledown(x,y) {
        var tscale = scale
        scale = getmax(0.1,scale/1.1)
        var s = scale/tscale,tx = mainimage.x,ty = mainimage.y
        mainimage.x -= (picwidth * scale - mainimage.width)/2
        mainimage.y -= (picheight * scale - mainimage.height)/2
        mainimage.x += (1-s)*(x-tx-mainimage.width/2)
        mainimage.y += (1-s)*(y-ty-mainimage.height/2)
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
