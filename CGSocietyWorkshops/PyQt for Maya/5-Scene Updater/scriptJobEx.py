import maya.cmds as cmds
for job in cmds.scriptJob(lj=True):
    print job
    
def printVal():
    print "Hey yo, this actually worked!!"   

#Either one works. 
val = cmds.scriptJob( kws=True, attributeChange=["pSphere1.translateX", printVal])    
#val = cmds.scriptJob( attributeChange=["pSphere1.translateX", printVal])    

cmds.scriptJob(kill=val)
