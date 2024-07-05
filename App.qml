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
            "/viewpicture":"qrc:/qml/pictureviewwindow.qml",
            "/playvideo":"qrc:/qml/videoplaywindow.qml",
            "/addpicture":"qrc:/qml/addpicturedialog.qml",
            "/addreview":"qrc:/qml/addreviewdialog.qml",
            "/changeprofile":"qrc:/qml/changeprofiledialog.qml",
            "/changebirthday":"qrc:/qml/changebirthdaywindow.qml",
            "/addactivitypicture":"qrc:/qml/addactivitypicturewindow.qml",
            "/addteacherpetphrase":"qrc:/qml/addteacherpetphrasewindow.qml",
            "/checkupload":"qrc:/qml/checkuploadwindow.qml",
            "/newactivity":"qrc:/qml/newactivitywindow.qml",
            "/crash":"qrc:/qml/crashwindow.qml"
        }
        if(MainTool.is_crashmode())
            FluRouter.navigate("/crash")
        else
            FluRouter.navigate("/")
    }

}
