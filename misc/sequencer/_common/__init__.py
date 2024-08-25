#!/usr/bin/env python3

from .delegate_combobox import DelegateCompbBox
from .dataview import DataView
from .resource import Icon, Resource
from .music import Note, Scale
from .singleton_window import SingeltonWindow
from .stretcher_widget import StretcherWidget
from .timepoint import TimePoint


def __addCommonToSysPath():

   import os
   import sys
   common_dir_name = os.path.dirname(os.path.realpath(__file__))
   sys.path.append(common_dir_name)


if __name__ == '_common':
   __addCommonToSysPath()
