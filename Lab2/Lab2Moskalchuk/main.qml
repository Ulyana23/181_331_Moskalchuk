import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import SortFilterProxyModel 0.2
import QtWebView 1.1

ApplicationWindow {
    id: mainWindow
    width: 500
    height: 320
    visible: true
    title: qsTr("Main window")

    Material.accent: "#899d58"

    Page {
        id: mainPage
        anchors.fill: parent

        ColumnLayout {
            width: 250
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter


            TextField {
                id: inputPass
                placeholderText: qsTr("Enter password")
                anchors.left: parent.left
                anchors.right: parent.right

                topPadding: 8
                bottomPadding: 8
                leftPadding: 10
                rightPadding: 10

                font.pixelSize: 12
                color: "#899d58"
                placeholderTextColor: "#808080"


                echoMode: TextInput.Password
                //color: "#ffffff"

                background: Rectangle {
                    width: parent.width
                    height: parent.height
                    color: "#ffffff"
                    radius: 5
                }

            }

            Button {
                id: btn
                text: "SING IN"
                //flat: true
                //highlighted: true
                anchors.left: parent.left
                anchors.right: parent.right

                Material.background: "#899d58"
                Material.foreground: "#ffffff"


                Layout.topMargin: 10

                font.bold: true
                font.pixelSize: 13

                onClicked: {
                    Controller.getPassword(inputPass.text)
                }

            }


            /*Button {
                id: btnFaceId
                text: "FACE ID"
                //flat: true
                anchors.left: parent.left
                anchors.right: parent.right

                Material.background: "#899d58"
                Material.foreground: "#ffffff"

                Layout.topMargin: 5

                font.bold: true
                font.pixelSize: 15
            }*/

        }

        background: Rectangle {
            anchors.fill: parent
            color: "#f4f0e5"
        }

        Dialog {
            id: dialogIncorrect
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2

            Label {
                text: "Incorrect password"
                color: "red"
            }
            modal: true
            standardButtons: Dialog.Ok
        }



        Connections {
            target: Controller

            function onSendAnswer(flag) {
                if(flag === true)
                {
                    passwordsPage.visible = true
                    mainPage.visible = false
                }
                else dialogIncorrect.open()
            }
        }

    }

    Page {
        id: passwordsPage
        anchors.fill: parent
        visible: false
        horizontalPadding: 10
        //verticalPadding: 10


        //        WebView {
        //            id: webview
        //         url: "https://stackoverflow.com/questions/12850723/how-can-i-display-the-icon-of-a-webview-in-qml"
        //        }

        //        Image {
        //            id: iconImage
        //            fillMode: Image.PreserveAspectFit
        //            source: webview.icon;
        //            height: 200
        //            width: 200
        //        }


        ListView {
            id: list
            model: proxyModel
            topMargin: 60

            anchors.fill: parent
            spacing: 10

            Rectangle {
                width: parent.width
                color: "#f4f0e5"
                height: 52

                TextField {
                    id: searchField
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.topMargin: 10
                    anchors.right: parent.right

                    //focus: true

                    placeholderText: "search..."
                    //height: 40

                    //padding: 8
                    topPadding: 10
                    bottomPadding: 10
                    leftPadding: 15
                    rightPadding: 15

                    font.pixelSize: 12
                    //visible: false

                    background: Rectangle {
                        width: parent.width
                        height: parent.height
                        color: "#ffffff"
                        radius: 5
                    }

                    onFocusChanged: {
                        searchField.focus = true
                    }


                }
            }



            delegate: Rectangle {
                id: delegateButton
                width: parent.width
                color: "#899d58"
                //flat: true
                height: 50
                radius: 5

                //anchors.horizontalCenter: parent.horizontalCenter

                RowLayout {
                    //anchors.horizontalCenter: parent.horizontalCenter
                    height: parent.height
                    width: parent.width
                    spacing: 5

                    //                    Item {
                    //                        width: 100
                    //                        Layout.leftMargin: 15

                    //                        Text {
                    //                            text: siteName
                    //                            font.pixelSize: 13
                    //                            font.bold: true
                    //                            color: "#ffffff"
                    //                        }
                    //                    }



                    Text {
                        Layout.preferredWidth: parent.width / 2.6
                        text: siteName

                        Layout.leftMargin: 15
                        font.pixelSize: 10
                        font.bold: true
                        color: "#ffffff"

                    }

                    TextField {
                        Layout.preferredWidth: parent.width / 4
                        font.pixelSize: 10
                        font.bold: true
                        color: "#ffffff"
                        text: "00000000"
                        echoMode: TextInput.Password

                        readOnly: true

                        background: Rectangle {
                            color: "#00000000"
                        }


                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                ToolTip.show("логин скопирован", 700)
                                Controller.copyData(siteName, "login");
                            }
                        }

                    }

                    TextField {
                        Layout.preferredWidth: parent.width / 4
                        font.pixelSize: 10
                        font.bold: true
                        color: "#ffffff"
                        text: "000000000000"
                        echoMode: TextInput.Password

                        readOnly: true

                        background: Rectangle {
                            color: "#00000000"
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                ToolTip.show("пароль скопирован", 700)
                                Controller.copyData(siteName, "password");
                            }
                        }

                    }
                }
            }

        }


        SortFilterProxyModel {
            id: proxyModel
            sourceModel: sitesModel

            filters: RegExpFilter {
                roleName: "siteName"
                pattern: searchField.text
            }
        }

        /*ColumnLayout {
            width: parent.width
            anchors.topMargin: 10

            Rectangle {
                anchors.fill: parent
                color: "#f4f0e5"
            }

            TextField {
                id: searchField
                anchors.left: parent.left
                anchors.right: parent.right
                //height: 40

                padding: 10
                text: "hi"
                //visible: false

                background: Rectangle {
                    width: parent.width
                    height: parent.height
                    color: "#ffffff"
                    radius: 5
                }
            }
        }*/




        //        ListModel {
        //            id: model

        //            ListElement {
        //                siteNume: "https://vk.com/"
        //                login: "vkLogin"
        //                password: "myPass"
        //            }

        //            ListElement {
        //                siteNume: "https://mail.google.com/"
        //                login: "mygooglecom"
        //                password: "GooglePass"
        //            }
        //        }



        background: Rectangle {
            anchors.fill: parent
            color: "#f4f0e5"
        }
    }
}

