#An example of how you would actually run the final UIs from your maya shelf.
import sys

#Change this to your actual folder
addFolder = r"C:\Users\Malcolm\Dropbox\cgWorkshopsUpload\hour4\code"

#Checks if it's in the sys.path
if  not in sys.path:
    #You can either add to the end with append or the beginning with insert
    #sys.path.append(addFolder)
    sys.path.insert(0, addFolder)
    
#Import the proper module and use the proper mayaRun function  
import fileReader_1
fileReader_1.mayaRun()
