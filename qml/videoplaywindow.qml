import QtQuick
import QtQuick.Controls
import FluentUI
import Qt.labs.settings

FluWindow {
    id: window
    width: 1200
    height: 800
    minimumWidth: 500
    minimumHeight: 400
    title:"播放视频"
    onInitArgument:
        (argument)=>{
            player.source = argument.videourl
            if(argument.ismoved === 1) {
                var status = argument.status
                player.play()
                player.pause()
                if(status.playing)
                    player.play()
            }
            else {
                player.play()
                if(!playsettings.autoplay)
                    player.pause()
                player.seek(0)
            }
        }
    MMediaPlayer {
        id: player
        width: window.width
        height: window.height
        loopplay: playsettings.loopplay
        autoplay: playsettings.autoplay
        disautoinit: true
    }
    Settings {
        id: playsettings
        property bool loopplay: true
        property bool autoplay: true
    }
}
