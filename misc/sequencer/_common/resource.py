from PySide6.QtGui import QIcon

import inspect
import os


class Resource:

    def __init__(self, fileName, path, suffix=None):

        if not path.startswith('/'):
            path = '/' + path
        if not path.endswith('/'):
            path = path + '/'

        self.path = os.path.dirname(fileName) + path

        if suffix:
            if not suffix.startswith('.'):
                suffix = '.' + suffix
            self.suffix = suffix
        else:
            self.suffix = None

    def file(self, resourceName):

        fileName = self.path + resourceName
        if self.suffix:
            fileName += self.suffix

        return fileName


class Icon:

    @staticmethod
    def common(iconName):

        r = Resource(__file__, 'icons', 'svg')
        fileName = r.file(iconName)
        return QIcon(fileName)

    @staticmethod
    def app(iconName):

        stack = inspect.stack()
        if not stack:
            return QIcon()

        r = Resource(stack[1].filename, 'icons', 'svg')
        fileName = r.file(iconName)
        return QIcon(fileName)
