from socket import *
from threading import Thread
import time 

#r3 is server for s and d


#r3 ips
r3Table = [0]*3
r3Table[0] = '10.10.3.2' #s
r3Table[1] = '10.10.6.2' #r2
r3Table[2] = '10.10.7.2' #d

#r3 ips
r3= [0]*3
r3[0] = '10.10.3.1' #s
r3[1] = '10.10.6.1' #r2
r3[2] = '10.10.7.1' #d

#client ports
portTable = [0]*3
portTable[0] = 25814 #s port
portTable[1] = 25812 #r2 port
portTable[2] = 25810 #d port
port = 25813 #r3 port

linkCosts = [0]*3


def r3Client(i):

        ## create socket between servers, bind them to s
        server = socket(AF_INET, SOCK_DGRAM)
        server.bind((r3Table[i],portTable[i]))
        
        j=0
        while True:
                 ## count for 1000 messages
                if(j==1000):
                        break
                ## receive message from servers
                msg, addr = server.recvfrom(1000)
                #print msg
                ## send the received time of the message as feedback to the servers
                feedback = "OK"
                server.sendto(feedback.encode(), addr)
                #print linkCosts[1]
                clientSocket = socket(AF_INET, SOCK_DGRAM)
                clientSocket.sendto(msg,(r3[2], port))
                feedback, serverAddress = clientSocket.recvfrom(2048)
                clientSocket.close()
                j=j+1

        server.close()




## create thread for s
## for s
t1 = Thread(target=r3Client, args=([0]))
t1.start()
t1.join()


