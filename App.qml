import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import FluentUI

FluLauncher {
    id: app
    Component.onCompleted: {
        FluApp.init(app)
        FluApp.windowIcon = "qrc:/res/logo.ico"
        FluRouter.routes = {
            "/":"qrc:/qml/main.qml",
            "/profile":"qrc:/qml/profilewindow.qml"
        }
        FluRouter.navigate("/")
    }
}
