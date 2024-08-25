from PySide6.QtCore import QObject

import json
import os

from PySide6.QtCore import Signal

from _common import TimePoint

from .pattern import Pattern


class Loom(QObject):

    loaded = Signal()
    beatModified = Signal()
    beatCountChange = Signal()
    tagsUpdated = Signal()

    the = None

    def __init__(self):

        super().__init__()
        Loom.the = self

        self.tags = dict()

    def load(self, fileName):

        if not os.path.exists(fileName):
            return False

        try:
            with open(fileName, 'r') as infile:
                content = json.load(infile)
        except json.JSONDecodeError:
            return False

        self.tags = dict()
        for tag, tagData in content.items():
            self.tags[tag] = dict()
            for tp, patternDict in tagData.items():
                self.tags[tag][tp] = Pattern.fromDict(patternDict)

        self.loaded.emit()
        self.tagsUpdated.emit()
        self.beatCountChange.emit()

        return True

    def save(self, fileName):

        content = dict()
        for tag, tagData in self.tags.items():
            content[tag] = dict()
            for tp, pattern in tagData.items():
                content[tag][tp] = pattern.toDict()

        with open(fileName, 'w') as outfile:
            json.dump(content, outfile, indent=3)

    def clear(self):

        self.tags = dict()

        self.loaded.emit()
        self.tagsUpdated.emit()
        self.beatCountChange.emit()

    def available(self, tag, timePoint):

        tp = TimePoint(timePoint)
        if not tp.valid():
            return False

        if not tag in self.tags.keys():
            return False

        pulses = self.tags[tag]
        if not pulses:
            return True

        if timePoint in pulses:
            return False

        return True

    def add(self, tag, timePoint):

        if not self.tags[tag]:
            self.tags[tag] = dict()

        pulses = self.tags[tag]
        pulses[timePoint] = Pattern()
        self.beatCountChange.emit()

    def remove(self, tag, timePoint):

        pulses = self.tags[tag]
        del pulses[timePoint]

        self.beatCountChange.emit()

    def changeTag(self, tag, oldTag):

        if tag in self.tags:
            return False

        self.tags[tag] = self.tags[oldTag]
        del self.tags[oldTag]

        return True

    def changeLength(self, tag, timePoint, value):

        try:
            length = int(value)
            if length < 0:
                return False
        except ValueError:
            return False

        pulses = self.tags[tag]
        pattern = pulses[timePoint]

        pattern.setLength(length)
        self.beatModified.emit()

        return True

    def changeLoop(self, tag, timePoint, loop):

        pulses = self.tags[tag]
        pattern = pulses[timePoint]
        pattern.loop = loop

        self.beatModified.emit()
