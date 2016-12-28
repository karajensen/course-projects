import maya.OpenMayaUI as apiUI
from PyQt4.QtGui import *
from PyQt4.QtCore import *
import sip
import maya.cmds as cmds

def getMayaWindow():
    ptr = apiUI.MQtUtil.mainWindow()
    return sip.wrapinstance(long(ptr), QObject)

def toQtObject(mayaName):
    ptr = apiUI.MQtUtil.findControl(mayaName)
    if ptr is None:
        ptr = apiUI.MQtUtil.findLayout(mayaName)
    if ptr is None:
        ptr = apiUI.MQtUtil.findMenuItem(mayaName)
    if ptr is not None:
        return sip.wrapinstance(long(ptr), QObject)

windowTitle = "Separate Channel Box Window"
windowObject = "pySepChannelBox"

class SeparateChannelBox(QMainWindow):
    def __init__(self, parent=None):
        super(SeparateChannelBox, self).__init__(parent)
        
        self.setObjectName(windowObject)
        self.setWindowTitle(windowTitle)
        
        self.channelBox = cmds.channelBox()
        qtChanBox = toQtObject(self.channelBox)
        self.setCentralWidget(qtChanBox)

def mayaRun():
    if cmds.window(windowObject, q=True, exists=True):
        cmds.deleteUI(windowObject)
    global gui
    gui = SeparateChannelBox(getMayaWindow())
    gui.show()

mayaRun()
