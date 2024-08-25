from _common import SingeltonWindow

from PySide6.QtCore import Qt
from PySide6.QtGui import QKeySequence
from PySide6.QtWidgets import QFileDialog

from _common import Icon

from .loom import Loom
from .pulseview import PulseView
from .tagmodel import TagModel
from .tagwidget import TagWidget


class PatternTriggerMainWidget(SingeltonWindow):

   def __init__(self):

      super().__init__()

      self._loom = Loom()
      self._loom.beatModified.connect(self.dataModified)
      self._loom.beatCountChange.connect(self.dataModified)

      self._tagModel = TagModel()

      self._pulseVieww = PulseView()
      self.setCentralWidget(self._pulseVieww)

      self._tagWidget = TagWidget()
      self.addAndCreateDockWidget(self._tagWidget, 'Tags', Qt.LeftDockWidgetArea)

      self._addControls()

   def loadFile(self, fileName):

      loaded = self._loom.load(fileName)
      if not loaded:
         self._loom.clear()

      self.updateWindowTitle(not loaded, fileName)

   def saveFile(self, fileName):

      self._loom.save(fileName)

      self.updateWindowTitle(False, fileName)

   def load(self):

      loadLocation = QFileDialog.getOpenFileName(
          self, 'Pattern File', str(), '*.json')
      if not loadLocation:
         return

      fileName = loadLocation[0]
      self.loadFile(fileName)

   def save(self):

      saveLocation = QFileDialog.getSaveFileName(self, 'Pattern File', self._currentFile, '*.json')
      if not saveLocation:
         return

      fileName = saveLocation[0]
      self.saveFile(fileName)

   def newFile(self):

      self._loom.clear()

      self._currentFile = ''
      self.setWindowModified(False)

   def _quickSave(self):

      if not self._currentFile:
         return

      self._loom.save(self._currentFile)
      self.setWindowModified(False)

   def _addControls(self):

      fileToolBar = self.addToolBar('File')
      fileToolBar.setObjectName('File')
      fileToolBar.setMovable(False)

      self._tagWidget.addControls(self)
      self._pulseVieww.addControls(self)

      settingsToolBar = self.addToolBar('Settings')
      settingsToolBar.setObjectName('Settings')
      settingsToolBar.setMovable(False)

      fileMenu = self.menuBar().addMenu('File')
      fileMenu.addAction('New', self.newFile)
      fileMenu.addAction('Load', self.load)

      # actions
      fileToolBar.addAction(Icon.common('save'), 'Save', self._quickSave)
      fileToolBar.addSeparator()

      fileMenu.addSeparator()
      fileMenu.addAction('Save', self.save)
      quickSaveAction = fileMenu.addAction(Icon.common('save'), 'QuickSave', self._quickSave)
      quickSaveAction.setShortcut(QKeySequence(QKeySequence.Save))
