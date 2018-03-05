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


class Particle:
    def __init__(self,cfgDict,mother=None):
        self._cfgDict = cfgDict
        self._p = float(self._cfgDict["p"])
        self._pdg = str(self._cfgDict["pdg"])
        #not phi defined in direction of LHC center
        #cylinder by default oriented along y instead
        self._phi = 0.5*math.pi-float(self._cfgDict["phi"])
        self._eta = float(self._cfgDict["eta"])#-0.5*math.pi
        self._radius = 0.015#-0.005*(self._p/(1.+self._p))
        self._length = 0.5+math.log(self._p+1.)*0.1
        self._theta = numpy.sign(self._eta)*(0.5*math.pi-2*math.atan(math.exp(-math.fabs(self._eta))))
        
        self._mother = mother
        
    def setMother(self,mother):
        if self._mother!=None:
            print "secondary mother: ",mother._pdg
        self._mother = mother
        
    def color(self,absId):
        if absId<=6:
            return '0. 0.1 0.5'
        if absId>20 and absId<30:
            return '0. 0.5 0.'
        if absId>=100 and absId<400 or absId>=1000 and absId<4000:
            return '0.4 0. 0.'
            
        if absId>=400 and absId<500 or absId>=4000 and absId<5000:
            return '0.4 0.2 0.'
        if absId>=500 and absId<600 or absId>=5000 and absId<6000:
            return '0.4 0. 0.2'
        if absId>=1000000:
            return '0.6 0.6 0.6'
        
        return '0.1 0.1 0.1'
            
        
    def writeGlobalTransform(self,insert):
        s = ""
        x = self._length*math.sin(0.5*math.pi-self._theta)*math.cos(0.5*math.pi+self._phi)
        y = self._length*math.sin(0.5*math.pi-self._theta)*math.sin(0.5*math.pi+self._phi)
        z = self._length*math.cos(0.5*math.pi-self._theta)
        #translate
        s += line("<Transform translation=\'"+str(x)+" "+str(y)+" "+str(z)+"\'>",1)
        s += insert
        s += line("</Transform>",1)
        if self._mother!=None:
            s = self._mother.writeGlobalTransform(s)
        return s
        
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
        s += line("<Cylinder radius=\'"+str(self._radius)+"\' height=\'"+str(self._length)+"\'/>",2)
        s += line("<Appearance>",2)
        s += line("<Material diffuseColor='.7 .1 .1' emissiveColor='"+self.color(abs(int(self._pdg)))+"'/>",3)
        s += line("</Appearance>",2)
        s += line("</Shape>",1)
        
        s += line("<Billboard axisOfRotation='0 0 0'>",1)
        s += line("<Transform translation='0.1 0.1 "+str(self._length*0.4)+"'>",1)
        s += line("<Shape>",1)
        s += line("<Text string='\""+self._pdg+"\"'>",1)
        s += line("<FontStyle family='\"SANS\"' size='0.1' justify='\"MIDDLE\" \"MIDDLE\"' style='BOLD'/>",1)
        s += line("</Text>",1)
        s += line("<Appearance>",1)
        s += line("<Material  ambientIntensity='0.25' diffuseColor='"+self.color(abs(int(self._pdg)))+"' shininess='0.06' specularColor='0.177935 0.249369 0.229278'/>",1)
        s += line("</Appearance>",1)
        s += line("</Shape>",1)
        s += line("</Transform>",1)
        s += line("</Billboard>",1)
        
        s = self.writeLocalTransform(s)
        if (self._mother!=None):
            s = self._mother.writeGlobalTransform(s)

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
    for cfg in event["Particles"]:
        particleShapes[cfg["index"]]= Particle(cfg)
    noAncestors = range(len(particleShapes))
    for relation in event["Relations"]:
        mother = particleShapes[relation["index"]]
        for daughterIndex in relation["daughters"]:
            daughter = particleShapes[daughterIndex]
            daughter.setMother(mother)
    
    for shape in particleShapes.values():
        x3d.addShape(shape)
        
    f.write(x3d.write())
    f.close()
    
    
    
