import QtQuick 2.12
import QtQuick.Controls 2.12
import QtMultimedia 5.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2

Rectangle {
    id: rootItem
    width: 640
    height: 480
    property var backend: undefined
    onBackendChanged: {
        if(undefined !== rootItem.backend)
            rootItem.backend.setQCamera(camera)
    }

    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        text: qsTr("AIaaS will detect objects")
        color: "red"
    }

    Camera {
        id: camera
        captureMode: Camera.CaptureStillImage
    }

    VideoOutput {
        source: camera
        anchors.fill: parent
        visible: true
    }
    FileDialog {
        id: filedialog
        nameFilters: ["Image files (*.png *.jpg *.jpeg)"]
        onAccepted: {
            console.log("Accepted Files:" + fileUrls[0])
            if (fileUrls.length > 0) {
                backend.setImagePath(fileUrls[0])
            }
        }
    }

    RowLayout {
        y: parent.height * 0.95 - height
        width: parent.width
        height: 77
        Item {
            Layout.fillWidth: true
        }
        Button {
            id: browseFile
            text: "Browse..."
            onClicked: {
                filedialog.open()
            }
        }

        Button {
            id: captureButton
            text: "Capture and Send to AIaaS"
            onClicked: {
                try {
                    camera.imageCapture.capture()
                } catch (e) {
                    console.log("Error on image capturing: ", e)
                }
            }
        }
        Item {
            Layout.fillWidth: true
        }
    }
}
