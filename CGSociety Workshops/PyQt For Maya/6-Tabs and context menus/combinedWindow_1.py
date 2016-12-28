from PyQt4.QtCore import *
from PyQt4.QtGui import *
from PyQt4 import uic
import os, sys, sip
import listWidgetExtra as lwe
import finalFileReader_1 as ffr
import maya.cmds as cmds
reload(ffr)
reload(lwe)

uiFile = os.path.join(os.path.dirname(__file__),"combinedWindow_1.ui")

windowTitle = "Combined Window"
windowObject = "pyCombinedWin"

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
        
        self.readerWidget = ffr.FileReaderWidget()
        self.listExtraWidget = lwe.ListExtraWidget()
        
        self.tab1_gridLayout = QGridLayout()
        self.tab1_gridLayout.addWidget(self.readerWidget)

        self.tab3_gridLayout = QGridLayout()
        self.tab3_gridLayout.addWidget(self.listExtraWidget)

        self.listWidget.setContextMenuPolicy(Qt.CustomContextMenu)
        self.listWidget.customContextMenuRequested.connect(self.Tab2RightClickMenu)
        self.listWidget.itemDoubleClicked.connect(self.focusOnNode)
        
        self.tab.setLayout(self.tab1_gridLayout)
        self.tab_3.setLayout(self.tab3_gridLayout)

    def keyPressEvent(self, event):
        if event.key() == Qt.Key_Escape:
            self.close()
        super(CombinedWin, self).keyPressEvent(event)

    def Tab2RightClickMenu(self, pos):
        newPos = QPoint(pos.x()+20, pos.y()+30)
        menu = QMenu()        
        menu.addAction("Fill List from Scene", self.fillList)
        menu.addAction("Clear List", self.clearList)
        menu.addAction("Remove selected in List", self.removeList)
        menu.addSeparator()
        menu.addAction("Append selected from Scene", self.appendList)
        menu.addAction("Select in Scene", self.selectInScene)
        menu.addAction("Delete in Scene", self.deleteInScene)
        menu.exec_(self.mapToGlobal(newPos))

    def focusOnNode(self,item):
        itemName = str(item.text())
        if cmds.objExists(itemName):
            cmds.select([itemName])
            cmds.FrameSelected(itemName)
        else:
            cmds.warning(itemName + " No longer exists")
            removedItem = self.listWidget.takeItem(self.listWidget.row(item))
            del removedItem

    def listHasSelectedMembers(self):                 
        if len(self.listWidget.selectedItems()) == 0:
            cmds.warning("Please select from the list first")
            return False
        return True

    def fillList(self):
        self.listWidget.clear()
        self.listWidget.addItems(cmds.ls(type='transform'))
        self.listWidget.sortItems()              

    def appendList(self):
        sel = cmds.ls(sl=True)
        if(len(sel) > 0):
            for item in sel:
                if not len(self.listWidget.findItems(item,Qt.MatchExactly)):
                    self.listWidget.addItem(item)
            self.listWidget.sortItems()
        else:
            cmds.warning("Please select from the scene to append to list")

    def removeList(self):
        if self.listHasSelectedMembers():
            for item in self.listWidget.selectedItems():
                removedItem = self.listWidget.takeItem(self.listWidget.row(item))
                del removedItem

    def clearList(self):
        self.listWidget.clear()
        
    def selectInScene(self):
        if self.listHasSelectedMembers():
            toSel = []
            for item in self.listWidget.selectedItems():
                toSel.append(item.text())
            cmds.select(toSel)

    def deleteInScene(self):
        if self.listHasSelectedMembers():
            toSel = []
            for item in self.listWidget.selectedItems():
                toSel.append(item.text())
                removedItem = self.listWidget.takeItem(self.listWidget.row(item))
                del removedItem
            cmds.delete(toSel)

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
