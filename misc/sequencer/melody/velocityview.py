from _common import DataView

from .veclocitymodel import VelocityModel


class VelocityView(DataView):

   def __init__(self):

      super().__init__(VelocityModel())

      self._model.modelReset.connect(self.modelUpdate)

   def modelUpdate(self):

      self.resizeColumnToContents(0)
      self.resizeColumnToContents(1)
