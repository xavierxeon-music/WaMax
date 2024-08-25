# nothing to import


class Sequence:

    class Event:

        def __init__(self):

            self.active = False
            self.value = 0

    def __init__(self, timeline):

        self.length = 16
        self.loop = True
        self.timeline = timeline

        self.eventList = []  # (127 * length) matrix

        self._createEmptyEventList()

    def setLength(self, value):

        from .timeline import TimeLine

        activeEventList = self.compileActiveEventList()
        self.length = value

        self._createEmptyEventList()
        self._applyActiveEventList(activeEventList)

        self.timeline.sequenceLengthChanged.emit()
        self.timeline.sequenceUpdated.emit()

    def copy(self):

        activeEventList = self.compileActiveEventList()
        return activeEventList

    def paste(self, activeEventList):

        from .timeline import TimeLine

        self._createEmptyEventList()
        self._applyActiveEventList(activeEventList)

        self.timeline.sequenceLengthChanged.emit()
        self.timeline.sequenceUpdated.emit()

    def clear(self):

        from .timeline import TimeLine

        self._createEmptyEventList()
        from .timeline import TimeLine

        self.timeline.sequenceUpdated.emit()

    def apply(self, content):

        self.length = content['length']
        self.loop = content['loop']
        self._createEmptyEventList()

        activeEventList = content["events"]
        self._applyActiveEventList(activeEventList)

    def compile(self):

        activeEventList = self.compileActiveEventList()
        content = {'length': self.length, 'loop': self.loop, 'events': activeEventList}

        return content

    def toggle(self, timePoint, rowNumber):

        cell = self.eventList[timePoint][rowNumber]
        if cell.active:
            cell.active = False
        else:
            cell.active = True
            if 0 == cell.value:
                cell.value = 127

        self.timeline.sequenceUpdated.emit()

    def setValue(self, timePoint, rowNumber, value):

        cell = self.eventList[timePoint][rowNumber]
        cell.active = True
        cell.value = value

        self.timeline.sequenceUpdated.emit()

    def _createEmptyEventList(self):

        self.eventList = []
        for col in range(self.length):
            timePoint = []
            for row in range(128):
                timePoint.append(Sequence.Event())
            self.eventList.append(timePoint)

    def compileActiveEventList(self):

        acticveEventList = []

        for col in range(self.length):
            timePoint = dict()
            for rowNumber in range(128):
                cell = self.eventList[col][rowNumber]
                if cell.active:
                    timePoint[rowNumber] = cell.value

            acticveEventList.append(timePoint)

        return acticveEventList

    def _applyActiveEventList(self, activeEventList):

        for col in range(self.length):
            if col >= len(activeEventList):
                break
            timePoint = activeEventList[col]
            for rowNumber, value in timePoint.items():
                rowNumber = int(rowNumber)
                cell = self.eventList[col][rowNumber]
                cell.active = True
                cell.value = value
