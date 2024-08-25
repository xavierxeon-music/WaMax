#


class Note:

   noteNames = ['C', 'C#', 'D', 'D#', 'E', 'F', 'F#', 'G', 'G#', 'A', 'A#', 'B']
   blackKeys = [False, True, False, True, False, False, True, False, True, False, True, False]


class Scale:

   all = list()

   def __init__(self):

      self.offset = 0
      self.minorName = None
      self.majorName = None

      self.active = [False] * 12

   def __str__(self):

      return f'{self.offset} {self.majorName}'

   @staticmethod
   def accidentials(name):
      name = name.replace("sharp", "\u266F")
      name = name.replace("flat", "\u266D")

      return name

   @staticmethod
   def add(offset, majorName, minorName):

      scale = Scale()

      scale.offset = offset
      scale.majorName = Scale.accidentials(majorName)
      scale.minorName = Scale.accidentials(minorName)

      inScale = [True, False, True, False, True, True, False, True, False, True, False, True]
      for index in range(12):
         note = (index + (7 * offset)) % 12
         if note < 0:
            note += 12
         scale.active[note] = inScale[index]

      Scale.all.append(scale)


if __name__ == '_common.music':

   Scale.add(-6, 'Gflat', 'eflat')
   Scale.add(-5, 'Dflat', 'bflat')
   Scale.add(-4, 'Aflat', 'f')
   Scale.add(-3, 'Eflat', 'c')
   Scale.add(-2, 'Bflat', 'g')
   Scale.add(-1, 'F', 'd')
   Scale.add(0, 'C', 'a')
   Scale.add(+1, 'G', 'e')
   Scale.add(+2, 'D', 'b')
   Scale.add(+3, 'A', 'fsharp')
   Scale.add(+4, 'E', 'csharp')
   Scale.add(+5, 'B', 'gsharp')
   Scale.add(+6, 'Fsharp', 'dsharp')
