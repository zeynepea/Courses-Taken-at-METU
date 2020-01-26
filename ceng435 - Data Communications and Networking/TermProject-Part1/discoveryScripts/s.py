from socket import*
from threading import Thread
import time 

## s is client for r1, r2 and r3

sTable = [0]*3
sTable[0] = '10.10.1.1' #to s r1
sTable[1] = '10.10.2.2' #to s r2
sTable[2] = '10.10.3.1' #to s r3

## port of servers
ports = [0]*3
ports[0] = 25811 #r1 port
ports[1] = 25812 #r2 port
ports[2] = 25813 #r3 port
port = 25814 # s port

def totalSource(i):

        ## create socket between servers, bind them to s
        server = socket(AF_INET, SOCK_DGRAM)
        server.bind((sTable[i],ports[i]))

        j=0
        while True:

                ## count for 1000 messages
                if(j==1000):
                        break
                ## receive message from servers
                msg, addr = server.recvfrom(1000)
                print msg
                ## send the received time of the message as feedback to the servers
                feedback = str(time.time())
                server.sendto(feedback.encode(), addr)
                j=j+1

        server.close()

## threads for every server
## for r1
t1 = Thread(target=totalSource, args=([0]))
t1.start()
## for r2
t2 = Thread(target=totalSource, args=([1]))
t2.start()
## for r3
t3 = Thread(target=totalSource, args=([2]))
t3.start()
t1.join()
t2.join()
t3.join()
