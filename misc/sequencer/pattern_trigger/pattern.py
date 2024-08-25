#

from PySide6.QtGui import QPainter, QPalette, QPen, QBrush
from PySide6.QtCore import QSize, Qt


class Pattern:

    scale = 15
    Role = Qt.UserRole + 10

    def __init__(self):

        super().__init__()

        self.loop = True
        self.length = 16
        self.values = [0, 0]

    def toDict(self):

        return {'loop': self.loop, 'length': self.length, 'values': self.values}

    def copy(self):

        pattern = Pattern()

        pattern.loop = self.loop
        pattern.length = self.length
        pattern.values = list()

        for value in self.values:
            pattern.values.append(value)

        return pattern

    def paste(self, pattern):

        self.loop = pattern.loop
        self.length = pattern.length
        self.values = list()

        for value in pattern.values:
            self.values.append(value)

    @staticmethod
    def fromDict(dictIn):

        pattern = Pattern()

        if 'loop' in dictIn:
            pattern.loop = dictIn['loop']
        if 'length' in dictIn:
            pattern.length = dictIn['length']
        if 'values' in dictIn:
            pattern.values = dictIn['values']

        return pattern

    def setLength(self, length):

        byteCount = 0
        while (8 * byteCount) < length:
            byteCount += 1

        while byteCount > len(self.values):
            self.values.append(0)

        while byteCount < len(self.values):
            self.values.pop()

        for index in range(8 * byteCount):
            if index < length:
                continue
            if self.readBit(index):
                self.toggleBit(index)

        self.length = length

    def readBit(self, index):

        if index < 0 or index >= self.length:
            return False

        bit = index % 8
        mask = (1 << bit)

        offset = int((index - bit) / 8)
        byte = self.values[offset]

        test = (mask & byte) != 0

        return test

    def toggleBit(self, index):

        if index < 0 or index >= self.length:
            return

        bit = index % 8
        mask = (1 << bit)

        offset = int((index - bit) / 8)
        byte = self.values[offset]

        add = (mask & byte) == 0

        if add:
            self.values[offset] = (byte | mask)
        else:
            invMask = ~mask
            self.values[offset] = (byte & invMask)

    def sizeHint(self):

        return Pattern.scale * QSize(self.length, 1)

    def paint(self, painter, rect, palette, isEditable, isHighlight):

        painter.save()

        painter.setRenderHint(QPainter.Antialiasing, True)

        if isEditable:
            painter.setPen(QPen(Qt.cyan))
        elif isHighlight:
            painter.setPen(palette.color(QPalette.HighlightedText))
        else:
            painter.setPen(palette.color(QPalette.WindowText))

        y_offset = (rect.height() - Pattern.scale) / 2
        painter.translate(rect.x(), rect.y() + y_offset)

        p = 0.1 * Pattern.scale
        d = 0.8 * Pattern.scale

        for index in range(self.length):
            if self.readBit(index):
                if isEditable:
                    painter.setBrush(QBrush(Qt.cyan))
                elif isHighlight:
                    painter.setBrush(palette.highlightedText())
                else:
                    painter.setBrush(palette.text())
            else:
                painter.setBrush(Qt.NoBrush)

            painter.drawEllipse(p, p, d, d)
            painter.translate(Pattern.scale, 0.0)
            if 3 == index % 4:
                painter.translate(Pattern.scale, 0.0)

        painter.restore()
