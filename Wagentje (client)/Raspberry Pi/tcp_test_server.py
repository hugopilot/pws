#!/usr/bin/env python

from socket import *

HOST = '10.0.0.117'
PORT = 3001
BUFSIZ = 1024
ADDR = (HOST, PORT)

tcpSerSock = socket(AF_INET, SOCK_STREAM)
tcpSerSock.bind(ADDR)
tcpSerSock.listen(5)

while 1:
   print ('waiting for connection...')
   tcpClisock, addr = tcpSerSock.accept()
   print ('...connected from: ', addr)
   
   while 1:  
      data = tcpCliSock.recv(BUFSIZ)
      if not data: break
      tcpCliSock.send('hello', data)

   tcpCliSock.close()
tcpSerSock.close() 