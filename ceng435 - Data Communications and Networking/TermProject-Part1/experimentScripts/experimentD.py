from socket import*
from threading import Thread
import time

## d is client for r3

## d's ips
ipTable = [0]*3
ipTable[0] = '10.10.4.2' #for r1
ipTable[1] = '10.10.5.2' #for r2
ipTable[2] = '10.10.7.1' #for r3


ports = [0]*3
ports[0] = 25811 #r1 port
ports[1] = 25812 #r2 port
ports[2] = 25813 #r3 port
port = 25810

def totalDest(i):

        ## create socket between servers, bind them to s
        server = socket(AF_INET, SOCK_DGRAM)
        server.bind((ipTable[i],ports[i]))
        e2e=0
        j=0
        while True:
                 ## count for 1000 messages
                if(j==1000):
                        break
                ## receive message from servers
                msg, addr = server.recvfrom(1000)
                #print msg

                feedback = "OK"
                server.sendto(feedback.encode(), addr)
                ## send the received time of the message as feedback to the servers
                ti = msg.decode()
                tim = float(ti)
                endTime = time.time()
                e2e += endTime-tim
                #print linkCosts[1]
                j=j+1

        ## taking average is dividing the result by 1000 and making the result msec is multiplying it with 1000
        ## this process will give the same result we find before hand so didn't divide and multiply with 1000
        print "e2e"
        print e2e
        server.close()

## only thread for r3
t3 = Thread(target=totalDest, args=([2]))
t3.start()
t3.join()

