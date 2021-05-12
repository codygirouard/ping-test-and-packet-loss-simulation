# Cody Girouard
# client - server-client communication using UDP
# simulate ping test over UDP
#! /usr/bin/env python
import socket, sys, random, select, timeit

serverName = sys.argv[1] #server name from command line
serverPort = int(sys.argv[2]) #port from command line

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) #create socket
sock.setblocking(0) #sets socket to non-blocking

sent, rcvd, avg, max, min = 0, 0, 0, 0, 2 #declare variables

#send 10 pings
for x in range(10):
    message = 'PING ' + str(x)
    #calculate time took to send
    start = timeit.default_timer()
    sock.sendto(message,(serverName, serverPort)) #send server msg
    ready = select.select([sock], [], [], 1) #wait for server to respond for 1 second
    stop = timeit.default_timer()
    #calculate time took to send
    sent += 1;
    if ready[0]: #packet was not dropped
        data = sock.recvfrom(4096) #read what server sent
        print message + ' sent... RTT=' + str(stop-start) + ' ms'
        rcvd += 1;
        avg += stop-start
        if min > stop-start:
            min = stop-start
        if max < stop-start:
            max = stop-start
    else: #packet was dropped
        print message + ' sent... Timed Out'
sock.close()

print('%d pkts xmited, %d pkts rcvd, %.1f%% pkt loss' % (sent, rcvd, float(sent-rcvd)/sent*100))
print('min: %s ms, max: %s ms, avg: %s ms' % (str(min), str(max), str(avg/rcvd)))