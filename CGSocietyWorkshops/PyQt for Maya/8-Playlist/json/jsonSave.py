import json, os

newList = [{"artist":"malcolm", "song":"n/a"}, {"artist":"acdc", "song":"back in black"}]

fObj = open(os.path.join(os.path.dirname(__file__), "jsonFile.json"), "w")
json.dump(newList, fObj, indent=4)
fObj.close()
