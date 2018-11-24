import socket
import sys
import time

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM);

srv_addr = ('localhost', 5000)
print >> sys.stderr, 'starting server on %s, on port %s' % srv_addr;
sock.bind(srv_addr);

sock.listen(1);
print 'now listening'
while True:
	print >> sys.stderr, 'waiting for a connection'
	connection, client_address = sock.accept()
	
	try:
		print >> sys.stderr, 'Connection from', client_address
		
		while True:
			connection.sendall('TESTALLPS');
			time.sleep(100);
	finally:
		connection.close()
		print 'connection terminated'
