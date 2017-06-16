#====================================================================================
# Scripting and Tool Dev in Maya - Week 6 - Kara Jensen
#====================================================================================

import os
import random
import maya.cmds as cmds

# Script 1 ------------------------------------------------------------------------
# Write a method that takes a path as an arg and return the content of that path
def script1(path):
    if os.path.exists(path):
        if os.path.isfile(path):
            return path
        else:
            return os.listdir(path)

print script1("D:/")
print script1("D:/bookmarks.html")

# Script 2 ------------------------------------------------------------------------
# Modify the script so that now it will print the content of that path given as the 
# arg in the method and write behind or before each item if its a file or a directory.
def script2(path):
    if os.path.exists(path):
        if os.path.isfile(path):
            return "FILE: " + path
        else:
            allFiles = os.listdir(path)
            for i in range(len(allFiles)):
                if os.path.isfile(os.path.join(path, allFiles[i])):
                    allFiles[i] = "FILE: " + allFiles[i]
                else:
                    allFiles[i] = "FOLDER: " + allFiles[i]
            return allFiles

print script2("D:/")
print script2("D:/bookmarks.html")

# Script 3 ------------------------------------------------------------------------
# Write a method that will randomize the vertices on the mesh (polygonal) objects
# that the user has selected.
def script3():
    selected = cmds.ls(sl=True)
    if len(selected) == 1:
        vertNumber = cmds.polyEvaluate(v=True)
        for i in range(vertNumber):
            x = random.uniform(-1.0,1.0)
            y = random.uniform(-1.0,1.0)
            z = random.uniform(-1.0,1.0)
            vtx = selected[0] + ".vtx[" + str(i) + "]"  
            cmds.move(x, y, z, vtx, r=True)
    else:
        cmds.warning("Can only be used on one object")

cmds.polySphere( radius=4 )
script3()