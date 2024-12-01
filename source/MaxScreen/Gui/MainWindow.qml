import QtQuick
import QtQuick.Window
import QtQuick.Layouts

import ScreenServer

Item
{
    id: mainWindow

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
                id: imageDisplay
            }
        }
    }

    MultiPointTouchArea
    {
        anchors.fill: parent
        onTouchUpdated: (touchPoints) => MaxScreen.touchPointsUpdated(touchPoints);

        focus: true
        Keys.onSpacePressed: MaxScreen.toogleFullScreen()
    }

    Component.onCompleted:
    {
        mainWindow.sendWindowSize()
        MaxScreen.setImageDisplay(imageDisplay)
    }
}
