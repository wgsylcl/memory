import QtQuick
import QtMultimedia
import QtQuick.Controls
import QtQuick.Layouts
import FluentUI
import Qt.labs.settings

FluContentPage {
    id: root
    animationEnabled: false
    property alias source: player.source
    property alias loopplay: loopswitch.checked

    Settings {
        property alias loopplay: root.loopplay
    }

    QtObject{
        id:d
        property bool flag: true
        property real currVolume: 1.0
    }
    FluRectangle {
        id: background
        z: 0
        color: FluColors.Black
        anchors.fill: parent
    }
    ListModel{
        id:speedmodel
        ListElement{
            text:"3.0x"
            value:3.0
        }
        ListElement{
            text:"2.0x"
            value:2.0
        }
        ListElement{
            text:"1.5x"
            value:1.5
        }
        ListElement{
            text:"1.25x"
            value:1.25
        }
        ListElement{
            text:"1.0x"
            value:1.0
        }
        ListElement{
            text:"0.75x"
            value:0.75
        }
        ListElement{
            text:"0.5x"
            value:0.5
        }
    }

    MediaPlayer {
        id: player
        audioOutput:
            AudioOutput {
            id: audiooutput
            volume: d.currVolume
        }
        videoOutput: videooutput
        onPositionChanged:
            (position) => {
                if(d.flag)
                slider.value = position
            }
        onPlaybackStateChanged: {
            if(playbackState === MediaPlayer.StoppedState) {
                player.play()
                player.pause()
                player.setPosition(0)
                if(loopplay)
                    player.play()
            }
        }
        playbackRate: speedmodel.get(speedmenu.currIndex).value
    }

    VideoOutput {
        id: videooutput
        anchors.fill: parent
    }

    Timer {
        id: showtimer
        interval: 3000
        onTriggered: hidecontrol()
        Component.onCompleted: showtimer.start()
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onCanceled: {
            hidecontrol()
        }
        onExited: {
            hidecontrol()
        }
        onPositionChanged:
            (mouse) => {
                control.opacity = true
                if(mouse.x > control.x && mouse.y > control.y && mouse.x <control.x + control.width && mouse.y<control.y + control.height){
                    showtimer.stop()
                }
                else{
                    showtimer.restart()
                }
            }
        onClicked:
            (mouse) => {
                if(!(mouse.x > control.x && mouse.y > control.y && mouse.x <control.x + control.width && mouse.y<control.y + control.height)){
                    if(player.playing){
                        player.pause()
                    }else{
                        player.play()
                    }
                }
            }

        FluFrame {
            id:control
            width: parent.width-30
            height: 100
            opacity: 1
            visible: opacity
            anchors{
                horizontalCenter: parent.horizontalCenter
                bottom: parent.bottom
                bottomMargin: 38
            }
            Behavior on opacity {
                NumberAnimation { duration: 88 }
            }
            MouseArea{
                anchors.fill: parent
            }
            FluSlider{
                id:slider
                anchors{
                    left: parent.left
                    right: parent.right
                    leftMargin: 15
                    rightMargin: 15
                    top: parent.top
                    topMargin: 15
                }
                text:formatDuration(value)
                from: 0
                to: player.duration
                value: player.position
                onPressedChanged: {
                    if(pressed){
                        d.flag = false
                    }else {
                        player.setPosition(value)
                        d.flag = true
                    }
                }
            }
            FluIconButton{
                iconSource: player.playing ? FluentIcons.Pause : FluentIcons.Play
                anchors{
                    horizontalCenter: parent.horizontalCenter
                    bottom: parent.bottom
                    bottomMargin: 5
                }
                width: 30
                height: 30
                iconSize: 20
                verticalPadding: 0
                horizontalPadding: 0
                onClicked: {
                    if(player.playing){
                        player.pause()
                    }else{
                        player.play()
                    }
                }
            }
            FluText{
                text: formatDuration(player.position)
                anchors{
                    left: slider.left
                    top: slider.bottom
                }
            }
            FluText{
                text: formatDuration(player.duration - player.position)
                anchors{
                    right: slider.right
                    top: slider.bottom
                }
            }
            FluMenu{
                id:speedmenu
                width: 68
                property int currIndex: 4
                Repeater{
                    model: speedmodel
                    FluMenuItem{
                        text: model.text
                        onClicked: {
                            speedmenu.currIndex = index
                        }
                    }
                }
            }
            Row{
                height: 30
                anchors{
                    left: parent.left
                    bottom: parent.bottom
                    leftMargin: 15
                    bottomMargin: 5
                }

                FluIconButton{
                    iconSource: audiooutput.volume === 0 ? FluentIcons.Mute : FluentIcons.Volume
                    width: 30
                    height: 30
                    iconSize: 16
                    verticalPadding: 0
                    horizontalPadding: 0
                    onClicked: {
                        if(audiooutput.volume !== 0){
                            audiooutput.volume = 0
                        }else{
                            audiooutput.volume = d.currVolume
                        }
                    }
                }
                FluSlider{
                    from: 0
                    to:100
                    value: audiooutput.volume * 100
                    width: 100
                    text: Math.floor(value)
                    onValueChanged: {
                        audiooutput.volume = value / 100
                    }
                }
            }
            RowLayout {
                height: 30
                anchors{
                    right: parent.right
                    bottom: parent.bottom
                    rightMargin: 10
                    bottomMargin: 5
                }
                spacing: 38
                Layout.alignment: Qt.AlignHCenter

                FluToggleSwitch {
                    id: loopswitch
                    text: "循环播放"
                    Layout.alignment: Qt.AlignHCenter
                    textRight: false
                }

                FluTextButton{
                    id:speedbutton
                    text: speedmenu.currIndex === 4 ? "倍速" : speedmodel.get(speedmenu.currIndex).text
                    normalColor: FluTheme.fontPrimaryColor
                    onClicked: {
                        var pos = mapToItem(background,0,0)
                        speedmenu.x = pos.x - (68-speedbutton.width)/2
                        speedmenu.y =  pos.y - (speedmenu.count * 36)
                        speedmenu.open()
                    }
                    Layout.alignment: Qt.AlignHCenter
                }
            }
        }
    }

    Action {
        shortcut: "left"
        onTriggered: {
            if(d.flag)
                player.setPosition(Math.max(0,player.position - 5000))
        }
    }

    Action {
        shortcut: "right"
        onTriggered: {
            if(d.flag)
                player.setPosition(Math.min(player.duration,player.position + 5000))
        }
    }

    Action {
        shortcut: "space"
        onTriggered: {
            if(player.playing){
                player.pause()
            }else{
                player.play()
            }
        }
    }

    function formatDuration(duration) {
        const seconds = Math.floor(duration / 1000);
        const hours = Math.floor(seconds / 3600);
        const minutes = Math.floor((seconds % 3600) / 60);
        const remainingSeconds = seconds % 60;
        return `${pad(hours)}:${pad(minutes)}:${pad(remainingSeconds)}`;
    }

    function pad(value) {
        return value.toString().padStart(2, '0');
    }

    function hidecontrol() {
        if(!speedmenu.visible)
            control.opacity = 0
    }

    function play() {
        player.play()
    }

    function pause() {
        player.pause()
    }

    function stop() {
        player.stop()
    }
}
