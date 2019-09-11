import time
import serial

# configure the serial connections (the parameters differs on the device you are connecting to)
ser = serial.Serial(
    port='/dev/ttyACM0',
    baudrate=2000000,
    parity=serial.PARITY_ODD,
    stopbits=serial.STOPBITS_TWO,
    bytesize=serial.SEVENBITS
)

ser.isOpen()

print('Enter your commands below.\r\nInsert "exit" to leave the application.')


while 1 :
    # get keyboard input
    inputA = input(">> ")
        # Python 3 users
        # input = input(">> ")
    if inputA == 'exit':
        ser.close()
        exit()
    else:
        print(bytes(inputA, 'utf-8'))
        # send the character to the device
        # (note that I happend a \r\n carriage return and line feed to the characters - this is requested by my device)
        ser.write(bytes(inputA + "!", 'utf-8'))
