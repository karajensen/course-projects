import cPickle, os

#Creating a basic newList and then one with a lot of elements to show the difference
#in file size between ascii and binary
#newList = [1,2,3,4, "information", {"sub":"Dir"}]
newList = range(10000)

#Important thing here is to make sure you're set to "w" to write into the file
fObj = open(os.path.join(os.path.dirname(__file__), "pickleFile.pfa"), "w")
#Ascii save of the file by default
cPickle.dump(newList, fObj)
#Always make sure your file gets closed or you could have issues later
fObj.close()

fObj = open(os.path.join(os.path.dirname(__file__), "pickleFile_binary.pfb"), "w")
#By changing the protocol at the end this file will save in binary format
cPickle.dump(newList, fObj, 2)
fObj.close()