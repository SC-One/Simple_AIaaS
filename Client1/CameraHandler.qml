import QtQuick 2.0
import QtQuick.Controls 2.0
import QtMultimedia 5.0

Rectangle {
    id: rootItem
    width: 640
    height: 480
    property var backend: undefined
    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        text: qsTr("AIaaS will detect objects")
        color: "red"
    }

    Camera {
        id: camera
        captureMode: Camera.CaptureStillImage
        imageCapture {
            onImageCaptured: {
                if (undefined === rootItem.back)
                    return
                var image = preview.image
                var bytes = image.bits()
                var size = image.byteCount()
                var format = image.format()
                backend.setImage(bytes, size, format)
            }
        }
    }

    VideoOutput {
        source: camera
        anchors.fill: parent
        visible: true
    }

    Button {
        id: captureButton
        text: "Capture and Send to AIaaS"
        onClicked: {
            try {
                camera.imageCapture.capture()
            } catch (e) {
                console.log("EEEEEEE: ", e)
            }
        }
        y: parent.height * 0.95 - height
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
