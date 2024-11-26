import QtQuick
import QtQuick.Window
import QtQuick.Layouts

import MaxScreen

Window
{
    id: mainWindow

    visible: true
    visibility: "FullScreen"

    function toogleFullScreen(){

        if(5 == mainWindow.visibility)
            mainWindow.visibility = "Maximized";
        else
            mainWindow.visibility = "FullScreen";
    }

    function sendWindowSize(){

        MaxScreen.setWindowSize(mainWindow.width, mainWindow.height)
    }

    onWidthChanged: mainWindow.sendWindowSize();
    onHeightChanged: mainWindow.sendWindowSize();

    ColumnLayout
    {
        anchors.fill: parent
        spacing: 0

        StackLayout
        {
            currentIndex: MaxScreen.stackId

            WaitScreen
            {

            }

            ImageDisplay
            {

            }
        }
    }

    MultiPointTouchArea
    {
        anchors.fill: parent
        onTouchUpdated: (touchPoints) => MaxScreen.touchPointsUpdated(touchPoints);

        focus: true
        Keys.onSpacePressed: mainWindow.toogleFullScreen()
    }

    Component.onCompleted:
    {
        mainWindow.sendWindowSize()
    }
}
