
def check_type(value, type_to_check):
    if type(value) is not type_to_check:
        raise ValueError(("value " + str(value) + " needs to be of type " + str(type_to_check)))
    return value

class MythicalCreature(object):
    "Generic mythical creature base class"
    def __init__(self,name,species,location,power="None"):
        super(MythicalCreature,self).__init__()
        self.Name = name
        self.Species = species
        self.Location = location
        self.Power = power
        self.Purpose = "None"
        self.__hybrid = (len(species) > 1)
    @property
    def Name(self):
        return self.__name
    @Name.setter
    def Name(self, name):
        self.__name = check_type(name,str)
    @property
    def Species(self):
        return self.__species
    @Species.setter
    def Species(self, species):
        self.__species = check_type(species,list)
    @property
    def Location(self):
        return self.__location
    @Location.setter
    def Location(self, location):
        self.__location = check_type(location,str)
    @property
    def Power(self):
        return self.__power
    @Power.setter
    def Power(self, power):
        self.__power = check_type(power,str)
    @property
    def Purpose(self):
        return self.__purpose
    @Purpose.setter
    def Purpose(self, purpose):
        self.__purpose = check_type(purpose,str)
    @property
    def Hybrid(self):
        return self.__hybrid

class FlyingCreature(MythicalCreature):
    "Mythical creature that can fly"
    def __init__(self,hasWings,name,species,location,power="None"):
        super(FlyingCreature,self).__init__(name,species,location,power)
        self.HasWings = hasWings
        self.AirSpeed = 10.0 #in m/s
    @property
    def HasWings(self):
        return self.__hasWings
    @HasWings.setter
    def HasWings(self, hasWings):
        self.__hasWings = check_type(hasWings,bool)
    @property
    def AirSpeed(self):
        return self.__airSpeed
    @AirSpeed.setter
    def AirSpeed(self, airSpeed):
        self.__airSpeed = check_type(airSpeed,float)

class FeatheredFlyingCreature(FlyingCreature):
    "Mythical creature that can fly and has feathers"
    def __init__(self,hasWings,name,species,location,power="None"):
        super(FeatheredFlyingCreature,self).__init__(hasWings,name,species,location,power)
        self.FeathersDoDamage = False
        self.FeathersRegrow = False
    @property
    def FeathersDoDamage(self):
        return self.__feathersDoDamage
    @FeathersDoDamage.setter
    def FeathersDoDamage(self, feathersDoDamage):
        self.__feathersDoDamage = check_type(feathersDoDamage,bool)
    @property
    def FeathersRegrow(self):
        return self.__feathersRegrow
    @FeathersRegrow.setter
    def FeathersRegrow(self, feathersRegrow):
        self.__feathersRegrow = check_type(feathersRegrow,bool)

class ScaledFlyingCreature(FlyingCreature):
    "Mythical creature that can fly and is scaled"
    def __init__(self,hasWings,name,species,location,power="None"):
        super(ScaledFlyingCreature,self).__init__(hasWings,name,species,location,power)
        self.ScalesProvideArmour = True
    @property
    def ScalesProvideArmour(self):
        return self.__scalesProvideArmour
    @ScalesProvideArmour.setter
    def ScalesProvideArmour(self, scalesProvideArmour):
        self.__scalesProvideArmour = check_type(scalesProvideArmour,bool)

class SpiritFlyingCreature(FlyingCreature):
    "Mythical creature that can fly and has no physical body"
    def __init__(self,hasWings,name,species,location,power="None"):
        super(SpiritFlyingCreature,self).__init__(hasWings,name,species,location,power)
        self.CanBecomeInvisible = True
    @property
    def CanBecomeInvisible(self):
        return self.__canBecomeInvisible
    @CanBecomeInvisible.setter
    def CanBecomeInvisible(self, canBecomeInvisible):
        self.__canBecomeInvisible = check_type(canBecomeInvisible,bool)

class LandCreature(MythicalCreature):
    "Mythical creature that is land-based"
    def __init__(self,hasHooves,hasArms,name,species,location,power="None"):
        super(LandCreature,self).__init__(name,species,location,power)
        self.HasArms = hasArms
        self.HasHooves = hasHooves
        self.TravelSpeed = 10.0 #in m/s
    @property
    def HasArms(self):
        return self.__hasArms
    @HasArms.setter
    def HasArms(self, hasArms):
        self.__hasArms = check_type(hasArms,bool)
    @property
    def HasHooves(self):
        return self.__hasHooves
    @HasHooves.setter
    def HasHooves(self, hasHooves):
        self.__hasHooves = check_type(hasHooves,bool)
    @property
    def TravelSpeed(self):
        return self.__travelSpeed
    @TravelSpeed.setter
    def TravelSpeed(self, travelSpeed):
        self.__travelSpeed = check_type(travelSpeed,float)

class QuadrupedLandCreature(LandCreature):
    "Land-based Mythical creature that has four legs"
    def __init__(self,hasHooves,hasArms,name,species,location,power="None"):
        super(QuadrupedLandCreature,self).__init__(hasHooves,hasArms,name,species,location,power)
        self.CanBeRidden = True
    @property
    def CanBeRidden(self):
        return self.__canBeRidden
    @CanBeRidden.setter
    def CanBeRidden(self, canBeRidden):
        self.__canBeRidden = check_type(canBeRidden,bool)

class BipedalLandCreature(LandCreature):
    "Land-based Mythical creature that has two legs"
    def __init__(self,hasHooves,hasArms,name,species,location,power="None"):
        super(BipedalLandCreature,self).__init__(hasHooves,hasArms,name,species,location,power)
        self.CanRide = True
    @property
    def CanRide(self):
        return self.__canRide
    @CanRide.setter
    def CanRide(self, canRide):
        self.__canRide = check_type(canRide,bool)
        
phoenix  = FeatheredFlyingCreature(hasWings=True,name="Phoenix",species=["Bird"],location="Volcano",power="Fire")
griffin  = FeatheredFlyingCreature(hasWings=True,name="Griffin",species=["Bird","Feline"],location="Mountain")
harpy    = FeatheredFlyingCreature(hasWings=True,name="Harpy",species=["Bird","Human"],location="Woods")
dragon   = ScaledFlyingCreature(hasWings=True,name="Dragon",species=["Reptile"],location="Mountain",power="Fire")
genie    = SpiritFlyingCreature(hasWings=False,name="Genie",species=["Ghost"],location="Lamp",power="Magic")
centuar  = QuadrupedLandCreature(hasHooves=True,hasArms=True,name="Centaur",species=["Equine","Human"],location="Mountain")
unicorn  = QuadrupedLandCreature(hasHooves=True,hasArms=False,name="Unicorn",species=["Equine"],location="Woods",power="Magic")
minotaur = BipedalLandCreature(hasHooves=True,hasArms=True,name="Minotaur",species=["Human","Bull"],location="Labyrinth")

phoenix.FeathersRegrow = True
phoenix.FeathersDoDamage = True
phoenix.Purpose = "Symbol of resurrection"
griffin.Purpose = "Guards Treasure"
genie.Purpose = "Grants Wishes"
harpy.Purpose = "Steals objects"
centuar.Purpose = "Lives in community"
unicorn.Purpose = "Symbol of purity"
minotaur.Purpose = "Kills virgins"
	
if dragon.Hybrid is True:
    dragon.Purpose = "Burns Villages"
else:
    dragon.Purpose = "Guards Treasure"