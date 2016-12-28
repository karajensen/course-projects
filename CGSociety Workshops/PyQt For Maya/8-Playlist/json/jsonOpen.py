import json, os

fObj = open(os.path.join(os.path.dirname(__file__), "jsonFile.json"), "r")
newList = json.load(fObj)
fObj.close()

print newList
for i in newList:
    print "The artist is {0} and the song is {1}".format(i["artist"], i["song"])