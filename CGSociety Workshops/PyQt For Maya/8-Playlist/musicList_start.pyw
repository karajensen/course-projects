from PyQt4.QtCore import *
from PyQt4.QtGui import *
from PyQt4 import uic
import os, sys, sip, re, cPickle

uiFile = os.path.join(os.path.dirname(__file__),"musicList.ui")
libFile = os.path.join(os.path.dirname(__file__),"music.lib")
    
windowTitle = "Music List"
windowObject = "musicListWin"

class MusicListWin(QMainWindow):
    def __init__(self, parent=None):
        """Super, loadUi, signal connections"""
        super(MusicListWin, self).__init__(parent)
        
        #Connects the ui elements to self
        uic.loadUi(uiFile, self)
        
    def keyPressEvent(self, event):
        if event.key() == Qt.Key_Escape:
            self.close()
        super(MusicListWin, self).keyPressEvent(event)  

def run():
    app = QApplication(sys.argv)
    gui = MusicListWin()
    gui.show()
    sys.exit(app.exec_())

if __name__ == "__main__":
    run()
