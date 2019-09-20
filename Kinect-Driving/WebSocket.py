import logging
import json
import time
import serial
from websocket_server import WebsocketServer
shiftVal = ""


ser = serial.Serial(
    port='/dev/ttyACM1',
    baudrate=2000000,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout = .1
)
ser.isOpen()

time.sleep(2)
def new_client(client, server):
	server.send_message_to_all("Hey all, a new client has joined us")
def new_msg(client, server, message):
    messageObj = json.loads(message)
   
    print(messageObj['throttle']*180)
   # print(bytes(str(int(float(message)*180)) + "\r\n", 'utf-8'))
    ser.write(bytes(str(int(messageObj['throttle']*180))+ ',', 'utf-8'))
    ser.write(bytes(str(shiftDetermine(messageObj))+ ',', 'utf-8'))
    encoderPoz = str(ser.readline())
    encoderPoz = encoderPoz.strip("b'")
    encoderPoz = encoderPoz.strip("\\r\\n'")
    print("ENCODER POZ : " + encoderPoz)
    shiftVal = str(ser.readline())
    shiftVal = shiftVal.strip("b'")
    shiftVal = shiftVal.strip("\\r\\n'")
    print("SHIFT VAL : " + shiftVal) 
    timeBetweenPulse = str(ser.readline())
    print("Time between pulse: " + timeBetweenPulse)
    timeBetweenPulse = timeBetweenPulse.strip("b'")
    timeBetweenPulse = int(timeBetweenPulse.strip("\\r\\n'"))
    
    if timeBetweenPulse == 0:
        rpm = 0
    else:
        frequency = 1000 / timeBetweenPulse
        rpm = frequency * 60 / 360
    print("RPM: " + str(rpm))
    values = {"shiftVal": shiftVal, "rpm": rpm}
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

server = WebsocketServer(13254, host='127.0.0.1')
server.set_fn_new_client(new_client)
server.set_fn_message_received(new_msg)
server.run_forever()