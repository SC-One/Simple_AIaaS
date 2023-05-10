import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.VirtualKeyboard 2.0
import AIaaS.Simple.Client 1.0
import QtQuick.Controls 1.4

Window {
    id: window
    width: 640
    height: 480
    visible: true
    title: qsTr("Client1")

    TabView {
        anchors.fill: parent;
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


    InputPanel {
        id: inputPanel
        z: 99
        x: 0
        y: window.height
        width: window.width

        states: State {
            name: "visible"
            when: inputPanel.active
            PropertyChanges {
                target: inputPanel
                y: window.height - inputPanel.height
            }
        }

        transitions: Transition {
            from: ""
            to: "visible"
            reversible: true
            ParallelAnimation {
                NumberAnimation {
                    properties: "y"
                    duration: 250
                    easing.type: Easing.InOutQuad
                }
            }
        }
    }


}