from PySide6.QtGui import QStandardItemModel
from PySide6.QtCore import QSortFilterProxyModel

from PySide6.QtGui import QStandardItem
from PySide6.QtCore import Qt

from _common import TimePoint

from .loom import Loom
from .pattern import Pattern


class PulseSortModel(QSortFilterProxyModel):

   def __init__(self, pulseModel):

      super().__init__()
      self._pulseModel = pulseModel
      self.setSourceModel(pulseModel)

   def lessThan(self, leftIndex, rightIndex):

      leftTpData = self.sourceModel().data(leftIndex)
      rightTpData = self.sourceModel().data(rightIndex)

      leftTimePoint = TimePoint(leftTpData)
      rightTimePoint = TimePoint(rightTpData)

      if leftTimePoint < rightTimePoint:
         return True
      elif leftTimePoint == rightTimePoint:
         leftTagIndex = self.sourceModel().index(leftIndex.row(), 1)
         rightTagIndex = self.sourceModel().index(rightIndex.row(), 1)

         leftTagData = self.sourceModel().data(leftTagIndex)
         rightTagData = self.sourceModel().data(rightTagIndex)
         if leftTagData < rightTagData:
            return True

      return False


class PusleModel(QStandardItemModel):

   def __init__(self):

      super().__init__()

      Loom.the.loaded.connect(self._create)
      Loom.the.beatCountChange.connect(self._create)
      Loom.the.tagsUpdated.connect(self._create)

   def _create(self):

      self.beginResetModel()
      self.clear()

      # resort
      tpDict = dict()
      for tag, tpData in Loom.the.tags.items():
         for tp, pattern in tpData.items():
            if not tp in tpDict:
               tpDict[tp] = dict()
            tpDict[tp][tag] = pattern

      for tp, tagData in tpDict.items():
         for tag, pattern in tagData.items():

            tpItem = QStandardItem(tp)
            tpItem.setEditable(False)

            tagItem = QStandardItem(tag)
            tagItem.setEditable(False)

            length = str(pattern.length)
            lengthItem = QStandardItem(length)
            lengthItem.setCheckable(True)
            lengthItem.setCheckState(Qt.Checked if pattern.loop else Qt.Unchecked)

            patternItem = QStandardItem('***')
            patternItem.setData(pattern, Pattern.Role)

            self.invisibleRootItem().appendRow([tpItem, tagItem, lengthItem, patternItem])

      self.setHorizontalHeaderLabels(['bar.beat', 'tag', 'length', 'pattern'])
      self.endResetModel()

   def setData(self, index, value, role):

      column = index.column()
      if 2 != column:
         return super().setData(index, value, role)

      row = index.row()

      tpItem = self.invisibleRootItem().child(row, 0)
      timePoint = tpItem.text()

      tagItem = self.invisibleRootItem().child(row, 1)
      tag = tagItem.text()

      if Qt.EditRole == role:
         if not Loom.the.changeLength(tag, timePoint, value):
            return False
      elif Qt.CheckStateRole == role:
         loop = (value == 2)  # Qt::Checked
         Loom.the.changeLoop(tag, timePoint, loop)

      return super().setData(index, value, role)
