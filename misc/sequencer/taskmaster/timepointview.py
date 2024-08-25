from _common import DataView

from PySide6.QtCore import QSortFilterProxyModel, Qt
from PySide6.QtWidgets import QAbstractItemView, QLineEdit

from _common import TimePoint, Icon

from .timepointmodel import TimePointModel
from .calendar import Calender


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

   def modelUpdate(self):

      self.resizeColumnToContents(0)
      self.resizeColumnToContents(1)
      self.sortByColumn(0, Qt.AscendingOrder)

   def addControls(self, mainWindow):

      self.timePointEdit = QLineEdit()
      self.timePointEdit.setStyleSheet("color: #ff0000")
      self.timePointEdit.textChanged.connect(self._checkTimeLine)
      self.timePointEdit.returnPressed.connect(self._add)

      timePointBar = mainWindow.addToolBar('TimePoint')
      timePointBar.setObjectName('TimePoint')
      timePointBar.setMovable(False)

      timePointBar.addWidget(self.timePointEdit)
      self.addAction = timePointBar.addAction(Icon.app('new_timepoint'), 'Add Time Point', self._add)
      self.addAction.setEnabled(False)

      timePointBar.addAction(Icon.app('delete_timepoint'), 'Remove Time Point', self._remove)

      timePointBar.addSeparator()

   def itemClicked(self, timePoint):

      Calender.the.setCurrentTimePoint(timePoint)

   def _checkTimeLine(self):

      timePoint = self.timePointEdit.text()

      if Calender.the.isValidTimePoint(timePoint):
         self.timePointEdit.setStyleSheet("color: #000000")
         self.addAction.setEnabled(True)
      else:
         self.timePointEdit.setStyleSheet("color: #ff0000")
         self.addAction.setEnabled(False)

   def _add(self):

      timePoint = self.timePointEdit.text()
      if Calender.the.isValidTimePoint(timePoint):
         Calender.the.addTimePoint(timePoint)

      self._checkTimeLine()

   def _remove(self):

      timePoint = self.selectedText()
      if not timePoint:
         return

      Calender.the.removeTimePoint(timePoint)
      self._checkTimeLine()
