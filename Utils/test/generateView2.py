import json
import os
import math
import numpy

def line(text,level=0):
    s=""
    for i in range(level):
        s+="  "
    s+=text+"\n"
    return s
    
def color(absId):
    if absId<=6:
        return '0. 0.15 0.6'
    if absId==11 or absId==13 or absId==15:
        return '0.1 0.3 0.2'
    if absId>20 and absId<30:
        return '0. 0.4 0.'
    if absId>=100 and absId<400 or absId>=1000 and absId<4000:
        return '0.3 0.2 0.'
        
    if absId>=400 and absId<500 or absId>=4000 and absId<5000:
        return '0.3 0.4 0.'
    if absId>=500 and absId<600 or absId>=5000 and absId<6000:
        return '0.4 0. 0.2'
    if absId>=1000000:
        return '0.6 0.6 0.6'
    
    return '0.2 0.2 0.2'
    
def colorSpec(absId):
    if absId<=6:
        return '0. 0.2 0.3'
    if absId==11 or absId==13 or absId==15:
        return '0.15 0.4 0.3'
    if absId>20 and absId<30:
        return '0. 0.4 0.'
    if absId>=100 and absId<400 or absId>=1000 and absId<4000:
        return '0.3 0.2 0.'
        
    if absId>=400 and absId<500 or absId>=4000 and absId<5000:
        return '0.3 0.4 0.'
    if absId>=500 and absId<600 or absId>=5000 and absId<6000:
        return '0.4 0. 0.2'
    if absId>=1000000:
        return '0.3 0.3 0.3'
    
    return '0.2 0.2 0.2'
    
def colorEmit(absId):
    if absId<=6:
        return '0. 0.1 0.3'
    if absId==11 or absId==13 or absId==15:
        return '0.0 0.3 0.2'
    if absId>20 and absId<30:
        return '0. 0.2 0.'
    if absId>=100 and absId<400 or absId>=1000 and absId<4000:
        return '0.1 0.2 0.'
        
    if absId>=400 and absId<500 or absId>=4000 and absId<5000:
        return '0.3 0.4 0.'
    if absId>=500 and absId<600 or absId>=5000 and absId<6000:
        return '0.3 0. 0.1'
    if absId>=1000000:
        return '0.2 0.2 0.2'
    
    return '0.2 0.2 0.2'


