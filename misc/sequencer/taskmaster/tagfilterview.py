from _common import DataView

from PySide6.QtWidgets import QLineEdit

from _common import Icon, StretcherWidget

from .calendar import Calender

from .tagfiltermodel import TagFilterModel


class TagFilterView(DataView):

   def __init__(self):

      super().__init__(TagFilterModel())

   def addControls(self, mainWindow):

      self.tagEdit = QLineEdit()
      self.tagEdit.setStyleSheet("color: #ff0000")
      self.tagEdit.textChanged.connect(self._checkTag)
      self.tagEdit.returnPressed.connect(self._add)

      tagBar = mainWindow.addToolBar('Tag')
      tagBar.setObjectName('Tag')
      tagBar.setMovable(False)

      tagBar.addWidget(StretcherWidget())
      tagBar.addSeparator()

      tagBar.addWidget(self.tagEdit)
      self.addAction = tagBar.addAction(Icon.app('new_tag'), 'Add Tag', self._add)
      self.addAction.setEnabled(False)

      tagBar.addAction(Icon.app('all_tags'), 'All Tags', Calender.the.checkAllTags)
      tagBar.addAction(Icon.app('no_tags'), 'No Tags', Calender.the.checkNoTags)

   def _add(self):

      tag = self.tagEdit.text()
      Calender.the.addTag(tag)

   def _checkTag(self):

      tag = self.tagEdit.text()
      if tag in Calender.the.tagDict:
         self.addAction.setEnabled(False)
         self.tagEdit.setStyleSheet("color: #ff0000")
      else:
         self.addAction.setEnabled(True)
         self.tagEdit.setStyleSheet("color: #000000")
