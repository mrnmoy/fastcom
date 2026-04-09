import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Universal
import SerialManager

Window {
    id: window
    width: 660
    height: 400
    visible: true
    title: qsTr("Hello World")
    color: "#11111b"

    Universal.theme: Universal.Dark
    Universal.background: "#1e1e2e"
    Universal.foreground: "#cdd6f4"
    Universal.accent: "#b4befe"

    SerialManager {
        id: serialManager

        onStatusChanged: status => output.append({
                str: status ? "Port opened" : "Port closed",
                time: getTime(),
                type: "system"
            })
        onReceived: data => output.get(output.count - 1).str += data
        onReceivedLn: output.append({
            str: "",
            time: getTime(),
            type: "read"
        })
        onError: err => output.append({
                str: err,
                time: getTime(),
                type: "system",
                subType: "error"
            })
    }

    Component {
        id: closedActions

        Button {
            id: openBtn
            topPadding: 8
            leftPadding: 16
            rightPadding: 16
            bottomPadding: 8
            font.pixelSize: 16
            Universal.foreground: "#11111b"
            text: "Start"
            onClicked: {
                serialManager.open(portName.currentValue, baudRate.currentValue, openMode.currentValue);
            }
            background: Rectangle {
                border.width: 2
                radius: 8
                color: "#a6e3a1"
            }
        }
    }

    Component {
        id: openedActions

        Button {
            id: closeBtn
            topPadding: 8
            leftPadding: 16
            rightPadding: 16
            bottomPadding: 8
            font.pixelSize: 16
            Universal.foreground: "#11111b"
            text: "Close"
            enabled: serialManager.isConnected
            onClicked: {
                serialManager.close();
            }
            background: Rectangle {
                border.width: 2
                radius: 8
                color: "#f38ba8"
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 8
        anchors.margins: 8

        FlexboxLayout {
            Layout.fillHeight: false
            justifyContent: FlexboxLayout.JustifySpaceBetween
            alignItems: FlexboxLayout.AlignCenter

            RowLayout {
                Text {
                    id: appName
                    text: "FastCom"
                    font.bold: true
                    font.pixelSize: 24
                    color: "#b4befe"
                    leftPadding: 8
                }

                Text {
                    id: state
                    text: serialManager.isConnected ? "Opened" : "Closed"
                    font.weight: 500
                    font.pixelSize: 20
                    color: serialManager.isConnected ? "#a6e3a1" : "#f38ba8"
                }
            }

            RowLayout {
                RowLayout {
                    enabled: !serialManager.isConnected

                    ComboBox {
                        id: portName
                        model: serialManager.availablePorts
                        spacing: 4
                        background: Rectangle {
                            radius: 8
                            color: "#1e1e2e"
                        }
                        contentItem: Text {
                            text: parent.displayText
                            font.pixelSize: 16
                            color: "#cdd6f4"
                            leftPadding: 8
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                    ComboBox {
                        id: baudRate
                        font.pixelSize: 16
                        model: serialManager.availableBaudRates
                        background: Rectangle {
                            radius: 8
                            color: "#1e1e2e"
                        }
                        contentItem: Text {
                            text: parent.displayText
                            font.pixelSize: 16
                            color: "#cdd6f4"
                            leftPadding: 8
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                    ComboBox {
                        id: openMode
                        font.pixelSize: 16
                        model: serialManager.openModes
                        background: Rectangle {
                            radius: 8
                            color: "#1e1e2e"
                        }
                        contentItem: Text {
                            text: parent.displayText
                            font.pixelSize: 16
                            color: "#cdd6f4"
                            leftPadding: 8
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }

                Loader {
                    sourceComponent: serialManager.isConnected ? openedActions : closedActions
                }
            }
        }

        Component {
            id: systemOutputComp

            FlexboxLayout {
                width: outputList.width
                justifyContent: FlexboxLayout.JustifyCenter

                TextField {
                    text: _str
                    font.pixelSize: 16
                    readOnly: true
                    wrapMode: width > parent.width / 1.25 ? TextInput.Wrap : TextInput.NoWrap
                    color: _subType == "error" ? "#f38ba8" : _subType == "warning" ? "#f9e2af" : "#cdd6f4"
                    background: Rectangle {
                        radius: 8
                        color: "#1e1e2e"
                    }
                }
            }
        }

        Component {
            id: writeOutputComp

            FlexboxLayout {
                width: outputList.width
                justifyContent: FlexboxLayout.JustifyEnd

                TextField {
                    id: writeOutputStr
                    text: _str
                    font.pixelSize: 16
                    readOnly: true
                    wrapMode: width > parent.width / 1.25 ? TextInput.Wrap : TextInput.NoWrap
                    color: "#cdd6f4"
                    background: Rectangle {
                        radius: 8
                        color: "#1e1e2e"
                    }
                }

                TextField {
                    id: writeOutputTime
                    text: _time
                    font.pixelSize: 12
                    readOnly: true
                    background: Rectangle {
                        color: "transparent"
                    }
                }
            }
        }

        Component {
            id: readOutputComp

            FlexboxLayout {
                width: outputList.width
                justifyContent: FlexboxLayout.JustifyStart

                TextField {
                    text: _str
                    font.pixelSize: 16
                    readOnly: true
                    wrapMode: width > parent.width / 1.25 ? TextInput.Wrap : TextInput.NoWrap
                    color: "#cdd6f4"
                    background: Rectangle {
                        radius: 8
                        color: "#1e1e2e"
                    }
                }

                TextField {
                    text: _time
                    font.pixelSize: 12
                    readOnly: true
                    background: Rectangle {
                        color: "transparent"
                    }
                }
            }
        }

        ListModel {
            id: output
            onCountChanged: outputList.positionViewAtEnd()
        }

        ListView {
            id: outputList
            model: output
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 8
            delegate: Loader {
                id: outputLoader
                property string _str: str
                property string _time: time
                property string _subType: subType
                sourceComponent: type == "system" ? systemOutputComp : type == "write" ? writeOutputComp : readOutputComp
            }
        }

        RowLayout {
            enabled: serialManager.isConnected && !serialManager.isReadOnly

            TextField {
                id: input
                implicitHeight: sendBtn.height
                Layout.fillWidth: true
                font.pixelSize: 16
                placeholderText: qsTr("Enter something")
                background: Rectangle {
                    border.width: 2
                    radius: 8
                    color: "#1e1e2e"
                }
            }

            Button {
                id: sendBtn
                topPadding: 8
                leftPadding: 16
                rightPadding: 16
                bottomPadding: 8
                font.pixelSize: 16
                Universal.foreground: "#11111b"
                text: "Send"
                enabled: input.text != ""
                onClicked: {
                    serialManager.send(input.text);
                    output.append({
                        str: input.text,
                        time: getTime(),
                        type: "write"
                    });
                    input.clear();
                }
                background: Rectangle {
                    border.width: 2
                    radius: 8
                    opacity: parent.enabled ? 1 : 0.5
                    color: "#89b4fa"
                }
            }
        }
    }

    Component.onCompleted: {
        output.append({
            str: "Application started",
            time: getTime(),
            type: "system",
            subType: "info"
        });
    }

    function getTime() {
        var now = new Date();
        return ("0" + now.getHours()).slice(-2) + ":" + ("0" + now.getMinutes()).slice(-2) + ":" + ("0" + now.getSeconds()).slice(-2);
    }
}
