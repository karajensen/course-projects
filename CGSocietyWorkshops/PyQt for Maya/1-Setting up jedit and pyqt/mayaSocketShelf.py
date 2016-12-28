import maya.cmds as cmds
if not cmds.commandPort('localhost:6000', q=True):
    cmds.commandPort(name='localhost:6000')
