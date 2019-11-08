import numpy as np
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.animation as animation
from rplidar import RPLidar
import threading
import time
import logging
import random
import Queue
lidar = RPLidar('/dev/ttyUSB0')

#Constants for edge detection
#TODO Hone values to proper 
EDGE_SENSITIVITY = 200
MAX_DIST = 600
objectList = []
BUF_SIZE = 100
q = Queue.Queue(BUF_SIZE)
info = lidar.get_info()
print(info)
health = lidar.get_health()
print(health)


class LidarObj:
    def __init__(self):
        self.area = []
        self.areaNp = np.zeros(shape = (1,2))
    def addPoint(self,alist):
        self.area.append(alist)
    def initAreaNp(self):
        self.areaNp = np.array(self.area)
        print(self.areaNp)
        print(type(self.areaNp))
    

def objectProducer():
    for scan in enumerate(lidar.iter_scans()):
        print("amt of objects ",len(objectList))
        if objectList:
            objectList[-1].initAreaNp();
        objectList = []
        inObject = False

        for i in range(len(scan[-1])):
            dist = scan[-1][i][2]
            angle = scan[-1][i][1]
            prevAngle = scan[-1][i][1]

            if dist < MAX_DIST:
                prevDist = scan[-1][i-1][2]
                #abs(dist - prevDist) < EDGE_SENSITIVITY
                if not inObject:
                    #print("Making new obj")
                    inObject = True
                    objectList.append(LidarObj())
                    objectList[-1].addPoint([dist,angle])
            
                else:
                    #print("Adding point to existing object")
                    objectList[-1].addPoint([dist,angle])
            else:
                inObject = False

def objectConsumer():
    #TODO put data analytic stuff in here  
    print("Hello world")
    
def run():
    print("banana phone")

lidar.stop()
lidar.stop_motor()
lidar.disconnect()