from PySide6.QtWidgets import QTableView

from PySide6.QtCore import QTimer
from PySide6.QtWidgets import QAbstractItemView

from .notemodel import NoteModel


class NoteView(QTableView):

    def __init__(self):

        super().__init__()
        self._model = NoteModel()
        self.setModel(self._model)

        self.horizontalHeader().hide()
        self._model.modelReset.connect(self.modelUpdate)
        self.doubleClicked.connect(self._model.toggle)
        self.setSelectionMode(QAbstractItemView.NoSelection)

    def modelUpdate(self):

        colCount = self.model().columnCount()
        for col in range(colCount):
            self.setColumnWidth(col, 30)

        def center():
            index = self._model.index(127 - 48, 0)
            self.scrollTo(index)

        QTimer.singleShot(100, center)
