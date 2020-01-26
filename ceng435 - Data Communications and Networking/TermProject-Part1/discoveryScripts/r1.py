from socket import *
from threading import Thread
import time 

#r1 is server for s, r2 and d

#r1 ips( to other)
r1Table = [0]*3
r1Table[0] = '10.10.1.1' #s
r1Table[1] = '10.10.8.2' #r2
r1Table[2] = '10.10.4.2' #d

#client ports
portTable = [0]*3
portTable[0] = 25814 #s port
portTable[1] = 25812 #r2 port
portTable[2] = 25810 #d port

port = 25811 #r1 port

linkCosts = [0]*3


## send messages to s, r2 and d
def r1Server(i):
        j = 0
        while True:
                ## count for 1000 messages
                if(j==1000):
                        break
                mt = "melike"

                ## socket for clients
                clientSocket = socket(AF_INET, SOCK_DGRAM)
                # take time to calculate link cost
                startTime = time.time()
                clientSocket.sendto(mt.encode(),(r1Table[i], port))

                #waiting for feedback
                feedback, serverAddress = clientSocket.recvfrom(2048)

                ## take end time to calculate link cost
                endTime = time.time()
                clientSocket.close()
                linkCosts[i] += endTime-startTime
                j = j+1




## create link_cost file
def totalR1():
        ## taking average is dividing the result by 1000 and making the result msec is multiplying it with 1000
        ## this process will give the same result we find before hand so didn't divide and multiply with 1000
        f= open("link_cost.txt","w+")
        hosts = ["r1-source","r1 -r2", "r1-dest"]
        k=0
        for k in range(3):
                lis = str(linkCosts[k])
                st = "Link Cost to " + hosts[k] + " = " + lis+" \n"
                f.write(st)
        f.close()
        print "total r1"
        print linkCosts[0]
        print linkCosts[1]
        print linkCosts[2]

## create thread for every client
## for s
t1 = Thread(target=r1Server, args=([0]))
t1.start()
## for r2
t2 = Thread(target=r1Server, args=([1]))
t2.start()
## for d
t3 = Thread(target=r1Server, args=([2]))
t3.start()
t1.join()
t2.join()
t3.join()
totalR1()

