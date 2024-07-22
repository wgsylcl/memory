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
    property alias imagex: mainimage.x
    property alias imagey: mainimage.y

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
            NumberAnimation { duration: 88 }
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
            scale: rootwindow.scale

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

    Behavior on imagex {
        NumberAnimation { duration: 138 }
    }

    Behavior on imagey {
        NumberAnimation { duration: 138 }
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
        onTriggered: {
            imagey -= 38
        }
    }

    Action {
        shortcut: "Down"
        onTriggered: {
            imagey += 38
        }
    }

    Action {
        shortcut: "Left"
        onTriggered: {
            imagex -= 38
        }
    }

    Action {
        shortcut: "Right"
        onTriggered: {
            imagex += 38
        }
    }

    Action {
        shortcut: "Ctrl+Shift+R"
        onTriggered: {
            scale = 1.0
            mainimage.width = picwidth
            mainimage.height = picheight
            mainimage.x = (rootwindow.width - mainimage.width)/2
            mainimage.y = (rootwindow.height - appBar.height - picheight*scale)/2
            showscaleframe.opacity = 0.8
            showscaletimer.restart()
        }
    }

    Action {
        shortcut: "Ctrl+R"
        onTriggered: {
            imagex = (rootwindow.width - mainimage.width)/2
            imagey = (rootwindow.height - appBar.height - mainimage.height)/2
        }
    }

    function scaleup(x,y) {
        var tscale = scale
        scale = getmin(100,scale*1.1)
        var s = scale/tscale,tx = mainimage.x,ty = mainimage.y,tw = mainimage.width,th = mainimage.height
        mainimage.width = picwidth * scale
        mainimage.height = picheight * scale
        mainimage.x += (1-s)*(x-tx-tw/2) - (mainimage.width - tw)/2
        mainimage.y += (1-s)*(y-ty-th/2) - (mainimage.height - th)/2
        showscaleframe.opacity = 0.8
        showscaletimer.restart()
    }
    function scaledown(x,y) {
        var tscale = scale
        scale = getmax(0.1,scale/1.1)
        var s = scale/tscale,tx = mainimage.x,ty = mainimage.y,tw = mainimage.width,th = mainimage.height
        mainimage.width = picwidth * scale
        mainimage.height = picheight * scale
        mainimage.x += (1-s)*(x-tx-tw/2) - (mainimage.width - tw)/2
        mainimage.y += (1-s)*(y-ty-th/2) - (mainimage.height - th)/2
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
