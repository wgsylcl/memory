import QtQuick
import QtQuick.Controls
import FluentUI

FluWindow {
    id: window
    width: 1200
    height: 800
    minimumWidth: 500
    minimumHeight: 400
    title:"播放视频"
    fitsAppBarWindows: visibility === Window.FullScreen ? true : false
    launchMode: FluWindowType.SingleInstance
    onInitArgument:
        (argument)=>{
            player.vediosource = argument.videourl
        }
    FluMediaPlayer {
        id: player
        width: window.width
        height: window.height
    }
    Component.onCompleted: player.reset()
    Action {
        shortcut: " "
        onTriggered: {
            if(player.isplaying)
                player.pause()
            else
                player.start()
        }
    }
}
