from PyQt4.QtCore import *

settings = QSettings("ChocoINC", "Awesome App")

settings.setValue("editor/wrapMargin", 50)

settings.beginGroup("MainWindow");
settings.setValue("size", (50, 700));
settings.setValue("pos", (100, 20));
settings.endGroup();

