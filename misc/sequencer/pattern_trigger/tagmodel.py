from PySide6.QtGui import QStandardItemModel

from PySide6.QtGui import QStandardItem

from .loom import Loom


class TagModel(QStandardItemModel):

   the = None

   def __init__(self):

      super().__init__()
      TagModel.the = self

      Loom.the.tagsUpdated.connect(self._create)

   def _create(self):

      self.beginResetModel()
      self.clear()

      keyList = list(Loom.the.tags.keys())
      keyList.sort()

      for key in keyList:
         nameItem = QStandardItem(key)
         self.invisibleRootItem().appendRow(nameItem)

      self.setHorizontalHeaderLabels(['name'])
      self.endResetModel()

   def setData(self, index, value, role):

      row = index.row()
      item = self.invisibleRootItem().child(row, 0)
      oldValue = item.text()

      if Loom.the.changeTag(value, oldValue):
         result = super().setData(index, value, role)
         Loom.the.tagsUpdated.emit()
         return result

      return False
