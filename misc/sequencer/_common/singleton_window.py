from PySide6.QtWidgets import QMainWindow

import os
import platform
import sys
import signal

from PySide6.QtCore import QSettings, QStandardPaths, QTimer
from PySide6.QtNetwork import QLocalServer, QLocalSocket
from PySide6.QtWidgets import QApplication, QDockWidget, QWidget


class SingeltonWindow(QMainWindow):

   def __init__(self):

      super().__init__()

      self._currentFile = ''
      self.updateWindowTitle(False)

      appName = QApplication.applicationName()

      self._server = None
      self._socketName = QStandardPaths.writableLocation(
          QStandardPaths.TempLocation) + '/' + appName.replace(" ", "_")

      qtsettings = QSettings()
      self.restoreGeometry(qtsettings.value('geometry'))
      self.restoreState(qtsettings.value('state'))

      # print(qtsettings.fileName())

   def __del__(self):

      # if self._server:
      #    self._server.close()
      pass

   def closeEvent(self, event):

      qtsettings = QSettings()
      qtsettings.setValue('geometry', self.saveGeometry())
      qtsettings.setValue('state', self.saveState())

      super().closeEvent(event)

   def singletonLoad(self, fileName):

      socket = QLocalSocket()
      socket.connectToServer(self._socketName)
      if socket.waitForConnected():  # found server
         # print('socket connected')
         socket.write(fileName.encode())
         socket.waitForBytesWritten()
         return False
      elif not self._server:
         # print('create server')
         if os.path.exists(self._socketName):
            os.remove(self._socketName)
         self._server = QLocalServer()
         self._server.newConnection.connect(self._serverConnected)
         self._server.listen(self._socketName)

      # print('load file locally', fileName)
      self.loadFile(fileName)
      return True

   def addAndCreateDockWidget(self, payload, name, area):

      dockWidget = QDockWidget()
      dockWidget.setObjectName(name)
      dockWidget.setWidget(payload)
      dockWidget.setTitleBarWidget(QWidget())
      dockWidget.setFeatures(QDockWidget.NoDockWidgetFeatures)
      self.addDockWidget(area, dockWidget)

   def _serverConnected(self):
      socket = self._server.nextPendingConnection()
      socket.waitForReadyRead()
      fileName = bytes(socket.readAll()).decode()
      self.loadFile(fileName)

   def loadFile(self, fileName):

      pass

   def updateWindowTitle(self, isModified, fileName=None):

      if fileName:
         self._currentFile = fileName

      appName = QApplication.applicationName()
      if not self._currentFile:
         self.setWindowTitle(f'{appName} [*]')
      else:
         fileName = os.path.basename(self._currentFile)
         self.setWindowTitle(f'{appName} - {fileName} [*]')

      self.setWindowModified(isModified)

   def dataModified(self):

      self.setWindowModified(True)

   @staticmethod
   def _signit_handler(*args):

      QApplication.quit()

   @classmethod
   def start(cls, appName):

      QApplication.setOrganizationName('Schweinesystem')
      QApplication.setOrganizationDomain('schweinesystem.eu')
      QApplication.setApplicationName(appName)

      app = QApplication([])

      fileName = ' '.join(sys.argv[1:])
      if 'Darwin' == platform.system():
         index = fileName.find(':')
         if index >= 0:
            frontPart = fileName[:index]
            if not frontPart.startswith('/Volumes'):
               frontPart = '/Volumes/' + frontPart
            endPart = fileName[index + 1:]
            fileName = frontPart + endPart

      mainWindow = cls()
      if not mainWindow.singletonLoad(fileName):  # other instance of application is running
         print("open file in exisiting application")
         return 0

      signal.signal(signal.SIGINT, SingeltonWindow._signit_handler)
      timer = QTimer()
      timer.start(500)
      timer.timeout.connect(lambda: None)  # Let the interpreter run each 500 ms.

      mainWindow.show()
      status = app.exec()
      sys.exit(status)
