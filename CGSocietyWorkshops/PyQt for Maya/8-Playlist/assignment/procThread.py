from PyQt4.QtCore import *
from PyQt4.QtGui import *
import subprocess

class ProcThread(QThread):
    def __init__(self, proc):
        super(ProcThread, self).__init__()
        self.proc = proc
        
    def __del__(self):
        self.wait()
    
    def run(self):
        print "about to Run"
        self.p = subprocess.Popen(self.proc, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        self.p.wait()
        print "I just ran"
        
if __name__ == "__main__":
    thread = ProcThread(["python", r"C:\Users\Malcolm\Dropbox\cgWorkshopsUpload\hour5\code\assignmentFiles\assignmentFileReader_1.pyw"])
    thread.start()
