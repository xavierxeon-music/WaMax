from PySide6.QtCore import QObject


from PySide6.QtCore import Signal

import functools
import json
import os

from _common import TimePoint


class Calender(QObject):

   loaded = Signal()
   timePointSelected = Signal(str)
   timePointEdited = Signal()
   eventEdited = Signal()
   tagsChanged = Signal()

   the = None

   def __init__(self):

      super().__init__()
      Calender.the = self

      self.data = dict()
      self._currentTimePoint = None

      self.tagDict = dict()

   def load(self, fileName):

      if not os.path.exists(fileName):
         return False

      self.data = dict()

      try:
         with open(fileName, 'r') as infile:
            content = json.load(infile)
      except json.JSONDecodeError:
         return False

      for tp, eventList in content.items():
         self.data[tp] = dict()
         for entry in eventList:
            entryContent = entry.split(' ')
            tag = entryContent[0]
            value = entryContent[1] if len(entryContent) > 1 else None
            self.data[tp][tag] = value

      self.tagDict = dict()
      for eventList in self.data.values():
         for event in eventList:
            if not event:
               continue
            eventContent = event.split(' ')
            tag = eventContent[0]
            if not tag in self.tagDict:
               self.tagDict[tag] = True

      self.loaded.emit()
      self.tagsChanged.emit()

      return True

   def save(self, fileName):

      def tpSort(text1, text2):
         tp1 = TimePoint(text1)
         tp2 = TimePoint(text2)
         if tp1 < tp2:
            return -1
         elif tp1 == tp2:
            return 0
         else:
            return 1

      timePoints = list(self.data.keys())
      timePoints.sort(key=functools.cmp_to_key(tpSort))

      sortedData = dict()
      for tp in timePoints:
         sortedData[tp] = list()
         for tag, value in self.data[tp].items():
            content = tag
            if value:
               content += ' ' + value
            sortedData[tp].append(content)

      with open(fileName, 'w') as outfile:
         json.dump(sortedData, outfile, indent=3)

   def clear(self):

      self.data = dict()
      self.loaded.emit()

   def addTag(self, tagName):

      if tagName in self.tagDict:
         return

      self.tagDict[tagName] = True
      self.tagsChanged.emit()

   def checkTag(self, tagName, active):

      if not tagName in self.tagDict:
         return

      self.tagDict[tagName] = active
      self.tagsChanged.emit()

   def checkAllTags(self):

      for tagName in self.tagDict.keys():
         self.tagDict[tagName] = True

      self.tagsChanged.emit()

   def checkNoTags(self):

      for tagName in self.tagDict.keys():
         self.tagDict[tagName] = False

      self.tagsChanged.emit()

   def isValidTimePoint(self, timePoint):

      tp = TimePoint(timePoint)
      if not tp.valid():
         return False

      if timePoint in self.data:
         return False

      return True

   def addTimePoint(self, timePoint):

      self.data[timePoint] = list()
      self._currentTimePoint = timePoint

      self.loaded.emit()
      self.timePointEdited.emit()

   def removeTimePoint(self, timePoint):

      del self.data[timePoint]
      if timePoint == self._currentTimePoint:
         self._currentTimePoint = None

      self.loaded.emit()
      self.timePointEdited.emit()

   def setCurrentTimePoint(self, timePoint):

      self._currentTimePoint = timePoint
      self.timePointSelected.emit(timePoint)

   def addEvent(self, tag):

      if not self._currentTimePoint:
         return

      current = Calender.the.data[self._currentTimePoint]
      if not tag or tag in current:
         return

      current[tag] = None

      self.timePointSelected.emit(self._currentTimePoint)  # update event model
      self.eventEdited.emit()

   def removeEvent(self, tag):

      if not self._currentTimePoint:
         return

      current = Calender.the.data[self._currentTimePoint]
      if not tag or not tag in current:
         return

      del current[tag]

      self.timePointSelected.emit(self._currentTimePoint)  # update event model
      self.eventEdited.emit()

   def editEvent(self, tag, value):

      if not self._currentTimePoint:
         return

      current = Calender.the.data[self._currentTimePoint]
      if not tag or not tag in current:
         return

      current[tag] = value

      self.eventEdited.emit()
