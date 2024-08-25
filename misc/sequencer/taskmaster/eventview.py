from _common import DataView
from PySide6.QtCore import QSortFilterProxyModel

from PySide6.QtWidgets import QComboBox

from _common import Icon

from .calendar import Calender
from .eventmodel import EventModel, EventTagModel


class EventFilterModel(QSortFilterProxyModel):

   def __init__(self, timeModel):

      super().__init__()
      self._timeModel = timeModel
      self.setSourceModel(timeModel)

      Calender.the.tagsChanged.connect(self.invalidateFilter)

   def filterAcceptsRow(self, sourceRow, sourceParent):

      index = self.sourceModel().index(sourceRow, 0, sourceParent)

      tagItem = self._timeModel.itemFromIndex(index)
      tag = tagItem.text()

      if not tag in Calender.the.tagDict:
         return False

      active = Calender.the.tagDict[tag]
      return active


class EventView(DataView):

   def __init__(self):

      super().__init__(EventModel())

      self._proxyModel = EventFilterModel(self._model)
      self.setModel(self._proxyModel)

      Calender.the.timePointSelected.connect(self._checkTag)

   def addControls(self, mainWindow):

      self.tagSelectCombo = QComboBox()
      self.tagSelectCombo.setModel(EventTagModel())
      self.tagSelectCombo.textActivated.connect(self._tagChanged)

      eventBar = mainWindow.addToolBar('Event')
      eventBar.setObjectName('Event')
      eventBar.setMovable(False)

      eventBar.addWidget(self.tagSelectCombo)
      self.addAction = eventBar.addAction(Icon.common('new'), 'Add Event', self._add)
      self.addAction.setEnabled(False)

      eventBar.addAction(Icon.common('delete'), 'Remove Event', self._remove)

      eventBar.addSeparator()

   def _tagChanged(self, text):

      if not text in self._model.current:
         self.addAction.setEnabled(True)
      else:
         self.addAction.setEnabled(False)

   def _checkTag(self):

      tag = self.tagSelectCombo.currentText()
      self._tagChanged(tag)

   def _add(self):

      tag = self.tagSelectCombo.currentText()
      Calender.the.addEvent(tag)

   def _remove(self):

      tag = self.selectedText()
      Calender.the.removeEvent(tag)