class Particle:
    def __init__(self,cfgDict,mother=None):
        self._cfgDict = cfgDict
        self._p = float(self._cfgDict["p"])
        self._pdg = str(self._cfgDict["pdg"])
        self._status = str(self._cfgDict["status"])
        #not phi defined in direction of LHC center
        #cylinder by default oriented along y instead
        self._phi = 0.5*math.pi-float(self._cfgDict["phi"])
        self._eta = float(self._cfgDict["eta"])#-0.5*math.pi
        self._radius = 0.015+math.log(self._p+1)*0.1
        #self._length = 0.5+math.log(self._p+1.)*0.1
        self._theta = numpy.sign(self._eta)*(0.5*math.pi-2*math.atan(math.exp(-math.fabs(self._eta))))
        self._length = None
        self._scale = 3.
        self._mothers = []
        
    def addMother(self,mother):
        self._mothers.append(mother)
        

        
    def calculateCoordinates(self):
        if (self._length==None):
            self.calculateLevel()
        self._x = self._length*self._scale*math.sin(0.5*math.pi-self._theta)*math.cos(0.5*math.pi+self._phi)
        self._y = self._length*self._scale*math.sin(0.5*math.pi-self._theta)*math.sin(0.5*math.pi+self._phi)
        self._z = self._length*self._scale*math.cos(0.5*math.pi-self._theta)
            
    def writeLocalTransform(self,insert):
        s =""
        '''
        s += line("<Transform rotation='0 0 1 "+str(self._phi)+"\'>",1)
        s += line("<Transform rotation='1 0 0 "+str(self._theta)+"\'>",1)
        #always translate out of center
        s += line("<Transform translation='0 "+str(self._length*self._scale)+" 0'>",1)
        s += insert
        s += line("</Transform>",1)
        s += line("</Transform>",1)
        s += line("</Transform>",1)
        '''
        s += line("<Transform translation=\'"+str(self._x)+" "+str(self._y)+" "+str(self._z)+"\'>",1)
        s += insert
        s += line("</Transform>",1)
        
        return s
        
    def calculateLevel(self):
        if self._length!=None:
            return self._length #cache
        if len(self._mothers)==0:
            self._length = 1
        else:
            self._length = max(map(lambda m:m.calculateLevel(),self._mothers))+1
        return self._length
        
    def write(self):
        if (self._length==None):
            self.calculateLevel()
        self.calculateCoordinates()
        
        s =""
        s += line("<Shape>",1)
        s += line("<Sphere radius=\'"+str(self._radius)+"\'/>",2)
        s += line("<Appearance>",2)
        s += line("<Material diffuseColor='"+color(abs(int(self._pdg)))+"' shininess='0.06' specularColor='"+colorSpec(abs(int(self._pdg)))+"' emissiveColor='"+colorSpec(abs(int(self._pdg)))+"'/>",3)
        s += line("</Appearance>",2)
        s += line("</Shape>",1)
        
        s += line("<Billboard axisOfRotation='0 0 0'>",1)
        s += line("<Transform translation='"+str(self._radius*1.3)+" "+str(self._radius*1.3)+" "+str(self._radius*1.3)+"'>",1)
        s += line("<Shape>",1)
        s += line("<Text string='\""+self._pdg+"\"'>",1)
        s += line("<FontStyle family='\"SANS\"' size='"+str(self._radius*1.3)+"' justify='\"MIDDLE\" \"MIDDLE\"' style='BOLD'/>",1)
        s += line("</Text>",1)
        s += line("<Appearance>",1)
        s += line("<Material  ambientIntensity='0.25' diffuseColor='"+color(abs(int(self._pdg)))+"' shininess='0.06' specularColor='"+colorSpec(abs(int(self._pdg)))+"' emissiveColor='"+colorSpec(abs(int(self._pdg)))+"'/>",1)
        s += line("</Appearance>",1)
        s += line("</Shape>",1)
        s += line("</Transform>",1)
        s += line("</Billboard>",1)
        
        s += line("<Billboard axisOfRotation='0 0 0'>",1)
        s += line("<Transform translation='-"+str(self._radius*1.3)+" -"+str(self._radius*1.3)+" "+str(self._radius*1.3)+"'>",1)
        s += line("<Shape>",1)
        s += line("<Text string='\""+self._status+"\"'>",1)
        s += line("<FontStyle family='\"SANS\"' size='"+str(self._radius*1.3)+"' justify='\"MIDDLE\" \"MIDDLE\"' style='BOLD'/>",1)
        s += line("</Text>",1)
        s += line("<Appearance>",1)
        s += line("<Material  ambientIntensity='0.25' diffuseColor='"+color(abs(int(self._pdg)))+"' shininess='0.06' specularColor='"+colorSpec(abs(int(self._pdg)))+"' emissiveColor='"+colorSpec(abs(int(self._pdg)))+"'/>",1)
        s += line("</Appearance>",1)
        s += line("</Shape>",1)
        s += line("</Transform>",1)
        s += line("</Billboard>",1)
        
        s = self.writeLocalTransform(s)

        return s
        
class Relation:
    def __init__(self,mother,daughter):
        self._mother = mother
        self._daughter = daughter
        self._radius = 0.01+math.log(self._mother._p+1)*0.01
        
    def calculateCoordinates(self):
        self._mother.calculateCoordinates()
        self._daughter.calculateCoordinates()
        
        dx = self._daughter._x - self._mother._x
        dy = self._daughter._y - self._mother._y
        dz = self._daughter._z - self._mother._z
        dr = math.sqrt(dx**2+dy**2)
        deta = self._daughter._eta - self._mother._eta
        
        self._length = math.sqrt(dx**2+dy**2+dz**2)
        self._phi = -(0.5*math.pi-math.atan2(dy,dx))
        #between y and z (from y)
        self._theta = math.atan2(dz,dr)
        
    def writeLocalTransform(self,insert):
        s =""
        s += line("<Transform rotation='0 0 1 "+str(self._phi)+"\'>",1)
        s += line("<Transform rotation='1 0 0 "+str(self._theta)+"\'>",1)
        #always translate out of center
        s += line("<Transform translation='0 "+str(self._length*0.5)+" 0'>",1)
        s += insert
        s += line("</Transform>",1)
        s += line("</Transform>",1)
        s += line("</Transform>",1)
        return s
        
            
        
    def write(self):
        s =""
        s += line("<Shape>",1)
        s += line("<Cylinder radius=\'"+str(self._radius)+"\' height='"+str(self._length)+"'/>",2)
        s += line("<Appearance>",2)
        s += line("<Material diffuseColor='"+color(abs(int(self._mother._pdg)))+"' shininess='0.06' specularColor='"+colorSpec(abs(int(self._mother._pdg)))+"' emissiveColor='"+colorSpec(abs(int(self._mother._pdg)))+"'/>",3)
        s += line("</Appearance>",2)
        s += line("</Shape>",1)
        s = self.writeLocalTransform(s)
        s = self._mother.writeLocalTransform(s)

        return s
        
        
