from _common import DataView


from PySide6.QtWidgets import QLineEdit

from _common import Icon

from .loom import Loom
from .tagmodel import TagModel


class TagWidget(DataView):

   def __init__(self):

      super().__init__(TagModel.the)

      TagModel.the.modelReset.connect(self.modelUpdate)

   def modelUpdate(self):

      self.resizeColumnToContents(0)

   def addControls(self, mainWindow):

      tagToolBar = mainWindow.addToolBar('Tags')
      tagToolBar.setObjectName('Tags')
      tagToolBar.setMovable(False)

      self.tagEdit = QLineEdit()
      self.tagEdit.setStyleSheet("color: #ff0000")
      self.tagEdit.textChanged.connect(self._checkTag)
      self.tagEdit.returnPressed.connect(self._add)
      tagToolBar.addWidget(self.tagEdit)

      self.addAction = tagToolBar.addAction(Icon.app('new_tag'), 'Add Tag', self._add)
      self.addAction.setEnabled(False)

      tagToolBar.addAction(Icon.app('delete_tag'), 'Remove Tag', self._remove)

      tagToolBar.addSeparator()

   def _add(self):

      tag = self.tagEdit.text()

      if not tag in Loom.the.tags:
         Loom.the.tags[tag] = dict()
         Loom.the.tagsUpdated.emit()

      self._checkTag()

   def _remove(self):

      indices = self.selectionModel().selectedRows()
      if not indices:
         return

      row = indices[0].row()
      item = Loom.the.item(row, 0)

      tag = item.text()
      del Loom.the.tags[tag]

      Loom.the.tagsUpdated.emit()
      self._checkTag()

   def _checkTag(self):

      tag = self.tagEdit.text()

      if tag in Loom.the.tags:
         self.tagEdit.setStyleSheet("color: #ff0000")
         self.addAction.setEnabled(False)
      else:
         self.tagEdit.setStyleSheet("color: #000000")
         self.addAction.setEnabled(True)
