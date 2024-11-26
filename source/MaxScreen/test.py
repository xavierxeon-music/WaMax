#!/usr/bin/env python3

import os
import signal
import sys

from PySide6.QtCore import QCoreApplication, QTimer
from PySide6.QtNetwork import QTcpSocket


class Client:

   def __init__(self):

      self.socket = QTcpSocket()

      self.socket.readyRead.connect(self.readData)
      # self.socket.connectToHost('WaTravel.local', 6667)
      self.socket.connectToHost('192.168.1.109', 6667)

   def readData(self):

      text = self.socket.readAll()
      text = text.data().decode()
      print(text)


def signit_handler(*args):

   QCoreApplication.quit()


def main():

   app = QCoreApplication([])

   signal.signal(signal.SIGINT, signit_handler)
   timer = QTimer()
   timer.start(500)
   timer.timeout.connect(lambda: None)  # Let the interpreter run each 500 ms.

   client = Client()

   sys.exit(app.exec())


if __name__ == "__main__":
   main()
