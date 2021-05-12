# Cody Girouard
# server - server-client communication using UDP
# simulate ping test over UDP
#! /usr/bin/env python

import sys, socket, random, datetime
from time import ctime

argv = sys.argv
port = int(argv[1])



random.seed(datetime.datetime.now()) #seed random generator with current time
serverPort = int(sys.argv[1])
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) #set up udp server
sock.bind(('', serverPort)) #bind to localhost with port given from command line
print '[server: ', socket.gethostname() + ']: ready to accept data...'
while 1: #keep accepting messages from clientS
    message, clientAddress = sock.recvfrom(2048)
    if random.randint(1,10) > 3: #70% chance to send packet
        sock.sendto(message, clientAddress)
        print '[' + clientAddress[0] + ']: ' + message + ' rcvd at ' + str(ctime())
    else: #30% chance to drop packet
        print '[server: ', socket.gethostname() + ']: dropped packet'

