import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Window
import FluentUI
import studenthelper

FluContentPage {
    id: root
    title: qsTr("我和我的同学们")
    property var datas: []

    FluStatusLayout {
        anchors.fill: parent
        errorItem: com_loaderror
        statusMode: MainTool.haveprofilepicture() ? FluStatusLayoutType.Success : FluStatusLayoutType.Error

        Component {
            id:com_loaderror
            FluFrame {
                padding: 0
                border.width: 0
                radius: 0
                color: Qt.rgba(0,0,0,0)
                ColumnLayout{
                    anchors.centerIn: parent
                    spacing: 20

                    FluImage {
                        visible: true
                        source: "qrc:/res/erroregg.png"
                        Layout.fillHeight: true
                        fillMode: Image.PreserveAspectFit
                        Layout.preferredHeight: 120

                    }

                    FluText {
                        text: "啊噢……"
                        font: FluTextStyle.BodyStrong
                        Layout.alignment: Qt.AlignHCenter
                    }

                    FluText {
                        text: "貌似图文数据库还没下载~"
                        font: FluTextStyle.BodyStrong
                        Layout.alignment: Qt.AlignHCenter
                    }

                    FluText {
                        text: "点击导航栏中的\"数据库管理 -> 数据库更新\"下载图文数据库后再来看看吧！"
                        font: FluTextStyle.BodyStrong
                        Layout.alignment: Qt.AlignHCenter
                    }
                }
            }
        }

        FluText {
            id: fronttext
            text: qsTr("三年同窗，共沐一片阳光；一千个白昼，谱写了多少友谊的篇章？愿逝去的岁月，都化作美好的回忆，永留在心房。")
        }

        MTableView {
            id: maintable
            width: parent.width
            verticalHeaderVisible: false
            anchors{
                left: parent.left
                right: parent.right
                top: fronttext.bottom
                topMargin: 10
                bottom: parent.bottom
            }
            columnSource: [
                {
                    title: qsTr("学号"),
                    width: 100,
                    dataIndex: "id"
                },
                {
                    title: qsTr("名字"),
                    width: 100,
                    dataIndex: "name"
                },
                {
                    title: qsTr("生日"),
                    width: 200,
                    dataIndex: "birthday"
                },
                {
                    title: qsTr("操作"),
                    width: 200,
                    dataIndex: "action"
                },
                {
                    title: qsTr("自我介绍"),
                    width: 400,
                    dataIndex: "profile"
                }
            ]
        }

        StudentReader {
            id: reader
        }

        Component.onCompleted: {
            var data = []
            var names = []
            names = reader.readdata()
            for(var i=0;i<names.length;i++){
                var name = names[i]
                var stu = reader.at(name)
                data.push({
                              iddata: stu["id"],
                              id: maintable.customItem(idaction),
                              namedata: stu["name"],
                              name: maintable.customItem(nameaction),
                              birthdaydata: stu["birthday"],
                              birthday: maintable.customItem(birthdayaction),
                              action: maintable.customItem(lookaction),
                              profiledata: stu["profile"],
                              profile: maintable.customItem(profileaction),
                              sign: stu["sign"],
                              picpaths: reader.getpicpaths(name),
                              reviews: reader.getreviews(name)
                          })
            }

            root.datas = data
            maintable.dataSource = root.datas
        }
    }

    Component {
        id: lookaction
        Item {
            FluFilledButton {
                anchors.centerIn: parent
                text: qsTr("看看ta的资料卡片")
                onClicked: {
                    var stu = maintable.getRow(row)
                    FluRouter.navigate("/profile",{name:stu.namedata,id:stu.iddata,profile:stu.profiledata,sign:stu.sign,picpaths:stu.picpaths,reviews:stu.reviews})
                }
            }
        }
    }

    FluWindowResultLauncher {
        id: changebirthdaywindow
        path: "/changebirthday"
        onResult:
            (data) => {
                uploader.addstudentbirthday(data.name,data.birthday)
                showSuccess("提交成功，可在\"数据库管理 -> 上传图文\"处查看！")
            }
    }

    Component {
        id: birthdayaction
        Item {
            FluTextButton {
                anchors.centerIn: parent
                anchors.margins: 8
                text: maintable.getRow(row).birthdaydata
                onClicked: {
                    changebirthdaywindow.launch({name:maintable.getRow(row).namedata})
                }
                FluTooltip {
                    visible: parent.hovered
                    delay: 1000
                    text: "点我以修改ta的生日"
                }
            }
        }
    }

    Component {
        id: idaction
        Item {
            FluText {
                anchors.centerIn: parent
                text: maintable.getRow(row).iddata
            }
        }
    }


    Component {
        id: nameaction
        Item {
            FluText {
                anchors.centerIn: parent
                text: maintable.getRow(row).namedata
            }
        }
    }

    Component {
        id: profileaction
        Item {
            FluText {
                anchors.centerIn: parent
                text: maintable.getRow(row).profiledata
            }
        }
    }
}
