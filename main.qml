import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import xyz.prinkov 1.0
ApplicationWindow {
    visible: true
    width: 640
    height: 250
    property bool showMsg: false
    property bool checkLatex: false
    property bool checkHtml: true
    title: qsTr("Методы оптимизации")


    Solver {
        id: solver
        onSendMsg: {
            progressText.text = msg
        }
        function start() {
            solver.run(variant.text)
        }
    }

    Rectangle {
           anchors.fill: parent
           Rectangle {
                id: inp
                width: 450
                height: 50
                color :"transparent"
                anchors.top: parent.top
                anchors.topMargin: 20
                anchors.horizontalCenter: parent.horizontalCenter
                Text {
                    id: labl
                    text: "Вариант: "
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 18
                }

                TextField {
                    id: variant
                    anchors.left: labl.right
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter

                    anchors.leftMargin: 10
                }
           }
           Text {
               id:progressText
               anchors.top: inp.bottom
               anchors.topMargin: 10
               anchors.horizontalCenter: parent.horizontalCenter
               text: "Прогресс"
           }
            ProgressBar {
                id: progressBar
                anchors.top: progressText.bottom
                anchors.topMargin: 5
                anchors.horizontalCenter: parent.horizontalCenter
                value: 0
            }
            WorkerScript {
                id: scripts

            }

            Row {
                id: outputFormat
                anchors.top: progressBar.bottom
                anchors.topMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                CheckBox {
                    id: htmlCheckedBox
                    text: "html отчет"
                    checked: checkHtml
                    onCheckedChanged: {
                        checkHtml = htmlCheckedBox.checked
                    }
                }
                CheckBox {
                    id: latexCheckBox
                    text: "pdf отчет"
                    checked: checkLatex
                    onCheckedChanged: {
                        if(!showMsg) {
                            popup.open()
                            showMsg = true
                        }
                        checkLatex = latexCheckBox.checked
                    }
                }
            }

            Popup {
                    id: popup
                    x: parent.width / 4
                    y: parent.height / 4
                    width: parent.width/2
                    height: parent.height/2
                    contentHeight: height
                    contentWidth: width
                    modal: true
                    contentItem:
                        Text {
                            id: lblMsg
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            wrapMode: Text.Wrap
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            text: "Для использования этой опции убедитесь, что у вас установлен Maple и LaTeX"
                        }
                    focus: true
                    enter: Transition {
                           NumberAnimation { property: "opacity"; from: 0.0; to: 1.0 }
                       }
                    exit: Transition {
                        NumberAnimation { property: "opacity"; from: 1.0; to: 0.0 }
                    }
            }

            Button {
                id: start
                anchors.top: outputFormat.bottom
                anchors.topMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Решить!"
                onClicked: {
                    solver.run(variant.text, checkHtml, checkLatex)
                }
           }
    }
}


