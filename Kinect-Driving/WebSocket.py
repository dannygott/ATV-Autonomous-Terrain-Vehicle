import logging
import json
import time
import serial
from websocket_server import WebsocketServer

ser = serial.Serial(
    port='/dev/ttyACM0',
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
    ser.write(bytes(str(int(messageObj['throttle']*180)), 'utf-8'))
    print(ser.readline())
    


server = WebsocketServer(13254, host='127.0.0.1')
server.set_fn_new_client(new_client)
server.set_fn_message_received(new_msg)
server.run_forever()