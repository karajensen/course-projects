from PyQt4.QtCore import *
from PyQt4.QtGui import *
from PyQt4 import uic
import os, sys, sip

uiFile = os.path.join(os.path.dirname(__file__),"finalFileReader_1.ui")

windowTitle = "File Reader"
windowObject = "fileReaderWin"

def getMayaWindow():
    import maya.OpenMayaUI as mui
    #Get the maya main window as a QMainWindow instance
    ptr = mui.MQtUtil.mainWindow()
    return sip.wrapinstance(long(ptr), QObject)

class FileReaderWidget(QWidget):
    def __init__(self, parent=None, startDir="", runIn="python"):
        """Super, loadUi, signal connections"""
        super(FileReaderWidget, self).__init__(parent)

        self.runIn = runIn
        if not startDir:
            startDir = os.path.dirname(__file__)
        
        #Connects the ui elements to self
        uic.loadUi(uiFile, self)
        
        self.browse_pb.clicked.connect(self.folderBrowse)
        self.folder_le.editingFinished.connect(self.fillList)
        self.files_lw.currentItemChanged.connect(self.fillTextBrowser)
        
        self.folders_lw.itemDoubleClicked.connect(self.jumpFolder)
        
        self.folder_le.setText(startDir)
        self.fillList()
        
    def jumpFolder(self, *args):
        workFolder = self.folder_le.text()
        newFolder = args[0].text()
        
        if newFolder == "..":
            self.folder_le.setText(os.path.split(workFolder)[0])
        else:
            if os.path.exists(os.path.join(workFolder, newFolder)):
                self.folder_le.setText(os.path.join(workFolder, newFolder))
                
        self.fillList()

    def folderBrowse(self):
        folderName = ""

        if self.runIn == "python":
            fileDialog = QFileDialog()
            folderName = fileDialog.getExistingDirectory(None, "Select Folder", self.folder_le.text())
        
        elif self.runIn == "maya":
            import maya.cmds as cmds
            folderName = cmds.fileDialog2(fm=3, okc="Select Folder", dir=str(self.folder_le.text()))
            if folderName:
                folderName = folderName[0]
                
        if folderName:
            self.folder_le.setText(folderName)
            self.fillList()
    
    def fillList(self):
        self.files_lw.clear()
        self.folders_lw.clear()
        
        workFolder = str(self.folder_le.text())
        
        if os.path.exists(workFolder):
            contents = os.listdir(workFolder)
            filesToUse = []
            
            if os.path.split(workFolder)[1]:
                foldersToUse= [".."]
            else:
                foldersToUse = []

            for obj in contents:
                if os.path.isfile(os.path.join(workFolder, obj)):
                    filesToUse.append(obj)
                else:
                    foldersToUse.append(obj)
                    
            self.files_lw.insertItems(0, filesToUse)
            self.folders_lw.insertItems(0, foldersToUse)
        else:
            self.folders_lw.insertItem(0, "Folder Not Found")
    
    def fillTextBrowser(self):
        try:
            curFile = os.path.join(str(self.folder_le.text()), str(self.files_lw.currentItem().text()))
        except AttributeError:
            self.files_tb.setPlainText("")
            return
            
        if os.path.exists(curFile):
            fObj = open(curFile, "r")
            data = fObj.read()
            fObj.close()
            self.files_tb.setPlainText(data)
 
class FileReaderWin(QMainWindow):
    def __init__(self, parent=None, startDir="", runIn="python"):
        """Super, loadUi, signal connections"""
        super(FileReaderWin, self).__init__(parent)
        
        self.frWidget = FileReaderWidget(self, startDir, runIn)
        self.setCentralWidget(self.frWidget)
        
        self.setObjectName(windowObject)
        self.setWindowTitle(windowTitle)
 
def mayaRun(startDir="", runIn="maya"):
    import maya.cmds as cmds
    
    if cmds.window(windowObject, q=True, exists=True):
        cmds.deleteUI(windowObject)
    global gui
    gui = FileReaderWin(getMayaWindow(), startDir="", runIn=runIn)
    gui.show()
    
def run(startDir="", args=[], runIn="python"):
    app = QApplication(args)
    gui = FileReaderWin(startDir="", runIn=runIn)
    gui.show()
    sys.exit(app.exec_())

if __name__ == "__main__":
    run(startDir = "", args=sys.argv)
    
#mayaRun()
