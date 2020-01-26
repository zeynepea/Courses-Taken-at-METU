from socket import*
from threading import Thread
import time

## d is client for r1, r2 and r3

## d's ips'
ipTable = [0]*3
ipTable[0] = '10.10.4.2' #for r1
ipTable[1] = '10.10.5.2' #for r2
ipTable[2] = '10.10.7.1' #for r3

## ports of servers
ports = [0]*3
ports[0] = 25811 #r1 port
ports[1] = 25812 #r2 port
ports[2] = 25813 #r3 port
port = 25810

def totalDest(i):

        ## create socket between servers, bind them to s
        server = socket(AF_INET, SOCK_DGRAM)
        server.bind((ipTable[i],ports[i]))

        j=0
        while True:
                ## count for 1000 messages
                if(j==1000):
                        break
                ## receive message from servers
                msg, addr = server.recvfrom(1000)
                ## send the received time of the message as feedback to the servers
                feedback = str(time.time())
                server.sendto(feedback.encode(), addr)
                #print linkCosts[1]
                j=j+1

        server.close()
t1 = Thread(target=totalDest, args=([0]))
t1.start()
t2 = Thread(target=totalDest, args=([1]))
t2.start()
t3 = Thread(target=totalDest, args=([2]))
t3.start()
t1.join()
t2.join()
t3.join()
