from _common import SingeltonWindow


from PySide6.QtCore import Qt
from PySide6.QtGui import QKeySequence
from PySide6.QtWidgets import QFileDialog

from _common import Icon, StretcherWidget

from .calendar import Calender
from .tagfilterview import TagFilterView
from .timepointview import TimePointView
from .eventview import EventView


class TaskmasterMainWidget(SingeltonWindow):

   def __init__(self):

      super().__init__()

      self._calendar = Calender()
      self._calendar.timePointEdited.connect(self.dataModified)
      self._calendar.eventEdited.connect(self.dataModified)

      self._timePointView = TimePointView()
      self.addAndCreateDockWidget(self._timePointView, 'TimePoints', Qt.LeftDockWidgetArea)

      self._eventView = EventView()
      self.setCentralWidget(self._eventView)

      self._tagFilterView = TagFilterView()
      self.addAndCreateDockWidget(self._tagFilterView, 'Tags', Qt.RightDockWidgetArea)

      self._addControls()

   def loadFile(self, fileName):

      loaded = self._calendar.load(fileName)
      if not loaded:
         self._calendar.clear()

      self.updateWindowTitle(not loaded, fileName)

   def saveFile(self, fileName):

      self._calendar.save(fileName)

      self.updateWindowTitle(False, fileName)

   def load(self):

      loadLocation = QFileDialog.getOpenFileName(self, 'Taskmaster File', str(), '*.json')
      if not loadLocation:
         return

      fileName = loadLocation[0]
      self.loadFile(fileName)

   def save(self):

      saveLocation = QFileDialog.getSaveFileName(self, 'Taskmaster File', self._currentFile, '*.json')
      if not saveLocation:
         return

      fileName = saveLocation[0]
      self.saveFile(fileName)

   def newFile(self):

      self._calendar.clear()

      self._currentFile = ''
      self.setWindowModified(False)

   def _quickSave(self):

      if not self._currentFile:
         return

      self._calendar.save(self._currentFile)
      self.setWindowModified(False)

   def _addControls(self):

      fileToolBar = self.addToolBar('File')
      fileToolBar.setObjectName('File')
      fileToolBar.setMovable(False)

      fileToolBar.addAction(Icon.common('save'), 'Save', self._quickSave)
      fileToolBar.addSeparator()

      self._timePointView.addControls(self)
      self._eventView.addControls(self)
      self._tagFilterView.addControls(self)

      fileMenu = self.menuBar().addMenu('File')
      fileMenu.addAction('New', self.newFile)
      fileMenu.addAction('Load', self.load)
      fileMenu.addSeparator()

      fileMenu.addAction('Save', self.save)
      quickSaveAction = fileMenu.addAction(Icon.common('save'), 'QuickSave', self._quickSave)
      quickSaveAction.setShortcut(QKeySequence(QKeySequence.Save))
