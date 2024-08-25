from PySide6.QtWidgets import QSizePolicy


from PySide6.QtWidgets import QWidget, QSizePolicy


class StretcherWidget(QWidget):

   def __init__(self):

      super().__init__()
      self.setSizePolicy(QSizePolicy.MinimumExpanding, QSizePolicy.Preferred)
