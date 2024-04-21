import QtQuick 2.15
import QtQuick.Controls 2.15
import FluentUI 1.0

FluContentPage {
    title: qsTr("回忆")
    property var activitynames: []
    // property var history: []
    property int index: 0
    property int mediasize: 0

    id: root

    ListModel {
        id: activitiymodel
    }

    Row {
        z: 1
        spacing: 20

        id: chooseactivityrow

        FluText {
            text: qsTr("选择要查看的活动")
        }

        FluComboBox {
            id: activitybox
            model: activitiymodel
            onCurrentIndexChanged: {
                mediasize = ActivityReader.readAllMedia(activitynames[currentIndex])
                for(var i=0;i<mediasize;i++) {
                    console.log(ActivityReader.getMediaPath(i))
                }

                stack.clear()
                index = 1
                stack.push(mainview)
            }
        }
    }

    StackView {
        id: stack
        z: 0
        anchors {
            left: parent.left
            right: parent.right
            top: chooseactivityrow.bottom
            bottom: parent.bottom
        }
        pushEnter: Transition {
            PropertyAnimation {
                property: "y"
                from: root.height
                to: 0
            }
        }
        pushExit: Transition {
            PropertyAnimation {
                property: "y"
                from: 0
                to: -root.height
            }
        }
        popEnter: Transition {
            PropertyAnimation {
                property: "y"
                from: -root.height
                to: 0
            }
        }
        popExit: Transition {
            PropertyAnimation {
                property: "y"
                from: 0
                to: root.height
            }
        }
    }
    Component {
        id: mainview
        Image {
            z: 0
            source: ""
            width: root.width
            height: parent.height
            fillMode: Image.PreserveAspectFit
            MouseArea {
                anchors.fill: parent
                onWheel: {
                    if(stack.busy) return;
                    if(wheel.angleDelta.y < 0) {
                        index++
                        stack.push(mainview)
                    }
                    else {
                        if(index > 1) {
                            index--
                            stack.pop()
                        }
                        else {
                            showWarning(qsTr("已经到顶端了噢~"))
                        }
                    }
                }
            }
            Component.onCompleted: {
                source = ActivityReader.getMediaPath((index-1)%mediasize)
            }
        }
    }

    Component.onCompleted: {
        activitynames = ActivityReader.getAllActivities()
        for(var i=0;i<activitynames.length;i++) {
            activitiymodel.append({text:activitynames[i]})
        }
    }
}
