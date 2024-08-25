from PySide6.QtGui import QStandardItemModel

from PySide6.QtGui import QStandardItem, QPalette
from PySide6.QtWidgets import QApplication


from .calendar import Calender


class TimePointModel(QStandardItemModel):

   def __init__(self):

      super().__init__()

      Calender.the.loaded.connect(self._create)
      Calender.the.tagsChanged.connect(self._update)

   def _create(self):

      self.clear()

      for timePoint in Calender.the.data:

         tpItem = QStandardItem(timePoint)
         tpItem.setEditable(False)
         self.invisibleRootItem().appendRow(tpItem)

      self.setHorizontalHeaderLabels(['time point'])

      self._update()

   def _update(self):

      activeBrush = QApplication.palette().brush(QPalette.Text)
      inactiveBrush = QApplication.palette().brush(QPalette.Mid)

      for row in range(self.invisibleRootItem().rowCount()):

         tpItem = self.item(row, 0)
         tp = tpItem.text()
         active = False
         for tag in Calender.the.data[tp].keys():
            if Calender.the.tagDict[tag]:
               active = True
               break

         if not active:
            tpItem.setForeground(inactiveBrush)
         else:
            tpItem.setForeground(activeBrush)
