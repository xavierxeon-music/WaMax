from PySide6.QtWidgets import QTreeView

from PySide6.QtWidgets import QAbstractItemView
from PySide6.QtGui import QStandardItemModel


class DataView(QTreeView):

   def __init__(self, model):

      super().__init__()
      self._model = model
      self.setModel(model)

      self.setRootIsDecorated(False)
      self.setSelectionMode(QAbstractItemView.SingleSelection)

      self.clicked.connect(self._itemClicked)

   def _itemClicked(self, index):

      if not self._model:
         return

      row = index.row()

      item = self._model.item(row, 0)
      if not item:
         return
      text = item.text()

      self.itemClicked(text)

   def itemClicked(self, text):

      pass

   def selectedText(self):

      if not self._model:
         return None

      indices = self.selectionModel().selectedRows()
      if not indices:
         return None

      row = indices[0].row()

      item = self._model.item(row, 0)
      return item.text()
