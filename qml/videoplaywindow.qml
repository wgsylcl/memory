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
    launchMode: FluWindowType.SingleInstance
    onInitArgument:
        (argument)=>{
            player.source = argument.videourl
            player.play()
        }
    MMediaPlayer {
        id: player
        width: window.width
        height: window.height
    }
}