class Beamline:
    def __init__(self,length,color,side=1):
        self._radius = length*0.05
        self._length = length
        self._color = color
        self._scale = 3.
        self._side = side

    def writeLocalTransform(self,insert):
        s =""
        s += line("<Transform rotation='1 0 0 "+str(self._side*1.570796)+"'>",1)
        #always translate out of center
        s += line("<Transform translation='0 "+str(-self._scale*self._length*0.5)+" 0'>",1)
        s += insert
        s += line("</Transform>",1)
        s += line("</Transform>",1)
        return s
        
    def write(self):
        s =""
        s += line("<Shape>",1)
        s += line("<Cone bottomRadius=\'"+str(self._radius)+"\' height='"+str(self._length*self._scale)+"'/>",2)
        s += line("<Appearance>",2)
        s += line("<Material diffuseColor='"+self._color+"' shininess='0.01' specularColor='0.5 0.5 0.5' emissiveColor='0.15 0.15 0.15'/>",3)
        s += line("</Appearance>",2)
        s += line("</Shape>",1)
        
        s = self.writeLocalTransform(s)

        return s
        
        
class GenJet:
    def __init__(self,cfgDict,length):
        self._cfgDict = cfgDict
        self._p = float(self._cfgDict["p"])
        #not phi defined in direction of LHC center
        #cylinder by default oriented along y instead
        self._phi = 0.5*math.pi-float(self._cfgDict["phi"])
        self._eta = float(self._cfgDict["eta"])#-0.5*math.pi
        self._length = length
        self._radius = self._length*float(self._cfgDict["distance"])*0.5
        self._depth = (0.01+math.log(self._p+1)*0.1)*self._length
        self._theta = numpy.sign(self._eta)*(0.5*math.pi-2*math.atan(math.exp(-math.fabs(self._eta))))
        self._scale = 3.
        
                
    def writeLocalTransform(self,insert):
        s =""
        s += line("<Transform rotation='0 0 1 "+str(self._phi)+"\'>",1)
        s += line("<Transform rotation='1 0 0 "+str(self._theta)+"\'>",1)
        #always translate out of center
        s += line("<Transform translation='0 "+str((self._depth*0.5+self._length)*self._scale)+" 0'>",1)
        s += insert
        s += line("</Transform>",1)
        s += line("</Transform>",1)
        s += line("</Transform>",1)
        
        return s
        

    def write(self):

        s =""
        s += line("<Shape>",1)
        s += line("<Cylinder radius=\'"+str(self._radius)+"\' height='"+str(self._depth)+"'/>",2)
        s += line("<Appearance>",2)
        s += line("<Material diffuseColor='0.2 0 0' shininess='0.03' specularColor='0.3 0.1 0.1' emissiveColor='0.2 0 0'/>",3)
        s += line("</Appearance>",2)
        s += line("</Shape>",1)
        
        
        s = self.writeLocalTransform(s)

        return s


