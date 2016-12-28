from PyQt4.QtCore import *

settings = QSettings("ChocoINC", "Awesome App")
newVal = settings.value("editor/wrapMargin")
print newVal

#Returns NONE because it doesn't exist
print settings.value("editor/test")

settings.beginGroup("MainWindow")
print settings.value("size")
print settings.value("pos")
settings.endGroup()
