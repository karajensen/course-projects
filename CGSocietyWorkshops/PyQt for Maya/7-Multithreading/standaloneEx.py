import standaloneSetup
import maya.standalone

print "imported maya.standalone"

maya.standalone.initialize()
print "Maya Standalone Initialized"

import maya.cmds as cmds
cmds.polySphere(n="extraClass")

cmds.file( rename=r'C:\Users\Malcolm\Dropbox\cgWorkshopsUpload\hour6\code\mayaStandalone\sphereFile2.ma' )
cmds.file( save=True, type='mayaAscii', f=True )
print "File Saved"
