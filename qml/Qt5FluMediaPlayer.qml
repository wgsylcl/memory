import QtQuick 2.15
import QtQuick.Window 2.15
import FluentUI 1.0
import mediahelper 1.0
import QtMultimedia 5.15

FluContentPage {
    id: window
    property alias vediosource: player.source
    QtObject{
        id:d
        property bool flag: true
        property real currVolume: 1.0
    }
    ListModel{
        id:model_speed
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
    ListModel{
        id:model_effect
        ListElement{
            text:"无滤镜"
            frag:""
            vert:""
        }
        ListElement{
            text:"红色盲"
            frag:"qrc:/FluentPlayer/res/shader/effect_red.frag"
            vert:"qrc:/FluentPlayer/res/shader/effect_red.vert"
        }
        ListElement{
            text:"绿色盲"
            frag:"qrc:/FluentPlayer/res/shader/effect_green.frag"
            vert:"qrc:/FluentPlayer/res/shader/effect_green.vert"
        }
        ListElement{
            text:"蓝色盲"
            frag:"qrc:/FluentPlayer/res/shader/effect_blue.frag"
            vert:"qrc:/FluentPlayer/res/shader/effect_blue.vert"
        }
    }
    Rectangle{
        id:layout_background
        anchors.fill: parent
        color: FluColors.Black
    }
    MediaPlayerItem{
        id:player
        onPositionChanged: {
            if(d.flag){
                slider.value = position
            }
        }
        speed: model_speed.get(menu_speed.currIndex).value
        volume: d.currVolume
    }
    VideoOutput{
        id:video_output
        anchors.fill: parent
        source: player
    }
    Loader{
        property var target: video_output
        anchors.fill: video_output
        sourceComponent:menu_effect.currIndex === 0 ? undefined : com_effect
    }
    Component{
        id:com_effect
        Item{
            ShaderEffectSource {
                id: effect_source
                anchors.fill: parent
                visible: false
                sourceItem: target
            }
            ShaderEffect {
                anchors.fill: parent
                property variant src: effect_source
                vertexShader: model_effect.get(menu_effect.currIndex).vert
                fragmentShader: model_effect.get(menu_effect.currIndex).frag
            }
        }
    }
    Timer{
        id:timer_hide_delay
        interval: 1000
        onTriggered: {
            hideControlLayout()
        }
    }
    MouseArea{
        anchors.fill: parent
        hoverEnabled: true
        onCanceled: {
            hideControlLayout()
        }
        onExited: {
            hideControlLayout()
        }
        onPositionChanged:
            (mouse)=> {
                layout_control.opacity = true
                if(mouse.x > layout_control.x && mouse.y > layout_control.y && mouse.x <layout_control.x + layout_control.width && mouse.y<layout_control.y + layout_control.height){
                    timer_hide_delay.stop()
                }else{
                    timer_hide_delay.restart()
                }
            }
        FluFrame{
            id:layout_control
            width: parent.width-30
            height: 100
            opacity: 1
            visible: opacity
            anchors{
                horizontalCenter: parent.horizontalCenter
                bottom: parent.bottom
                bottomMargin: 38
            }
            MouseArea{
                anchors.fill: parent
            }
            Behavior on opacity {
                SequentialAnimation {
                    PauseAnimation { duration: 300 }
                    NumberAnimation { duration: 83 }
                }
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
                    var position = value
                    if(pressed){
                        d.flag = false
                    }else{
                        player.seek(position)
                        d.flag = true;
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
                text: formatDuration(player.duration)
                anchors{
                    right: slider.right
                    top: slider.bottom
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
            Row{
                height: 30
                anchors{
                    left: parent.left
                    bottom: parent.bottom
                    leftMargin: 15
                    bottomMargin: 5
                }
                FluIconButton{
                    iconSource: player.volume === 0 ? FluentIcons.Mute : FluentIcons.Volume
                    width: 30
                    height: 30
                    iconSize: 16
                    verticalPadding: 0
                    horizontalPadding: 0
                    onClicked: {
                        if(player.volume !== 0){
                            player.volume = 0
                        }else{
                            player.volume = d.currVolume
                        }
                    }
                }
                FluSlider{
                    from: 0
                    to:100
                    value: player.volume * 100
                    width: 100
                    onValueChanged: {
                        player.volume = value / 100
                    }
                }
            }

            Row{
                height: 30
                anchors{
                    right: parent.right
                    bottom: parent.bottom
                    rightMargin: 10
                    bottomMargin: 5
                }
                spacing: 8
                FluTextButton{
                    id:btn_effect
                    text: model_effect.get(menu_effect.currIndex).text
                    normalColor: FluTheme.fontPrimaryColor
                    onClicked: {
                        var pos = mapToItem(layout_background,0,0)
                        menu_effect.x = pos.x - (68-btn_effect.width)/2
                        menu_effect.y =  pos.y - (menu_effect.count * 36)
                        menu_effect.open()
                    }
                }
                FluTextButton{
                    id:btn_speed
                    text: menu_speed.currIndex === 3 ? "倍速" : model_speed.get(menu_speed.currIndex).text
                    normalColor: FluTheme.fontPrimaryColor
                    onClicked: {
                        var pos = mapToItem(layout_background,0,0)
                        menu_speed.x = pos.x - (68-btn_speed.width)/2
                        menu_speed.y =  pos.y - (menu_speed.count * 36)
                        menu_speed.open()
                    }
                }/*
                FluIconButton{
                    iconSource: d.isFullScreen ? FluentIcons.BackToWindow : FluentIcons.FullScreen
                    width: 30
                    height: 30
                    iconSize: 14
                    verticalPadding: 0
                    horizontalPadding: 0
                    onClicked: {
                        if(d.isFullScreen){
                            showNormal()
                        }else{
                            showFullScreen()
                        }
                    }
                }*/
            }
        }
    }
    FluMenu{
        id:menu_speed
        width: 68
        property int currIndex: 3
        Repeater{
            model: model_speed
            FluMenuItem{
                text: model.text
                onClicked: {
                    menu_speed.currIndex = index
                }
            }
        }
    }/*
    FluMenu{
        id:menu_effect
        width: 68
        property int currIndex: 0
        Repeater{
            model: model_effect
            FluMenuItem{
                text: model.text
                onClicked: {
                    menu_effect.currIndex = index
                }
            }
        }
    }*/
    function hideControlLayout(){
        if(menu_speed.visible || menu_effect.visible){
            return
        }
        layout_control.opacity = false
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
    function start() {
        if(!player.playing){
            player.pause()
        }
    }
    function pause() {
        if(player.playing){
            player.pause()
        }
    }
    function reset() {
        player.seek(0)
        menu_speed.currIndex = 3
    }
}