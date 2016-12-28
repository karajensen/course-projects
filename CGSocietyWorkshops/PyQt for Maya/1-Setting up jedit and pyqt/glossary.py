import os, cPickle

#Change this to the folder that you want to save all of this with
writeDir = r"C:\Users\Malcolm\Dropbox\cgWorkshopsUpload\hour1\code\glossary"

class Glossary(object):
    def __init__(self, glosFile=""):
        """
        Base init that checks for a file and loads that as self.glos
        """
        super(Glossary, self).__init__()
        
        #Looks to see if a glosFile has been handed over
        #Creates one if there isn't
        if glosFile:
            self.glosFile = glosFile
        else:
            self.glosFile = os.path.join(writeDir, "base.glos")
        
        #If the file exists on disk then it's open
        #Otherwise another local glos is created
        if os.path.exists(self.glosFile):
            fObj = open(self.glosFile)
            self.glos = cPickle.load(fObj)
            fObj.close()
            print "Glossary Opened"
        else:    
            self.glos = {}
        
    def addTerm(self, term, definition):
        """Add a term to the glossary"""
        print "\tAdding " + term
        self.glos[term] = definition
        
    def removeTerm(self, term):
        """Remove a term from the glossary"""
        print "Removing " + term
        #Checks to see if the term is in the keys
        if term in self.glos.keys():
            self.glos.pop(term)
        
    def printTerms(self):
        """Print terms to the terminal"""
        for term in sorted(self.glos.keys()):
            print term
            print "\t" + self.glos[term]
    
    def printGlos(self, printFile=""):
        """Print the glossary to a file"""
        if not printFile:
            printFile = os.path.join(writeDir, "glossary.txt")
        fObj = open(printFile, "w")
        for term in sorted(self.glos.keys()):
            fObj.write(term + "\n")
            fObj.write("\t" + self.glos[term] + "\n")
        fObj.close()
        print "Glossary Printed"

    def saveGlos(self):
        """Save the glossary out to a pickle file"""
        fObj = open(self.glosFile, "w")
        cPickle.dump(self.glos, fObj)
        fObj.close()
        print "Glossary Saved"
        
#instance of the glossary
glos = Glossary()

#Adds terms
glos.addTerm("students", "aweosome people")
glos.addTerm("malcolm", "teacher of the PyQt class you're watching")
glos.addTerm("a", "a letter in the alphabet")

#removes term 'a'
glos.removeTerm("a")

#prints the terms
glos.printTerms()

#writes another file to disk
glos.printGlos()

#saves the glos
glos.saveGlos()

#prints the actual object
print glos
