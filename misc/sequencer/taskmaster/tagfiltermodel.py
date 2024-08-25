from PySide6.QtGui import QStandardItemModel

from PySide6.QtCore import Qt
from PySide6.QtGui import QStandardItem

from .calendar import Calender


class TagFilterModel(QStandardItemModel):

   def __init__(self):

      super().__init__()
      Calender.the.tagsChanged.connect(self._update)

   def _update(self):

      self.clear()
      self.setHorizontalHeaderLabels(['tag'])

      tagNameList = list(Calender.the.tagDict.keys())
      tagNameList.sort()

      for tagName in tagNameList:
         tagItem = QStandardItem(tagName)
         tagItem.setEditable(False)
         tagItem.setCheckable(True)
         self.invisibleRootItem().appendRow(tagItem)

         active = Calender.the.tagDict[tagName]
         tagItem.setCheckState(Qt.Checked if active else Qt.Unchecked)

   def setData(self, index, value, role):

      result = super().setData(index, value, role)

      column = index.column()
      if 0 == column and Qt.CheckStateRole == role:
         active = (value == 2)  # Qt::Checked

         row = index.row()
         tagItem = self.item(row, 0)
         tag = tagItem.text()

         Calender.the.checkTag(tag, active)

      return result
