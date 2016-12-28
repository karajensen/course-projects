from PyQt4.QtCore import *
from PyQt4.QtGui import *
from PyQt4 import uic
import os, sys, sip
#import finalFileReader_1 as ffr

uiFile = os.path.join(os.path.dirname(__file__),"combinedWindow_designerSubclass.ui")

windowTitle = "Combined Window"
windowObject =  "pyCombinedWin"

def getMayaWindow():
    import maya.OpenMayaUI as mui
    #Get the maya main window as a QMainWindow instance
    ptr = mui.MQtUtil.mainWindow()
    return sip.wrapinstance(long(ptr), QObject)

class CombinedWin(QMainWindow):
    def __init__(self, parent=None):
        """Super, loadUi, signal connections"""
        super(CombinedWin, self).__init__(parent)

        self.setObjectName(windowObject)
        self.setWindowTitle(windowTitle)

        uic.loadUi(uiFile, self)
        
        """
        self.readerWidget = ffr.FileReaderWidget()
        
        self.tab1_gridLayout = QGridLayout()
        self.tab1_gridLayout.addWidget(self.readerWidget)
        
        self.tab.setLayout(self.tab1_gridLayout)
        """
        
def mayaRun():
    import maya.cmds as cmds
    if cmds.window(windowObject, q=True, exists=True):
        cmds.deleteUI(windowObject)
    global gui
    gui = CombinedWin(getMayaWindow())
    gui.show()
    
def run():
    app = QApplication(sys.argv)
    gui = CombinedWin()
    gui.show()
    sys.exit(app.exec_())

if __name__ == "__main__":
    run()
    
mayaRun()
