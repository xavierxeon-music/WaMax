
from PySide6.QtGui import QStandardItemModel

from PySide6.QtCore import Qt
from PySide6.QtGui import QStandardItem

from _common import TimePoint

from .timeline import TimeLine


class TimePointModel(QStandardItemModel):

    LengthRole = Qt.UserRole + 2

    def __init__(self):

        super().__init__()

        TimeLine.the.loaded.connect(self.create)

    def create(self):

        self.beginResetModel()
        self.clear()

        for timeStamp, sequence in TimeLine.the.sequences.items():

            timeStampItem = QStandardItem(timeStamp)

            lengthItem = QStandardItem(str(sequence.length))
            lengthItem.setData(sequence.length, TimePointModel.LengthRole)
            lengthItem.setCheckable(True)
            lengthItem.setCheckState(Qt.Checked if sequence.loop else Qt.Unchecked)

            self.invisibleRootItem().appendRow([timeStampItem, lengthItem])

        self.setHorizontalHeaderLabels(['bar.div', 'loop / length'])
        self.endResetModel()

    def setData(self, index, value, role):

        if Qt.EditRole == role:
            if 0 == index.column():
                if not self._setTimeStamp(index.row(), value):
                    return False
            elif 1 == index.column():
                if not self._setLength(index.row(), value):
                    return False
        elif Qt.CheckStateRole == role:
            loop = (value == 2)  # Qt::Checked
            if not self._setLoop(index.row(), loop):
                return False

        return super().setData(index, value, role)

    def isValidTimePoint(self, timePoint):

        tp = TimePoint(timePoint)
        if not tp.valid():
            return False

        if timePoint in TimeLine.the.sequences:
            return False

        return True

    def _setTimeStamp(self, row, value):

        if not self.isValidTimePoint(value):
            return False

        item = self.item(row, 0)
        timePoint = item.text()
        sequnece = TimeLine.the.sequences[timePoint]

        # print('TIMESTAMP', timePoint, value)
        return True

    def _setLength(self, row, value):

        try:
            length = int(value)
        except ValueError:
            return False

        if length < 1:
            return False

        item = self.item(row, 0)
        timePoint = item.text()

        # print('LENGTH', timePoint, length)

        sequnece = TimeLine.the.sequences[timePoint]
        sequnece.setLength(length)

        return True

    def _setLoop(self, row, loop):

        item = self.item(row, 0)
        timePoint = item.text()
        sequnece = TimeLine.the.sequences[timePoint]

        sequnece.loop = loop
        TimeLine.the.sequenceUpdated.emit()

        return True
