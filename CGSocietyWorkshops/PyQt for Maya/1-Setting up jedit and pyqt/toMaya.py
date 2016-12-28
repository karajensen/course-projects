import socket, os, tempfile
from org.gjt.sp.jedit import Macros

#Grabs the currently selected text or else grabs the document
curText = (textArea.getSelectedText())
if not curText:
    curText = (textArea.getText())

#Only goes forward if there is some text to work with
if curText:
    #Strips off the extension from the name of the file to have a module name
    scriptBuffer = view.getBuffer()
    scriptName = scriptBuffer.getName()
    module = os.path.splitext(scriptName)[0]
    
    #Creates a temp directory
    #Change this appropriately if working on OSX
    tempDir = tempfile.gettempdir()
    if not os.path.isdir(tempDir):
        os.makedirs(tempDir)

    #Creates a temp file where the text is put
    tempFile = os.path.join(tempDir, scriptName)
    fObj = open(tempFile, 'w')
    fObj.write(curText)
    fObj.close()

    #Makes a socket and tries to connect it into maya
    mayaSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        mayaSocket.connect(('localhost', 6000))
    except:
        Macros.message(view, 'Please Open the socket in Maya and retry.')

    #Creates the code string to hand to maya
    #It's doing the following
    #if module in dir():
    #   reload(module)
    #else:
    #   import module
    cmd = 'python("if \\"' + module + '\\" in dir():'
    cmd += 'reload(' + module + ');\\nelse:' 
    cmd += 'import ' + module + '");'    
    
    #Sends over the code and closes the socket
    mayaSocket.send(cmd)
    mayaSocket.close()
