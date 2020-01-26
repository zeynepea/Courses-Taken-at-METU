from socket import *
from threading import Thread
import time # rtt

#r2 is a client for r1, r3 and server for s,d



ipTable = [0]*4
ipTable[0] = '10.10.2.2' #s ip
ipTable[1] = '10.10.8.1' #r1 ip
ipTable[2] = '10.10.6.2' #r3 ip
ipTable[3] = '10.10.5.2' #to d

#r2 ips to r2
r2Table = [0]*4
r2Table[0] = '10.10.2.2' #s
r2Table[1] = '10.10.8.2' #r1
r2Table[2] = '10.10.6.1' #r3
r2Table[3] = '10.10.5.2' #d

portTable = [0]*4
portTable[0] = 25814 #s port
portTable[1] = 25811#r1 port
portTable[2] = 25813 # r3 port
portTable[3] = 25810# d port


port = 25812 #r2 port

linkCosts = [0]*4

## receive message from r1 and r3
def r2Client(i):
        ##receive from 
        server = socket(AF_INET, SOCK_DGRAM)
        server.bind((r2Table[i],portTable[i]))


        j=0
        while True:
                ## count for 1000 messages
                if(j==1000):
                        break
                ## take message from servers r1 and r3
                msg, addr = server.recvfrom(1000)

                ## send the received time of the message as feedback to the servers
                feedback = str(time.time())
                server.sendto(feedback.encode(), addr)
                
                #print linkCosts[1]
                j = j+1

        server.close()

## send message to s and d
def r2Server(i):


        j=0
        while True:

        		## count for 1000 messages
                if(j==1000):
                        break

                ## socket for clients
                clientSocket = socket(AF_INET, SOCK_DGRAM)
                ## take start time
                startTime = time.time()
                clientSocket.sendto("kappa",(r2Table[i], port))
                ## take end time to calculate rtt

                feedback, serverAddress = clientSocket.recvfrom(2048)
                endTime = time.time()
                clientSocket.close()
                linkCosts[i] += endTime-startTime
                j=j+1

## create link_cost file
def totalR2():
        ## taking average is dividing the result by 1000 and making the result msec is multiplying it with 1000
        ## this process will give the same result we find before hand so didn't divide and multiply with 1000
        f= open("link_cost.txt","w+")
        hosts = ["r2 -s","","","r2-d"]
        k = 0
        for k in range(4):
                lis = str(linkCosts[k])
                st = "Link Cost to " + hosts[k] + " = " + lis+" \n"
                f.write(st)
        f.close()
        print "total r2"
        print linkCosts[0]
        print linkCosts[3]

## threads for every client and server

## for r1
t1 = Thread(target=r2Client, args=([1]))
t1.start()

## for r3
t2 = Thread(target=r2Client, args=([2]))
t2.start()

## for s
t3 = Thread(target=r2Server, args=([0]))
t3.start()

## for d
t4 = Thread(target=r2Server, args=([3]))
t4.start()
t1.join()
t2.join()
t3.join()
t4.join()
totalR2()


