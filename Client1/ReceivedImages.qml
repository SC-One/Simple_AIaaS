import QtQuick 2.0
import QtQuick.Layouts 1.0

Item {
    FullImage {
        id:fullScreen
        visible:false;
    }

    Flow {
        anchors.fill: parent;
        Repeater {
            id: repeater
            model: detectedImagesModel
            delegate: Item {
                width: 100
                height: 100
                Image {
                    source: "image://myImageProvider/" + id
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        fullScreen.source  = "image://myImageProvider/" + id
                        fullScreen.visible = true;
                    }
                }
            }
        }
    }
}
