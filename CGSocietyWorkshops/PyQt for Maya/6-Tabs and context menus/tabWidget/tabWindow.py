from PyQt4.QtCore import *
from PyQt4.QtGui import *
from PyQt4 import uic
import os, sys, sip

uiFile = os.path.join(os.path.dirname(__file__),"tabWindow_1.ui")
#uiFile = os.path.join(os.path.dirname(__file__),"tabWindow_2.ui")

windowTitle = "Tabbed Window"
windowObject = "pyTabbedWin"

def getMayaWindow():
    import maya.OpenMayaUI as mui
    #Get the maya main window as a QMainWindow instance
    ptr = mui.MQtUtil.mainWindow()
    return sip.wrapinstance(long(ptr), QObject)

class TabbedWin(QMainWidget):
    def __init__(self, parent=None):
        """Super, loadUi, signal connections"""
        super(TabbedWin, self).__init__(parent)
        #super(TabbedWin, self).__init__(None)

        self.setObjectName(windowObject)
        self.setWindowTitle(windowTitle)

        uic.loadUi(uiFile, self)

        #self.mainWidget = QWidget()
        #self.setCentralWidget(self.mainWidget)
        #self.mainWidget.setLayout(self.gridLayout)

def mayaRun():
    import maya.cmds as cmds
    if cmds.window(windowObject, q=True, exists=True):
        cmds.deleteUI(windowObject)
    global gui
    gui = TabbedWin(getMayaWindow())
    gui.show()
    
def run():
    app = QApplication(sys.argv)
    gui = TabbedWin()
    gui.show()
    sys.exit(app.exec_())

if __name__ == "__main__":
    run()
    
mayaRun()