class X3dEvent:
    def __init__(self):
        self._shapes = []
        
    def addShape(self,shape):
        self._shapes.append(shape)
        
        
    def write(self):
        s='''<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE X3D PUBLIC "ISO//Web3D//DTD X3D 3.3//EN" "http://www.web3d.org/specifications/x3d-3.3.dtd">
<X3D profile='Interchange' version='3.3'  xmlns:xsd='http://www.w3.org/2001/XMLSchema-instance' xsd:noNamespaceSchemaLocation =' http://www.web3d.org/specifications/x3d-3.3.xsd '>
<Scene>
    <Background skyColor='0 0 0'/>
    <Viewpoint description='View' orientation='-0.747 -0.624 -0.231 1.05' position='-1.81 2.12 2.59'/>
    <Shape>
    <Sphere radius='0.1'/>
    <Appearance>
    <Material diffuseColor='.7 .1 .1' emissiveColor='0.13 0.33 0'/>
    </Appearance>
    </Shape> 
    
    <Transform rotation='0 0 1 1.570796'>
    <Transform translation='0 0.5 0'>
    <Shape>
    <Cylinder radius='0.01' height='2.0'/>
    <Appearance>
      <Material diffuseColor='.7 .1 .1' emissiveColor='.0 0.43 0'/>
    </Appearance>
  </Shape>
  <Billboard axisOfRotation='0 0 0'> 
    <Transform translation='0.1 0.1 0'>
    <Shape>
        <Text string='"X"'>
            <FontStyle family='"SANS"' size='0.1' justify='"MIDDLE" "MIDDLE"' style='BOLD'/>
        </Text>
        <Appearance>
            <Material  ambientIntensity='0.25' diffuseColor='0.6 0.6 0.6' shininess='0.06' specularColor='0.177935 0.249369 0.229278'/> 
        </Appearance>
    </Shape>
    </Transform>
    </Billboard>
  
  </Transform>
  </Transform>
  
  <Transform translation='0 0.5 0'>
  <Shape>
    <Cylinder radius='0.01' height='2.0'/>
    <Appearance>
      <Material diffuseColor='.7 .1 .1' emissiveColor='.0 0.1 0.6'/>
    </Appearance>
  </Shape>
  <Billboard axisOfRotation='0 0 0'> 
    <Transform translation='0.1 0.1 0'>
    <Shape>
        <Text string='"Y"'>
            <FontStyle family='"SANS"' size='0.1' justify='"MIDDLE" "MIDDLE"' style='BOLD'/>
        </Text>
        <Appearance>
            <Material  ambientIntensity='0.25' diffuseColor='0.6 0.6 0.6' shininess='0.06' specularColor='0.177935 0.249369 0.229278'/> 
        </Appearance>
    </Shape>
    </Transform>
    </Billboard>
  </Transform>
  
  <Transform rotation='1 0 0 1.570796'>
  <Transform translation='0 0.5 0'>
  <Shape>
    <Cylinder radius='0.01' height='2.0'/>
    <Appearance>
      <Material diffuseColor='.7 .1 .1' emissiveColor='.4 0 0.5'/>
    </Appearance>
  </Shape>
  <Billboard axisOfRotation='0 0 0'> 
    <Transform translation='0.1 0.1 0'>
    <Shape>
        <Text string='"Z"'>
            <FontStyle family='"SANS"' size='0.1' justify='"MIDDLE" "MIDDLE"' style='BOLD'/>
        </Text>
        <Appearance>
            <Material  ambientIntensity='0.25' diffuseColor='0.6 0.6 0.6' shininess='0.06' specularColor='0.177935 0.249369 0.229278'/> 
        </Appearance>
    </Shape>
    </Transform>
    </Billboard>
  </Transform>
  </Transform>

'''
        for shape in self._shapes:
            s+=shape.write()
        s+='''</Scene>
</X3D> 
'''
        return s


data = json.load(open('test.txt'))


for ievent,event in enumerate(data["Events"]):
    print ievent
    f = open("event_"+str(ievent+1)+".x3d","w")
    x3d = X3dEvent()
    particleShapes = {}
    relations = []
    for cfg in event["Particles"]:
        particleShapes[cfg["index"]]= Particle(cfg)
    noAncestors = range(len(particleShapes))
    for relation in event["Relations"]:
        mother = particleShapes[relation["index"]]
        for daughterIndex in relation["daughters"]:
            daughter = particleShapes[daughterIndex]
            daughter.addMother(mother)
            relations.append(Relation(mother,daughter))
            
    for shape in particleShapes.values():
        shape.calculateCoordinates()
        x3d.addShape(shape)
        
    for relation in relations:
        relation.calculateCoordinates()
        x3d.addShape(relation)
        
    #add beamline
    lengthMax = max(map(lambda s: s._length,particleShapes.values()))
    beamline_pos = Beamline(1.25*lengthMax,'0.1 0.1 0.2',1)
    x3d.addShape(beamline_pos)
    beamline_neg = Beamline(1.25*lengthMax,'0.1 0.1 0.2',-1)
    x3d.addShape(beamline_neg)
    
    for genJet in event["GenJets"]:
        x3d.addShape(GenJet(genJet,1.05*lengthMax))
    
    f.write(x3d.write())
    f.close()
    
    
    
