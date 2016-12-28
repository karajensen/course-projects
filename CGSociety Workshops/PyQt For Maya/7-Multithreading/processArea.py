import os
from PyQt4.QtCore import *
from PyQt4.QtGui import *
from PyQt4 import uic

import procThread as pThread

uiFile = os.path.join(os.path.dirname(__file__),"processArea.ui")
uiDialog = os.path.join(os.path.dirname(__file__),"dialog.ui")

class dieThread(QThread):
    def run(self):
        pass

class DialogBox(QDialog):
    def __init__(self, parent):
        """Super, loadUi, signal connections"""
        super(DialogBox, self).__init__(parent)
        uic.loadUi(uiDialog, self)

        self.Cancel.clicked.connect(lambda: self.reject())
        self.Accept.clicked.connect(lambda: self.accept())
        self.jEdit.clicked.connect(self.jeditClicked)
        self.Python.clicked.connect(self.pythonClicked)
        self.Other.clicked.connect(self.otherClicked)
        self.command = ""
        self.script = ""

    def otherClicked(self):
        self.command = ""
        self.openFileBrowser()

    def pythonClicked(self):
        self.command = "python"
        self.openFileBrowser()

    def jeditClicked(self):
        self.command = "jedit"
        self.openFileBrowser()

    def openFileBrowser(self):
         self.LaunchWith.setText("Launch With: "+self.command)
         self.script = QFileDialog.getOpenFileName()
         self.scriptEdit.setText(self.script)
         self.commandEdit.setText(self.command)

class ProcessModule(QWidget):
    def __init__(self, parent, proc, procName="", priority=1):
        """Super, loadUi, signal connections"""
        #Normal super to setup the objects
        self.parent = parent
        super(ProcessModule, self).__init__(self.parent)
        uic.loadUi(uiFile, self)

        self.active.setChecked(True)
        self.procName.setText(procName)
        self.command.setText(proc[0])
        self.script.setText(proc[1])
        self.args.setText(proc[2])
        
        self.priority.setValidator(QIntValidator())
        self.priority.setText(str(priority)) 
                
        self.priVal = priority
        self.priority.textChanged.connect(self.setPriVal)
        self.loadScript.clicked.connect(self.openDialog)
        
        self.defaultColor = "Grey"
        self.runColor = "White"
        self.successColor = "Green"
        self.failColor = "Red"
        self.skipColor = "Purple"
                
        self.reset()
    
    def setPriVal(self, text):
        try:
            self.priVal = int(text)
        except ValueError:
            pass

    def updateTextBox(self):
        self.script.setText(self.dialog.script)
        self.command.setText(self.dialog.command)

    def openDialog(self):
        self.dialog = DialogBox(self)
        self.dialog.accepted.connect(self.updateTextBox)
        self.dialog.open()
    
    def reset(self):
        self.colorFrame.setStyleSheet("QFrame { background-color: %s }" % self.defaultColor)        
     
    def procReturn(self):
        out, err = self.thread.p.communicate()
        print "Subprocess Results: ", self.thread.p.returncode
        
        if out:
            print "Subprocess Output: ", out
        if err:
            print "Subprocess Error: ", err
            
        if self.thread.p.returncode == 0:
            print "Script Succeeded"
            self.colorFrame.setStyleSheet("QFrame { background-color: %s }" % self.successColor )
        elif self.thread.p.returncode == 1:
            print "Script Failed"
            self.colorFrame.setStyleSheet("QFrame { background-color: %s }" % self.failColor )
        QApplication.processEvents()

        self.parent.nextProc()
        
    def runScript(self):
        proc = [str(self.command.text()), str(self.script.text()), str(self.args.text())]
        if self.active.isChecked():
            self.colorFrame.setStyleSheet("QFrame { background-color: %s }" % self.runColor)
            QApplication.processEvents()
            self.thread = pThread.ProcThread(proc)
            self.thread.finished.connect(self.procReturn)
            print "Command:" + proc[0]
            print "Before Thread Start"
            print "Running Thread"
            self.thread.start()
            print "Thread Started"

        else:
            self.colorFrame.setStyleSheet("QFrame { background-color: %s }" % self.skipColor)
            QApplication.processEvents()
            self.thread = dieThread()
            self.thread.finished.connect(self.parent.nextProc)
            self.thread.start()
            print "Skipping Thread"
        
    def killProc(self):
        if hasattr(self.thread, "p"):
            return self.thread.p.pid
