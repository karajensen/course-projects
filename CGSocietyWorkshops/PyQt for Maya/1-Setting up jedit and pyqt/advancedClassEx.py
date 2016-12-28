class Vehicle(object):
    def __init__(self):
        super(Vehicle, self).__init__()
        self.curSpeed = 5.0
        print "Created Vehicle"
    
    def goFaster(self, val=10):
        self.curSpeed = self.curSpeed*val
        print self.curSpeed
        
class Plane(Vehicle):
    def __init__(self):
        super(Plane, self).__init__()
        self.__alt = 10000.0
        print "Created Plane"
        
    def getAlt(self):
        return self.__alt
        
    def setAlt(self, val):
        if type(val) == type(1.0):
            self.__alt = val
        else:
            print "Please set alt to a Float value"
    
    alt = property(getAlt, setAlt)
       
class Jet(Plane):
    def __init__(self):
        super(Jet, self).__init__()
        print "Created Jet"

dir(jet)

jet = Jet()
jet2 = Jet()

jet2.alt = 23123.3
jet2.alt

jet2.getAlt()
jet2.setAlt(50)

jet.alt = "ahasd"

plane = Plane()
plane2 = Plane()

plane.curSpeed
plane2.curSpeed
plane2.goFaster(-5)

help(plane)
