import QtQuick
import QtQuick.Window
import QtQuick.Layouts

import ScreenServer

Item
{
    id: mainWindow

    visible: true

    function toogleFullScreen(){

        return;
        if(5 == mainWindow.visibility)
            mainWindow.visibility = "Maximized";
        else
            mainWindow.visibility = "FullScreen";
    }

    function sendWindowSize(){

        ScreenServer.setWindowSize(mainWindow.width, mainWindow.height)
    }

    onWidthChanged: mainWindow.sendWindowSize();
    onHeightChanged: mainWindow.sendWindowSize();

    ColumnLayout
    {
        anchors.fill: parent
        spacing: 0

        StackLayout
        {
            currentIndex: ScreenServer.stackId

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
        onTouchUpdated: (touchPoints) => ScreenServer.touchPointsUpdated(touchPoints);

        focus: true
        Keys.onSpacePressed: mainWindow.toogleFullScreen()
    }

    Component.onCompleted:
    {
        mainWindow.sendWindowSize()
    }
}
