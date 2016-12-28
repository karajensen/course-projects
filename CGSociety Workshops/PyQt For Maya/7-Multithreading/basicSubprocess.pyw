import os, sys, sip
from PyQt4.QtCore import *
from PyQt4.QtGui import *
import processArea as pa
import subprocess

#adding as a top level import because everything here is in maya
windowTitle = "Basic Threading Window"
windowObject = "btwWin"

class BasicThreadWin(QMainWindow):
    def __init__(self, parent=None):
        """Super, loadUi, signal connections"""
        #Normal super to setup the objects
        super(BasicThreadWin, self).__init__(parent)
        
        #Creating the mainWidget and setting the names
        self.mainWidget = QWidget()
        self.setCentralWidget(self.mainWidget)
        self.setObjectName(windowObject)
        self.setWindowTitle(windowTitle)

        self.mainLayout = QVBoxLayout()        
        self.mainWidget.setLayout(self.mainLayout)
        
        self.runProcs_b = QPushButton("Run Processes")
        self.mainLayout.addWidget(self.runProcs_b)
        self.runProcs_b.clicked.connect(self.runProcs)
        
        self.scrollArea = QScrollArea()
        self.subWidget = QWidget()
        self.subLayout = QVBoxLayout()
        self.subWidget.setSizePolicy(QSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed))
        self.scrollArea.setWidget(self.subWidget)
        self.subWidget.setLayout(self.subLayout)
        self.scrollArea.setWidgetResizable(True)
        self.mainLayout.addWidget(self.scrollArea)
        
        self.killProcs_b = QPushButton("Kill Processes")
        self.mainLayout.addWidget(self.killProcs_b)
        self.killProcs_b.clicked.connect(self.killProcs)
        
        self.mods = []
        
        for i in range(5):
            proc = ["python", os.path.join(os.path.dirname(__file__),"runSub.py"), str(i)]
            self.mods.append(pa.ProcessModule(self, proc, "Process #" +str(i), i+1) )
            self.subLayout.addWidget(self.mods[-1])
        
        self.curProc = 0
        self.resize(575, 560)
        self.setMinimumWidth(575)
        self.setMaximumWidth(575)
        self.setMinimumHeight(150)
        
    def runProcs(self):
        self.curProc = 0
        for mod in self.mods:
            mod.reset()
        self.mods.sort(key = lambda mod : mod.priVal)
        self.nextProc()

    def nextProc(self):
        if self.curProc < len(self.mods):
            self.mods[self.curProc].runScript()
            self.curProc += 1
        #Would run all threads at the same time
        #for mod in self.mods:
        #    mod.runScript()
        
    def killProcs(self):
        killVal = self.mods[self.curProc-1].killProc()
        #linux look into this
        #import os
        #os.kill(killVal)
        subprocess.Popen("taskkill /F /PID " + str(killVal))
        self.curProc = len(self.mods)
            
def run():
    app = QApplication(sys.argv)
    gui = BasicThreadWin()
    gui.show()
    sys.exit(app.exec_())
    
if __name__ == "__main__":
    run()
