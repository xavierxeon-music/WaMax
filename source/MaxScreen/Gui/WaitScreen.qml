import QtQuick
import QtQuick.Layouts

import MaxScreen

Rectangle
{
    id: infoWindow
    color: MaxScreen.bgColor

    ColumnLayout
    {

        anchors.fill: parent

        Text
        {
            Layout.leftMargin:  50
            Layout.topMargin:  50

            text: "WAITING FOR"
            font.pointSize: 48
            color: "#ffffff"
        }
        Text
        {
            Layout.leftMargin:  50

            text: "CONNECTION"
            font.pointSize: 48
            color: "#ffffff"
        }

        RowLayout
        {
            GridLayout
            {
                columns: 2

                // row 0
                Text
                {
                    Layout.leftMargin:  50

                    text: "IP:"
                    font.pointSize: 24
                    color: "#ffffff"
                }
                Text
                {
                    text: MaxScreen.getHostName()
                    font.pointSize: 24
                    color: "#ffffff"
                }

                // row 2
                Text
                {
                    Layout.leftMargin:  50

                    text: "port:"
                    font.pointSize: 24
                    color: "#ffffff"
                }
                Text
                {
                    text: MaxScreen.getPort()
                    font.pointSize: 24
                    color: "#ffffff"
                }

                // row 3
                Text
                {
                    Layout.leftMargin:  50
                    Layout.topMargin:  50

                    text: "width:"
                    font.pointSize: 24
                    color: "#ffffff"
                }
                Text
                {
                    Layout.topMargin:  50

                    text:  mainWindow.width
                    font.pointSize: 24
                    color: "#ffffff"
                }

                // row 4
                Text
                {
                    Layout.leftMargin:  50

                    text: "height: "
                    font.pointSize: 24
                    color: "#ffffff"
                }
                Text
                {
                    text: infoWindow.height
                    font.pointSize: 24
                    color: "#ffffff"

                }
            }
            Item
            {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter

                Image
                {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter

                    source: "qrc:/MaxScreen.svg"
                    width: 128
                    height: 128
                    sourceSize.width: 128
                    sourceSize.height: 128
                }
            }
        }
    }

}
