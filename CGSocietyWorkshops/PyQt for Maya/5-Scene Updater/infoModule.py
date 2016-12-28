from PyQt4.QtCore import *
from PyQt4.QtGui import *
from PyQt4 import uic
import os, sys, sip
import maya.cmds as cmds 

uiFile = os.path.join(os.path.dirname(__file__),"infoModule.ui")
    
class InfoModuleWidget(QWidget):
    def __init__(self, obj, parent=None):
        """Super, loadUi, signal connections"""
        super(InfoModuleWidget, self).__init__(parent)
            
        #Connects the ui elements to self
        uic.loadUi(uiFile, self)
        
        self.obj = obj
        self.name_l.setText("Information for: " + self.obj)
        
        self.attrs = ["tx", "ty", "tz", "rx", "ry", "rz", "sx", "sy", "sz"]
        
        self.createJobs()
        self.fillModData()
        
    def createJobs(self):
        self.scriptJobs = []
        for attr in self.attrs:
            self.scriptJobs.append(cmds.scriptJob( kws=True, attributeChange=[self.obj+"."+attr, self.fillModData] ))

    def killJobs(self):
        for job in self.scriptJobs:
            cmds.scriptJob(kill=job, force=True)
        
    def fillModData(self):
        count = 0
        for x in range(3):
            for y in range(3):
                attrVal = cmds.getAttr(self.obj+"."+self.attrs[count])
                newSB = QDoubleSpinBox()           
                newSB.setValue(float(attrVal))
                newSB.attrData = self.attrs[count]
                self.info_t.setCellWidget(y, x, newSB)
                
                self.isEditingText = False
                newSB.valueChanged.connect(self.attrChange)
                newSB.editingFinished .connect(self.editingFinished)
                newSB.lineEdit().textEdited.connect(self.toggleEditText)
        
                count += 1

    def toggleEditText(self):
        self.isEditingText = True
    
    def attrChange(self):
        if not self.isEditingText:
            cmds.setAttr(self.obj+"."+self.sender().attrData, float(self.sender().text()))

    def editingFinished(self):
        self.isEditingText = False
        cmds.setAttr(self.obj+"."+self.sender().attrData, float(self.sender().text()))
        
        
def mayaRun():
    import maya.cmds as cmds
    global gui
    gui = InfoModuleWidget("pSphere1")
    gui.show()

#mayaRun()
