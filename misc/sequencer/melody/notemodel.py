from PySide6.QtGui import QStandardItemModel

from PySide6.QtGui import QStandardItem, QColor

from _common import Note, Scale

from .timeline import TimeLine


class NoteModel(QStandardItemModel):

   def __init__(self):

      super().__init__()
      TimeLine.the.loaded.connect(self.create)
      TimeLine.the.currentSequenceChanged.connect(self.create)
      TimeLine.the.sequenceLengthChanged.connect(self.create)
      TimeLine.the.sequenceUpdated.connect(self.updateColors)

   def create(self):

      self.beginResetModel()
      self.clear()

      sequence = TimeLine.the.currentSequence()

      scale = Scale.all[TimeLine.the.scaleIndex]

      rowHeaders = []
      for row in range(128):
         rowNumber = 127 - row
         if TimeLine.the.asNotes:
            rowIndex = rowNumber % 12
            octave = int((rowNumber - rowIndex) / 12) - 2
            rowNumber = Note.noteNames[rowIndex]
            rowNumber += str(octave)
            if scale.active[rowIndex]:
               rowNumber = '\u25a0 ' + rowNumber
            else:
               rowNumber = '\u25a1 ' + rowNumber
         else:
            rowNumber = str(rowNumber)
         rowHeaders.append(rowNumber)

         if not sequence:
            continue

         rowItems = []
         for col in range(sequence.length):
            rowItem = QStandardItem()
            rowItem.setEditable(False)
            rowItems.append(rowItem)

         self.invisibleRootItem().appendRow(rowItems)

      self.setVerticalHeaderLabels(rowHeaders)
      self.updateColors()
      self.endResetModel()

   def updateColors(self):

      sequence = TimeLine.the.currentSequence()
      if not sequence:
         return

      for row in range(128):
         rowNumber = 127 - row
         rowIndex = rowNumber % 12
         for col in range(sequence.length):
            rowItem = self.invisibleRootItem().child(row, col)
            if not rowItem:
               continue

            cell = sequence.eventList[col][rowNumber]

            if cell.active:
               rowItem.setBackground(QColor(200, 200, 255))
            elif 0 == rowIndex:
               rowItem.setBackground(QColor(240, 255, 255))
            elif TimeLine.the.asNotes and Note.blackKeys[rowIndex]:
               rowItem.setBackground(QColor(250, 250, 250))
            elif 0 == col % 4:
               rowItem.setBackground(QColor(255, 255, 240))
            else:
               rowItem.setBackground(QColor(255, 255, 255))

   def toggle(self, index):

      rowNumber = 127 - index.row()
      timePoint = index.column()

      sequence = TimeLine.the.currentSequence()
      sequence.toggle(timePoint, rowNumber)
