import logging
import json
import time
import serial
from websocket_server import WebsocketServer

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

    encoderPoz = stripArduinoMessage(ser.readline())
    shiftVal = stripArduinoMessage(ser.readline())
    timeBetweenPulse = stripArduinoMessage(ser.readline())
    rpm = rpmCalc(timeBetweenPulse)

    print("THROTTLE VAL : " + str(messageObj['throttle']*180))
    print("SHIFT VAL : " + shiftVal) 
    print("PULSE TIME : " + timeBetweenPulse)
    print("ENCODER POZ : " + encoderPoz)
    print("RPM: " + str(rpm))
    print()

    values["shiftVal"] = shiftVal
    values["rpm"] = rpm
    values_json = json.dumps(values)
    server.send_message_to_all(values_json)
    
     
    
def shiftDetermine(message):
    shiftup = int(message['shiftup'])
    shiftdown = int(message['shiftdown'])
    if shiftup + shiftdown < 2:
        if shiftup > 0:
            return 2
        elif shiftdown > 0:
            return 1
        else:
            return 0
    return 0

def stripArduinoMessage(message):
    return str(message).strip("b'").strip("\\r\\n'")

def writeToArduino(messageVals):
    ser.write(bytes(str(int(messageVals['throttle']*180))+ ',', 'utf-8'))
    ser.write(bytes(str(shiftDetermine(messageVals))+ ',', 'utf-8'))

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