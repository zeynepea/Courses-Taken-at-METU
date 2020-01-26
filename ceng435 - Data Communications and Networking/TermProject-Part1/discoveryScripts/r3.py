from socket import *
from threading import Thread
import time # for link cost 

#r3 is server for s, r2 and d

#r3 ips
r3Table = [0]*3
r3Table[0] = '10.10.3.1' #s
r3Table[1] = '10.10.6.1' #r2
r3Table[2] = '10.10.7.1' #d

#client ports
portTable = [0]*3
portTable[0] = 25814 #s port
portTable[1] = 25812 #r2 port
portTable[2] = 25810 #d port

port = 25813 #r3 port

linkCosts = [0]*3 # to store link costs


## send messages to s, r2 and d
def r3Server(i):
        j=0
        while True:

                ## count for 1000 messages
                if(j==1000):
                        break

                ## socket for clients
                clientSocket = socket(AF_INET, SOCK_DGRAM)
                # take time to calculate link cost
                startTime = time.time()
                clientSocket.sendto("msg",(r3Table[i], port))

                # wait fot feedback
                feedback, serverAddress = clientSocket.recvfrom(2048)

                ## take end time to calculate link cost
                endTime = time.time()
                clientSocket.close()
                linkCosts[i] += endTime-startTime
                j = j+1


## create link_cost file
def totalR3():
        ## taking average is dividing the result by 1000 and making the result msec is multiplying it with 1000
        ## this process will give the same result we find before hand so didn't divide and multiply with 1000
        f= open("link_cost.txt","w+")
        hosts = ["r3 -s","r3 -r2", "r3-dest"]
        k=0
        for k in range(3):
                lis = str(linkCosts[k]/1000)
                st = "Link Cost to " + hosts[k] + " = " + lis+" \n"
                f.write(st)
        f.close()
        print "total r3"
        print linkCosts[0]/1000
        print linkCosts[1]/1000
        print linkCosts[2]/1000

## create thread for every client
## for s
t1 = Thread(target=r3Server, args=([0]))
t1.start()
## for r2
t2 = Thread(target=r3Server, args=([1]))
t2.start()
## for d
t3 = Thread(target=r3Server, args=([2]))
t3.start()
t1.join()
t2.join()
t3.join()
totalR3()


