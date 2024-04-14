import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import FluentUI 1.0

FluLauncher {
    id: app
    Component.onCompleted: {
        FluApp.init(app)
        FluApp.windowIcon = "qrc:/res/logo.ico"
        FluRouter.routes = {
            "/":"qrc:/qml/main.qml",
            "/profile":"qrc:/qml/profilewindow.qml",
            "/viewpicture":"qrc:/qml/pictureviewwindow.qml"
        }
        FluRouter.navigate("/")
    }
}
