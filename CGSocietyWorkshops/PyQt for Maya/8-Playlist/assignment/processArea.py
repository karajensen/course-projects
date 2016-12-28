import os
from PyQt4.QtCore import *
from PyQt4.QtGui import *
from PyQt4 import uic

import procThread as pThread

uiFile = os.path.join(os.path.dirname(__file__),"processArea.ui")
launchWith = os.path.join(os.path.dirname(__file__),"launchWith.ui")

class dieThread(QThread):
    def run():
        pass

class LaunchWithDia(QDialog):
    def __init__(self, parent):
        """Super, loadUi, signal connections"""
        #Normal super to setup the objects
        super(LaunchWithDia, self).__init__(parent)
        uic.loadUi(launchWith, self)
        
        self.py_but.clicked.connect(lambda: self.setVal("python"))
        self.jedit_but.clicked.connect(lambda: self.setVal("jedit"))
        self.other_but.clicked.connect(lambda: self.setVal("other"))
        self.cancel_but.clicked.connect(lambda: self.setVal("cancel"))
        
    def setVal(self, val):
        if val == "other":
            self.val = self.other_le.text()
        else:
            self.val = val
        self.close()
        
class ProcessModule(QWidget):
    
    done = pyqtSignal()
    
    def __init__(self, parent, proc, procName="", priority=1):
        """Super, loadUi, signal connections"""
        #Normal super to setup the objects
        self.parent = parent
        super(ProcessModule, self).__init__(self.parent)
        
        uic.loadUi(uiFile, self)

        self.procName_le.setText(procName)
        
        self.command_le.setText(proc[0])
        self.script_le.setText(proc[1])
        
        if len(proc) > 2:
            args = [str(x) for x in proc[2:]]
            args = ", ".join( args  )
            self.args_le.setText(args)
        
        self.priority.setValidator(QIntValidator())
        self.priority.setText(str(priority)) 
                
        self.priVal = priority
        self.priority.textChanged.connect(self.setPriVal)
        
        self.defaultColor = "Grey"
        self.runColor = "White"
        self.successColor = "Green"
        self.failColor = "Red"
        self.disabledColor = "Purple"
                
        self.loadScript_but.clicked.connect(self.loadScript)
                
        self.done.connect(self.parent.nextProc)
                
        self.reset()
    
    def loadScript(self):
        dia = LaunchWithDia(None)
        dia.exec_()
        
        if dia.val != "cancel":
            self.command_le.setText(dia.val)
            fileDialog = QFileDialog()
            fileName = fileDialog.getOpenFileName(None, "Select Script", os.path.dirname(__file__))
            if fileName:
                self.script_le.setText(fileName)
                self.args_le.setText("")

    def setPriVal(self, text):
        try:
            self.priVal = int(text)
        except ValueError:
            pass
    
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
        if self.active_cb.isChecked():
            self.colorFrame.setStyleSheet("QFrame { background-color: %s }" % self.runColor)
            QApplication.processEvents()
            
            proc = [self.command_le.text(), self.script_le.text(), self.args_le.text()]
            self.thread = pThread.ProcThread(proc)
            
            self.thread.finished.connect(self.procReturn)
            
            print "Before Thread Start"
            print "Running Thread"
            self.thread.start()
            print "Thread Started"
        
        else:
            self.colorFrame.setStyleSheet("QFrame { background-color: %s }" % self.disabledColor)
            QApplication.processEvents()
            self.thread = dieThread()
            self.thread.finished.connect(self.parent.nextProc)
            self.thread.start()
            #self.parent.nextProc()
            #self.done.emit()
            
    def killProc(self):
        if hasattr(self.thread, "p"):
            return self.thread.p.pid
