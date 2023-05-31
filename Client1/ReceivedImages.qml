import QtQuick 2.0
import QtQuick.Layouts 1.0

Item {
    id: rt

    //    Timer {
    //        interval: 1000
    //        onTriggered: {
    //            console.log("DetectedModel: ", detectedImagesModel)
    //            console.log("rt: ", rt.width, rt.height)
    //        }
    //        repeat: true
    //        running: true
    //    }
    FullImage {
        id: fullScreen
        visible: false
        width: parent.width
        height: parent.height
    }

    Flow {
        visible: !fullScreen.visible
        anchors.fill: parent
        Repeater {
            id: repeater
            model: detectedImagesModel

            delegate: Item {
                width: 250
                height: 250
                Image {
                    source: "image://myImageProvider/" + index
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        fullScreen.source = "image://myImageProvider/" + index
                        fullScreen.visible = true
                    }
                }
            }
        }
    }
}
