import smbus
import sys
channel = 1
bus = smbus.SMBus(channel)
address = int(sys.argv[1])
dataToSend = sys.argv[2]



def writeNumber(value):
	bus.write_byte(address, value)
	return -1

writeNumber(int(dataToSend))
exit(0)
