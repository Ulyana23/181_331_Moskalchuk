import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

ApplicationWindow {
    id: mainWindow
    width: 250
    height: 150
    visible: true
    title: qsTr("PassThrough")

    Page {
        id: mainPage
        anchors.fill: parent


        ColumnLayout {
            width: 180
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter

            Button {
                id: btnLoad
                text: "Загрузить драйвер"
                anchors.left: parent.left
                anchors.right: parent.right
                font.capitalization: Font.AllLowercase
                font.bold: false
                font.pixelSize: 12

                onClicked: {
                    Controller.loadDriver()
                }
            }

            Button {
                id: btnUnload
                anchors.top: btnLoad.bottom
                anchors.topMargin: 10
                text: "Выгрузить драйвер"
                font.capitalization: Font.AllLowercase
                font.bold: false
                anchors.left: parent.left
                anchors.right: parent.right
                font.pixelSize: 12

                onClicked: {
                    Controller.unloadDriver()
                }
            }
        }
    }
}


