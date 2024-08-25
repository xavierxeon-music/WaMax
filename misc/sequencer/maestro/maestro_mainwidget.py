from _common import SingeltonWindow

import os

from PySide6.QtCore import Qt
from PySide6.QtGui import QIcon, QKeySequence
from PySide6.QtWidgets import QWidget, QFileDialog, QCheckBox, QLineEdit

from _common import resource


class MaestroMainWidget(SingeltonWindow):

    def __init__(self):

        super().__init__()
        self.setWindowTitle('Maestro Editor [*]')

        self._currentFile = ''

    def loadFile(self, fileName):

        pass

    def saveFile(self, fileName):

        pass

    def load(self):

        loadLocation = QFileDialog.getOpenFileName(self, 'Maestro File', str(), '*.json')
        if not loadLocation:
            return

        fileName = loadLocation[0]
        self.loadFile(fileName)

    def save(self):

        saveLocation = QFileDialog.getSaveFileName(self, 'Maestro File', self._currentFile, '*.json')
        if not saveLocation:
            return

        fileName = saveLocation[0]
        self.saveFile(fileName)

    def newFile(self):

        pass
