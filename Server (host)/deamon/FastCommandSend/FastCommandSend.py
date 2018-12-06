import socket
import sys
import time

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server_address = ('localhost', 8383)

sock.connect(server_address)
print('Connected')
mes = input("Type in command: ")
mess = mes + '\0'
sock.sendall(mess.encode('ascii'))
print(mes)

