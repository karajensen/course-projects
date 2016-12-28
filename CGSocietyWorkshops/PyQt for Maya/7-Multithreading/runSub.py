import random, sys, os
#if random.random() > .5:
#    aslkdj

folder =  str(os.path.dirname(sys.argv[0]))
fileNum =  str(sys.argv[1])

import standaloneSetup
import maya.standalone

print "Initializing Maya Standalone"    
maya.standalone.initialize()
print "Maya Standalone Initialized"
import maya.cmds as cmds

sph = cmds.polySphere( n = "sphere"+fileNum )

cmds.file( rename=os.path.join(folder, "sphereTest{0}.ma".format(fileNum) ) )
cmds.file( save=True, type='mayaAscii', f=True )
print "File Saved"
