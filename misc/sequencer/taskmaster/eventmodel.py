from PySide6.QtGui import QStandardItemModel

from PySide6.QtGui import QStandardItem
from PySide6.QtCore import Qt

from .calendar import Calender


class EventTagModel(QStandardItemModel):

   def __init__(self):

      super().__init__()

      Calender.the.tagsChanged.connect(self._update)

   def _update(self):

      self.clear()

      tagNameList = list(Calender.the.tagDict.keys())
      tagNameList.sort()

      for tag in tagNameList:
         tagItem = QStandardItem(tag)
         tagItem.setEditable(False)
         self.invisibleRootItem().appendRow(tagItem)


class EventModel(QStandardItemModel):

   def __init__(self):

      super().__init__()
      self.current = dict()

      Calender.the.loaded.connect(self._clear)
      Calender.the.timePointSelected.connect(self._update)

   def _clear(self):

      self.clear()

      self.setHorizontalHeaderLabels(['name', 'value'])

   def _update(self, timePoint):

      self.clear()
      self.setHorizontalHeaderLabels(['name', 'value'])

      self.current = Calender.the.data[timePoint]

      tagNames = list(self.current.keys())
      tagNames.sort()

      for tag in tagNames:

         nameItem = QStandardItem(tag)
         nameItem.setEditable(False)

         value = self.current[tag]
         valueItem = QStandardItem(value)

         self.invisibleRootItem().appendRow([nameItem, valueItem])

   def setData(self, index, value, role):

      result = super().setData(index, value, role)

      column = index.column()

      if 1 == column and Qt.EditRole == role:
         row = index.row()
         tagItem = self.item(row, 0)
         tag = tagItem.text()
         Calender.the.editEvent(tag, value)

      return result
