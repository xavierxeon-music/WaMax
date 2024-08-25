from PySide6.QtWidgets import QStyledItemDelegate, QWidget

from PySide6.QtWidgets import QStyle
from PySide6.QtCore import Signal
from PySide6.QtGui import QPainter

from .loom import Loom
from .pattern import Pattern


class PatternEditor(QWidget):

    editing_finished = Signal()

    def __init__(self, parent=None):

        super().__init__(parent)
        self.pattern = Pattern()

        self.setMouseTracking(True)
        self.setAutoFillBackground(True)

    def sizeHint(self):

        return self.pattern.sizeHint()

    def paintEvent(self, event):

        with QPainter(self) as painter:
            painter.fillRect(self.rect(), self.palette().highlight())
            self.pattern.paint(painter, self.rect(), self.palette(), True, True)

    def mousePressEvent(self, event):

        index = self._index(event)
        if None == index:
            return

        self.pattern.toggleBit(index)
        self.update()

        Loom.the.beatModified.emit()

    def mouseDoubleClickEvent(self, event):

        index = self._index(event)
        if None == index:
            self.editing_finished.emit()

    def _index(self, event):

        index = int(event.x() / Pattern.scale)
        if 4 == index % 5:
            return None

        realIndex = index
        while index > 4:
            realIndex -= 1
            index -= 5

        return realIndex


class PatternDelegate(QStyledItemDelegate):

    def __init__(self):

        super().__init__()

    def sizeHint(self, option, index):

        if index.column() != 3:
            return QStyledItemDelegate.sizeHint(self, option, index)

        pattern = index.data(Pattern.Role)
        return pattern.sizeHint()

    def paint(self, painter, option, index):

        if index.column() != 3:
            QStyledItemDelegate.paint(self, painter, option, index)
            return

        pattern = index.data(Pattern.Role)

        isHighlight = False
        if option.state & QStyle.State_Selected:
            painter.fillRect(option.rect, option.palette.highlight())
            isHighlight = True

        pattern.paint(painter, option.rect, option.palette, False, isHighlight)

    def createEditor(self, parent, option, index):

        if index.column() != 3:
            return QStyledItemDelegate.createEditor(self, parent, option, index)

        editor = PatternEditor(parent)
        editor.editing_finished.connect(self.commit_and_close_editor)
        return editor

    def setEditorData(self, editor, index):

        if index.column() != 3:
            QStyledItemDelegate.setEditorData(self, editor, index)
            return

        editor.pattern = index.data(Pattern.Role)

    def setModelData(self, editor, model, index):

        if index.column() != 3:
            QStyledItemDelegate.setModelData(self, editor, model, index)
            return

        model.setData(index, editor.pattern, Pattern.Role)

    def commit_and_close_editor(self):

        editor = self.sender()

        self.commitData.emit(editor)
        self.closeEditor.emit(editor, QStyledItemDelegate.NoHint)
