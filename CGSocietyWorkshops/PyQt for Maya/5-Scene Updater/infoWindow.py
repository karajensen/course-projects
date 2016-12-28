from PyQt4.QtCore import *
from PyQt4.QtGui import *
from PyQt4 import uic
import os, sys, sip

#adding as a top level import because everything here is in maya
import maya.cmds as cmds
#bringing in the actual information widget
import infoModule as infMod
    
windowTitle = "Info Window"
windowObject = "infoWin"

def getMayaWindow():
    import maya.OpenMayaUI as mui
    #Get the maya main window as a QMainWindow instance
    ptr = mui.MQtUtil.mainWindow()
    return sip.wrapinstance(long(ptr), QObject)

class InfoWin(QMainWindow):
    def __init__(self):
        """Super, loadUi, signal connections"""
        #Normal super to setup the objects
        super(InfoWin, self).__init__(getMayaWindow())
        
        #Creating the mainWidget and setting the names
        self.mainWidget = QWidget()
        self.setCentralWidget(self.mainWidget)
        self.setObjectName(windowObject)
        self.setWindowTitle(windowTitle)
        self.setMaximumWidth(450)
        self.setMinimumWidth(200)
        self.resize(410,270)
        
        self.mainLayout = QVBoxLayout()
        self.mainWidget.setLayout(self.mainLayout)

        self.modLayout = QGridLayout()
        self.scrollArea = QScrollArea()
        self.scrollWidget = QWidget()
        self.scrollWidget.setLayout(self.modLayout)
        self.scrollArea.setWidget(self.scrollWidget)
        self.scrollArea.setWidgetResizable(True)

        self.scrollLayout = QVBoxLayout()
        self.scrollLayout.addWidget(self.scrollArea)
        self.mainLayout.addLayout(self.scrollLayout)

        self.btnLayout = QVBoxLayout()
        self.mainLayout.addLayout(self.btnLayout)
        
        self.rebuild_b = QPushButton("Rebuild UI")
        self.btnLayout.addWidget(self.rebuild_b)
        self.rebuild_b.clicked.connect(self.fillInfo)
        
        self.killJob_b = QPushButton("Kill Jobs")
        self.btnLayout.addWidget(self.killJob_b)
        self.killJob_b.clicked.connect(self.killJobs)
        
        self.infoMods = []
        self.isSetup = 1
        
        self.fillInfo()

    def closeEvent(self, event):    
        print "this is a close event"

        if hasattr(self, "isSetup"):
            if self.isSetup == 1:
                self.killJobs()
                self.isSetup = 0
                print "jobs are killed"
        
        #This does not work here all the time.
        #super(InfoWin, self).closeEvent(event)

        #This is a bad idea if your parent class changes
        #event.accept()
        
        QMainWindow.closeEvent(self, event)
        
    def killJobs(self):
        for mod in self.infoMods:
            mod.killJobs()
            
    def fillInfo(self):
        self.killJobs()
        for mod in self.infoMods:
            mod.setParent(None)
            del(mod)
            
        self.infoMods = []
        sel = cmds.ls(sl=True)
        
        space = 10
        for obj in sel:
            newMod = infMod.InfoModuleWidget(obj, None)
            self.infoMods.append(newMod)
            self.modLayout.addWidget(newMod)
            self.modLayout.addItem(QSpacerItem(space, space, QSizePolicy.Minimum, QSizePolicy.Minimum))   
            
def mayaRun():
    if cmds.window(windowObject, q=True, exists=True):
        cmds.deleteUI(windowObject)
    global gui
    gui = InfoWin()
    gui.show()
    
mayaRun()
