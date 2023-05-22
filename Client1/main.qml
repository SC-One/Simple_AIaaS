import QtQuick 2.0
import QtQuick.Window 2.0
import AIaaS.Simple.Client 1.0
import QtQuick.Controls 1.4

Window {
    id: window
    width: 640
    height: 480
    visible: true
    title: qsTr("Client1")

    TabView {
        anchors.fill: parent
        Tab {
            title: "Camera"
            CameraHandler {
                anchors.fill: parent
                backend: mainController
            }
        }
        Tab {
            title: "DetectedImages"
            ReceivedImages {
                anchors.fill: parent
            }
        }
    }
}
