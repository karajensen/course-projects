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
        print "Created Plane"
        
plane = Plane()
plane2 = Plane()

plane.curSpeed
plane2.curSpeed
plane2.goFaster(-5)

help(plane)