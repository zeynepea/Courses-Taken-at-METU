from socket import*
from threading import Thread
import time 

## s is server for r3

# s's ips
sTable = [0]*3
sTable[0] = '10.10.1.1' #to s r1
sTable[1] = '10.10.2.2' #to s r2
sTable[2] = '10.10.3.1' #to s r3

xTable = [0]*3
xTable[0] = '10.10.1.2' #to s r1
xTable[1] = '10.10.2.1' #to s r2
xTable[2] = '10.10.3.2' #to s r3

ports = [0]*3
ports[0] = 25811 #r1 port
ports[1] = 25812 #r2 port
ports[2] = 25813 #r3 port
port = 25814 ## s port

def sServer(i):
        j=0
        while True:

                ## count for 1000 messages
                if(j==1000):
                        break

                clientSocket = socket(AF_INET, SOCK_DGRAM)
                startTime = time.time()
                startTimeString = str(startTime)
                clientSocket.sendto(startTimeString.encode(),(xTable[i], port))
                #print startTimeString

                # wait fot feedback
                feedback, serverAddress = clientSocket.recvfrom(2048)
                clientSocket.close()
                j=j+1


## only thread for r3
t3 = Thread(target=sServer, args=([2]))
t3.start()
t3.join()

