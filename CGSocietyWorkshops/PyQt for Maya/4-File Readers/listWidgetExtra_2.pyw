from PyQt4.QtCore import *
from PyQt4.QtGui import *
import os, sys, sip

windowTitle = "List Widget Example"
windowObject = "listWidgetEx"

def getMayaWindow():
    import maya.OpenMayaUI as mui
    #Get the maya main window as a QMainWindow instance
    ptr = mui.MQtUtil.mainWindow()
    return sip.wrapinstance(long(ptr), QObject)

class ListWidgetExtra(QListWidget):
    def __init__(self, parent=None):
        #Normal super to setup the objects
        super(ListWidgetExtra, self).__init__(parent)

    def getSelectedText(self):
        if self.row(self.currentItem()) >= 0:
            return self.currentItem().text()
        else:
            return ""

    def rename(self, newname, directory):
        selItems = []
        for row in range(self.count()):
            if self.isItemSelected(self.item(row)):
                selItems.append(self.item(row))

        for item in selItems:
            currentPath = os.path.join(directory, str(item.text()))
            newPath = os.path.join(directory, newname)
            splitname = newname.split(os.extsep, 1)

            #If no file extension, add blank
            if len(splitname) < 2:
                splitname.append("")
            else:
                splitname[1] = "."+splitname[1]

            if os.path.exists(currentPath):
                counter = 1
                while os.path.exists(newPath):
                    newPath = os.path.join(directory, splitname[0]+str(counter)+splitname[1])
                    counter += 1
                os.rename(currentPath, newPath)

        self.clear()
        self.fillList(directory)

    def fillList(self, directory):
        self.addItems(list(set(os.listdir(directory))))

    def move(self, up=1):
        #Create list of selected items from top to bottom
        selItems = []
        lastIndex = self.count()-1
        targetRow = lastIndex if up else 0
        for row in range(self.count()):
            if self.isItemSelected(self.item(row)):
                selItems.append(self.item(row))
                if up:
                    targetRow = min(targetRow, row)        
                else:
                    targetRow = max(targetRow, row)

        #Adjust the target row to the correct index 
        if up:
            targetRow = max(0,targetRow-1)
        else:
            targetRow = min(lastIndex,targetRow+1)

        #remove, insert and select each item
        for item in selItems:   
            itemTaken = self.takeItem(self.row(item))
            self.insertItem(targetRow, itemTaken)
            self.setItemSelected(itemTaken, True)
            if up:
                targetRow += 1
        
class ListExtraWidget(QWidget):
    def __init__(self, parent=None):
        #Normal super to setup the objects
        super(ListExtraWidget, self).__init__(parent)

        self.box = QVBoxLayout()
        self.textbox = QVBoxLayout()
        self.grid = QGridLayout()

        self.listWidgetE = ListWidgetExtra()
        self.listWidgetE.setSelectionMode(QAbstractItemView.ExtendedSelection)
        self.sortUp = QPushButton("sort up")
        self.sortDown = QPushButton("sort down")
        self.moveUp = QPushButton("move up")
        self.moveDown = QPushButton("move down")
        self.renamefile = QLineEdit()

        self.mainWidget.setLayout(self.box)
        self.box.addWidget(self.listWidgetE)
        self.box.addLayout(self.grid)
        self.box.addLayout(self.textbox)

        self.grid.addWidget(self.sortUp,0,0)
        self.grid.addWidget(self.sortDown,1,0)
        self.grid.addWidget(self.moveUp,0,1)
        self.grid.addWidget(self.moveDown,1,1)
        self.textbox.addWidget(QLabel("Rename:"))
        self.textbox.addWidget(self.renamefile)

        self.randomPath = os.path.join(os.path.dirname(__file__),"randomFiles")
        self.listWidgetE.fillList(self.randomPath)

        self.sortUp.clicked.connect(lambda: self.listWidgetE.sortItems(Qt.AscendingOrder))
        self.sortDown.clicked.connect(lambda: self.listWidgetE.sortItems(Qt.DescendingOrder))
        self.moveUp.clicked.connect(lambda: self.listWidgetE.move(1))
        self.moveDown.clicked.connect(lambda: self.listWidgetE.move(0))
        self.listWidgetE.currentRowChanged.connect(lambda: self.renamefile.setText(self.listWidgetE.getSelectedText()))
        self.renamefile.returnPressed.connect(lambda: self.listWidgetE.rename(str(self.renamefile.text()),self.randomPath))
        
class ListExtraWin(QMainWindow):
    def __init__(self, parent=None):
        super(ListExtraWin, self).__init__(parent)
        self.leWidget = ListExtraWidget()
        self.setCentralWidget(self.leWidget)
        self.setObjectName(windowObject)
        self.setWindowTitle(windowTitle)

def mayaRun():
    import maya.cmds as cmds
    if cmds.window(windowObject, q=True, exists=True):
        cmds.deleteUI(windowObject)
    global gui
    gui = ListExtraWin(getMayaWindow())
    gui.show()
    
def run(args):
    app = QApplication(args)
    gui = ListExtraWin()
    gui.show()
    sys.exit(app.exec_())

if __name__ == "__main__":
    run(args=sys.argv)

mayaRun()
