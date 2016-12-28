import cPickle, os

#Make sure you're set to "r" so that you read the file
fObj = open(os.path.join(os.path.dirname(__file__), "pickleFile_binary.pfb"), "r")
#Whatever you load into will get completely replace with the data from the file
newList = cPickle.load(fObj)
fObj.close()

#print newList
#for i in newList:
#    print i

#Just a good way to write out a bunch of data. More of a sidenote than anything.
newString = ""
for i in newList:
    newString += str(i) + "\n"
newString += "malcolm just blew your mind"
print newString