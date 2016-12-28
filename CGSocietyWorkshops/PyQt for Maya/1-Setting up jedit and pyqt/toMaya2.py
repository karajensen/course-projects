import socket, os, tempfile
from org.gjt.sp.jedit import Macros

#Gets the current file buffer
scriptBuffer = view.getBuffer()

#gets the information about the path
scriptName = scriptBuffer.getPath()
path = os.path.dirname(scriptName)

#gets the information about the module
scriptName = scriptBuffer.getName()
module = os.path.splitext(scriptName)[0]

#creates a temporary module name that you can hand code to
tempModule = "mayaToJedit"

#Works with the temp directory
tempDir = tempfile.gettempdir()
if not os.path.isdir(tempDir):
    os.makedirs(tempDir)
    
#Creates the code to put in the tempfile
pathAdd = """import sys
if r'PATH_REPLACE' not in sys.path: 
    sys.path.append(r'PATH_REPLACE')
if 'MODULE_NAME' in dir():
    reload(MODULE_NAME)
else:
    import MODULE_NAME"""
    
#Replaces the important parts of the code
pathAdd = pathAdd.replace("PATH_REPLACE", path)
pathAdd = pathAdd.replace("MODULE_NAME", module)

#Creates the tempfile and stores out the py in the temp dir
tempFile = os.path.join(tempDir, tempModule+".py")
fObj = open(tempFile, 'w')
fObj.write(pathAdd)
fObj.close()    

#Runs the temp dir in maya
#It's doing the following
#if module in dir():
#   reload(module)
#else:
#   import module
cmd = 'python("if \\"' + tempModule + '\\" in dir():'
cmd += 'reload(' + tempModule + ');' 
cmd += '\\nelse: import ' + tempModule + '");' 

#Creates a socket into maya and tries to connect it
mayaSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
try:
    mayaSocket.connect(('localhost', 6000))
except:
    Macros.message(view, 'Please Open the socket in Maya and retry.')

#Sends the command over to maya
mayaSocket.send(cmd)
mayaSocket.close()
