from PySide6.QtGui import QStandardItemModel

from PySide6.QtCore import Qt
from PySide6.QtGui import QStandardItem

from .notemodel import Note
from .timeline import TimeLine


class VelocityModel(QStandardItemModel):

    def __init__(self):

        super().__init__()
        TimeLine.the.currentSequenceChanged.connect(self.create)
        TimeLine.the.sequenceUpdated.connect(self.create)

    def create(self):

        sequence = TimeLine.the.currentSequence()
        if not sequence:
            self.clear()
            return

        self.beginResetModel()
        self.clear()

        activeEventList = sequence.compileActiveEventList()
        for col in range(sequence.length):

            timePoint = activeEventList[col]
            for rowNumber, value in timePoint.items():
                value1Name = rowNumber
                rowNumber = int(rowNumber)
                if TimeLine.the.asNotes:
                    noteValue = rowNumber
                    noteIndex = noteValue % 12
                    octave = int((noteValue - noteIndex) / 12)

                    value1Name = Note.noteNames[noteIndex]
                    value1Name += str(octave)

                timeItem = QStandardItem(str(col))
                timeItem.setEditable(False)

                value1Item = QStandardItem(value1Name)
                value1Item.setData(rowNumber)
                value1Item.setEditable(False)

                value2Item = QStandardItem(str(value))

                self.invisibleRootItem().appendRow([timeItem, value1Item, value2Item])

        self.setHorizontalHeaderLabels(['time', 'value1', 'value2'])
        self.endResetModel()

    def setData(self, index, value, role):

        sequence = TimeLine.the.currentSequence()

        result = super().setData(index, value, role)
        if Qt.EditRole == role and 2 == index.column():
            row = index.row()

            timeItem = self.invisibleRootItem().child(row, 0)
            timePoint = int(timeItem.text())

            value1Item = self.invisibleRootItem().child(row, 1)
            rowNumber = value1Item.data()

            sequence.setValue(timePoint, rowNumber, value)

        return result
