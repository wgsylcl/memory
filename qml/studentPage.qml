import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import FluentUI 1.0
import studenthelper 1.0

FluContentPage {
    id: root
    title: qsTr("我和我的同学们")
    property var datas: []
    FluText {
        id: fronttext
        text: qsTr("三年同窗，共沐一片阳光；一千个白昼，谱写了多少友谊的篇章？愿逝去的岁月，都化作美好的回忆，永留在心房。")
    }

    FluTableView {
        id: maintable
        width: parent.width
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

    Component {
        id: lookaction
        Item {
            FluFilledButton {
                anchors.centerIn: parent
                text: qsTr("看看ta的资料卡片")
                onClicked: {
                    var stu = maintable.getRow(row)
                    FluRouter.navigate("/profile",{name:stu.name,id:stu.id,key:row,profile:stu.profile,sign:stu.sign,picpaths:stu.picpaths,reviews:stu.reviews})
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
                text: maintable.getRow(row).birthdaydata
                onClicked: {
                    changebirthdaywindow.launch({name:maintable.getRow(row).name})
                }
                FluTooltip {
                    visible: parent.hovered
                    delay: 1000
                    text: "点我以修改ta的生日"
                }
            }
        }
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
                id: stu["id"],
                name: stu["name"],
                birthdaydata: stu["birthday"],
                birthday: maintable.customItem(birthdayaction),
                action: maintable.customItem(lookaction),
                profile: stu["profile"],
                sign: stu["sign"],
                picpaths: reader.getpicpaths(name),
                reviews: reader.getreviews(name)
            })
        }

        root.datas = data
        maintable.dataSource = root.datas
    }
}
