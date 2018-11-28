import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.3
//import org.example 1.0

Window {
    id: main_window
    objectName: "main_window"
    visible: true
    width: 640
    height: 480
    title: qsTr("Telecomunication Chart")

    signal getFilePath(string filePath)

 /*   ListModel {
        id: fruitModel

        ListElement {
            name: "Apple"
            cost: 2.45
        }
        ListElement {
            name: "Orange"
            cost: 3.25
        }
        ListElement {
            name: "Banana"
            cost: 1.95
        }
    }*/

    Component {
        id: highlightBar
        Rectangle {
            color: "grey"
            Text {
                 anchors.centerIn: parent
                 color: 'white'
             }
        }
    }

    ListView {
        id: values_list
        width: parent.width/2
        anchors.right: first_value_edit.left
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        model:ListWithValuesModel
            delegate:     Component {
                id: delegate_component
                Item {
                    id: wrapper
                    width: main_window.width/2
                    height: main_window.height/6
                        Column {
                        Text { text: model.time + "     " + model.averageAmplitude }
                    }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: wrapper.ListView.view.currentIndex = index
                        }
                }
            }
            highlight: highlightBar
    }


    Label {
        id: first_value_label
        width: parent.width/4
        text: "A"
        font.pixelSize: 15
        font.italic: true
        color: "steelblue"
        anchors.top: parent.top
        anchors.right: second_value_label.left
        horizontalAlignment: TextEdit.AlignHCenter
        verticalAlignment: TextEdit.AlignVCenter
    }

    TextEdit {
        id: first_value_edit
        width: parent.width/4
        text: "10"
        font.family: "Helvetica"
        font.pointSize: 14
        focus: true
        anchors.right: second_value_edit.left
        anchors.top: first_value_label.bottom
        horizontalAlignment: TextEdit.AlignHCenter
        verticalAlignment: TextEdit.AlignVCenter
    }

    Label {
        id: second_value_label
        width: parent.width/4
        text: "B"
        font.pixelSize: 15
        font.italic: true
        color: "steelblue"
        anchors.top: parent.top
        anchors.right: parent.right
        horizontalAlignment: TextEdit.AlignHCenter
        verticalAlignment: TextEdit.AlignVCenter
    }

    TextEdit {
        id: second_value_edit
        width: parent.width/4
        text: "10"
        font.family: "Helvetica"
        font.pointSize: 14
        focus: true
        anchors.right: parent.right
        anchors.top: second_value_label.bottom
        horizontalAlignment: TextEdit.AlignHCenter
        verticalAlignment: TextEdit.AlignVCenter
    }


    Button {
        id: add_point
        text: "Add Point"
        width: parent.width/2
        height: parent.height/6
        anchors.right: parent.right
        anchors.top: first_value_edit.bottom

    }

    FileDialog {
        id: fileDialog
//        visible: fileDialogVisible.checked
//        modality: fileDialogModal.checked ? Qt.WindowModal : Qt.NonModal
//        title: fileDialogSelectFolder.checked ? "Choose a folder" :
//            (fileDialogSelectMultiple.checked ? "Choose some files" : "Choose a file")
//        selectExisting: fileDialogSelectExisting.checked
//        selectMultiple: fileDialogSelectMultiple.checked
//        selectFolder: fileDialogSelectFolder.checked
        nameFilters: [ "Image files (*.png *.jpg)", "All files (*)" ]
        selectedNameFilter: "All files (*)"
//        sidebarVisible: fileDialogSidebarVisible.checked
        onAccepted: {
            console.log("Accepted: " + fileUrls)
            getFilePath(fileUrls);
 //           if (fileDialogOpenFiles.checked)
 //               for (var i = 0; i < fileUrls.length; ++i)
 //                   Qt.openUrlExternally(fileUrls[i])
        }
        onRejected: { console.log("Rejected") }
    }

    Button {
        id: add_points_from_file
        text: "Add points from file"
        width: parent.width/2
        height: parent.height/6
        anchors.top: add_point.bottom
        anchors.right: parent.right
        onClicked: fileDialog.open()
    }

    Button {
        id: generate_chart
        text: "Generate Chart"
        width: parent.width/2
        height: parent.height/6
        anchors.top: add_points_from_file.bottom
        anchors.right: parent.right
        onClicked: {
            var component = Qt.createComponent("chartWindow.qml")
            var window    = component.createObject(main_window)
            window.show()
        }
    }
}
