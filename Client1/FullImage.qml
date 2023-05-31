import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Window 2.0

Item {
    id: rootItem
    property alias source: image.source

    Rectangle {
        anchors.fill: parent
        color: "#000000"
        opacity: 0.5
        visible: rootItem.visible
    }

    Image {
        id: image
        anchors.centerIn: parent
        width: parent.width * 0.8
        height: parent.height * 0.8
        sourceSize.width: parent.width * 0.8
        sourceSize.height: parent.height * 0.8
        visible: rootItem.visible
    }

    MouseArea {
        anchors.fill: parent
        onClicked: rootItem.visible = false
        enabled: rootItem.visible
    }

    states: State {
        name: "visible"
        when: rootItem.visible
        PropertyChanges {
            target: rootItem
            opacity: 1
        }
        PropertyChanges {
            target: image
            opacity: 1
        }
    }
    transitions: Transition {
        NumberAnimation {
            properties: "opacity"
            duration: 300
        }
    }
}
