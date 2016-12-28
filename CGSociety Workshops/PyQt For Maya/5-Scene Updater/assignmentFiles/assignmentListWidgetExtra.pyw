from PyQt4.QtCore import *
from PyQt4.QtGui import *
import os, sys, sip

#Window Stuff
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
     
    def move(self, up=1):
        #Needs a starting place
        #The whole concept is setting a start index that's
        #As far a way as possible.
        #So if you're going up, the furthest index you could travel from is the count
        #If you're going down, the furthest index away is 0
        if up:
            curRow = self.count()
        else:
            curRow = 0
        
        #This is a group that gets all the names to later select them
        allItems = []
            
        #This figures out the proper insert point by looping over everything
        #And if the values is larger or smaller (depending on the direction)
        #The the curRow to work with is overwritten
        for item in self.selectedItems():
            if up:
                if self.row(item) < curRow:
                    curRow = self.row(item)
            else:
                if self.row(item) > curRow:
                    curRow = self.row(item)
            
            #Grabs the text off all the objects to create new objects
            #And to select them later
            allItems.append(self.takeItem(self.row(item)).text())
        
        #Inserts all those names in at the proper point
        #This creates new items because the list is just strings
        if up:
            if curRow > 0:
                #Moves the insert up the list from the highest one
                self.insertItems( curRow - 1, allItems )
            else:
                #This makes sure that you don't delete anything off the list
                self.insertItems( 0, allItems )
        else:
            if curRow < self.count():
                #Moves the insert down the list from the lowest one
                self.insertItems( curRow + 1, allItems )
            else:
                self.insertItems( self.count(), allItems )
                
        #Selects all the appropriate objects in the list
        if allItems:
            for item in allItems:
                found = self.findItems(item, Qt.MatchExactly)
                if found:
                    self.setItemSelected(found[0], True)
                #Variant way
                #item.setSelected(1)
        
class ListExtraWin(QMainWindow):
    def __init__(self, parent=None):
        #Normal super to setup the objects
        super(ListExtraWin, self).__init__(parent)
        
        #Sets the object name
        self.setObjectName(windowObject)    
        #Sets the window title
        self.setWindowTitle(windowTitle)
        
        self.mainWidget = QWidget()
        self.setCentralWidget(self.mainWidget)

        self.box = QVBoxLayout()

        self.listWidgetE = ListWidgetExtra()
        self.listWidgetE.setSelectionMode(self.listWidgetE.ExtendedSelection)

        self.sortUp = QPushButton("sort up")
        self.sortDown = QPushButton("sort down")
        self.moveUp = QPushButton("move up")
        self.moveDown = QPushButton("move down")

        self.mainWidget.setLayout(self.box)

        self.box.addWidget(self.listWidgetE)
        self.box.addWidget(self.sortUp)
        self.box.addWidget(self.sortDown)
        self.box.addWidget(self.moveUp)
        self.box.addWidget(self.moveDown)

        self.listWidgetE.addItems(os.listdir(r"C:\Users\Malcolm\Dropbox\CG Workshops\introToPyQt\code\Hour 4\randomFiles"))

        self.sortUp.clicked.connect(lambda: self.listWidgetE.sortItems(Qt.AscendingOrder))
        self.sortDown.clicked.connect(lambda: self.listWidgetE.sortItems(Qt.DescendingOrder))
        self.moveUp.clicked.connect(lambda: self.listWidgetE.move(1))
        self.moveDown.clicked.connect(lambda: self.listWidgetE.move(0))

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
