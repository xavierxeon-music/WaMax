from _common import DataView

from PySide6.QtWidgets import QLineEdit
from PySide6.QtCore import QSortFilterProxyModel, Qt

from _common import TimePoint, Icon

from .timeline import TimeLine
from .timepointmodel import TimePointModel


class TimePointSortModel(QSortFilterProxyModel):

   def __init__(self, timeModel):

      super().__init__()
      self._timeModel = timeModel
      self.setSourceModel(timeModel)

   def lessThan(self, leftIndex, rightIndex):

      leftData = self.sourceModel().data(leftIndex)
      rightData = self.sourceModel().data(rightIndex)

      tpLeft = TimePoint(leftData)
      tpRight = TimePoint(rightData)

      return tpLeft < tpRight


class TimePointView(DataView):

   def __init__(self):

      super().__init__(TimePointModel())

      self._proxyModel = TimePointSortModel(self._model)
      self.setModel(self._proxyModel)

      self.setSortingEnabled(True)

      self._model.modelReset.connect(self.modelUpdate)

      self._clipboard = None

   def modelUpdate(self):

      self.resizeColumnToContents(0)
      self.resizeColumnToContents(1)
      self.sortByColumn(0, Qt.AscendingOrder)

   def addControls(self, mainWindow):

      self.timePointEdit = QLineEdit()
      self.timePointEdit.setStyleSheet("color: #ff0000")
      self.timePointEdit.textChanged.connect(self._checkTimeLine)
      self.timePointEdit.returnPressed.connect(self._add)

      editToolBar = mainWindow.addToolBar('TimePoint')
      editToolBar.setObjectName('TimePoint')
      editToolBar.setMovable(False)

      editToolBar.addWidget(self.timePointEdit)
      self.addAction = editToolBar.addAction(Icon.common('new'), 'Add TimePoint', self._add)
      self.addAction.setEnabled(False)

      editToolBar.addAction(Icon.common('delete'), 'Remove TimePoint', self._remove)

      editToolBar.addSeparator()

      editToolBar.addAction(Icon.common('copy'), 'Copy Sequence', self._copy)
      editToolBar.addAction(Icon.common('paste'), 'Paste Sequence', self._paste)
      editToolBar.addAction(Icon.common('clear'), 'Clear Sequence', self._clear)

      editToolBar.addSeparator()

   def _add(self):

      timePoint = self.timePointEdit.text()
      if self._model.isValidTimePoint(timePoint):
         TimeLine.the.add(timePoint)

      self._checkTimeLine()

   def _remove(self):

      timePoint = self._selectedTimePoint()
      if not timePoint:
         return

      TimeLine.the.remove(timePoint)
      self._checkTimeLine()

   def _copy(self):

      timePoint = self._selectedTimePoint()
      if not timePoint:
         return

      sequence = TimeLine.the.sequences[timePoint]
      self._clipboard = sequence.copy()

   def _paste(self):

      if not self._clipboard:
         return

      timePoint = self._selectedTimePoint()
      if not timePoint:
         return

      sequence = TimeLine.the.sequences[timePoint]
      sequence.paste(self._clipboard)

      self._clipboard = None

   def _clear(self):

      timePoint = self._selectedTimePoint()
      if not timePoint:
         return

      sequence = TimeLine.the.sequences[timePoint]
      sequence.clear()

   def _checkTimeLine(self):

      timePoint = self.timePointEdit.text()

      if self._model.isValidTimePoint(timePoint):
         self.timePointEdit.setStyleSheet("color: #000000")
         self.addAction.setEnabled(True)
      else:
         self.timePointEdit.setStyleSheet("color: #ff0000")
         self.addAction.setEnabled(False)

   def itemClicked(self, timePoint):

      TimeLine.the.setCurrent(timePoint)

   def _selectedTimePoint(self):

      timePoint = self.selectedText()
      return timePoint
