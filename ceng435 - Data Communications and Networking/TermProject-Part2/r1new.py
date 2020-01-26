from socket import*
from threading import Thread
from time import*


#to r3 ips
r3Table = [0]*3
r3Table[0] = '10.10.3.2' #s


#r3 ips
r3= [0]*3
r3[0] = '10.10.7.1' #to d

portTable = [0]*3
portTable[0] = 26414 #s port


def r1sender():

        ## create socket between servers, bind them to s
        server = socket(AF_INET, SOCK_DGRAM)
        server.bind(('10.10.1.2',26414))
        
        while True:

                ## receive message from servers
                msg, addr = server.recvfrom(1000)
                #print(msg)
                server.sendto(msg, ('10.10.4.2', 26411)) 


        server.close()

def r1receiver():

        ## create socket between servers, bind them to s
        server = socket(AF_INET, SOCK_DGRAM)
        server.bind(('10.10.4.1',26410))

        
        while True:

                
                ## receive message from servers
                msg, addr = server.recvfrom(1000)
                #print(msg)
                server.sendto(msg, ('10.10.1.1', 26411)) 



t1 = Thread(target=r1sender, args=())
t12 = Thread(target=r1receiver, args=())
t1.daemon=True
t12.daemon=True
t1.start()
t12.start()
t1.join()
t12.join()