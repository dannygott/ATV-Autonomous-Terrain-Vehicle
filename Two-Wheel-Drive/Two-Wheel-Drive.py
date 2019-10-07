import logging
import json
import time
import serial
from websocket_server import WebsocketServer
import subprocess as sp

values = {"shiftVal": 0, "rpm": 0}

ser = serial.Serial(
    #Create serial object to communicate with arduino
    port='/dev/ttyACM0',
    baudrate=2000000,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout = .1
)
ser.isOpen()

time.sleep(2) # wait for arduino to boot

def new_client(client, server):
	server.send_message_to_all("Controller connected")
def new_msg(client, server, message):
    messageObj = json.loads(message)
    writeToArduino(messageObj)

    print("LEFT MOTOR : " + str(messageObj['leftMotor']))
    print("RIGHT MOTOR : " + str(messageObj['rightMotor']))

    
     
def stripArduinoMessage(message):
    return str(message).strip("b'").strip("\\r\\n'")

def writeToArduino(messageVals):
    ser.write(bytes(str(int(messageVals['leftMotor']))+ ',', 'utf-8'))
    ser.write(bytes(str(int(messageVals['rightMotor']))+ ',', 'utf-8'))

def rpmCalc(pulseTime):
    pulseTime = int(pulseTime)
    if pulseTime == 0:
        rpm = 0
    else:
        frequency = 1000 / pulseTime
        rpm = frequency * 60 / 600
    return rpm


server = WebsocketServer(13254, host='127.0.0.1')
server.set_fn_new_client(new_client)
server.set_fn_message_received(new_msg)
server.run_forever()