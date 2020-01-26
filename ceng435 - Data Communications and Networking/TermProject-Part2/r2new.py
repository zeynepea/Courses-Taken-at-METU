from socket import*
from threading import Thread
from time import*



def r2sender():

        ## create socket between servers, bind them to s
        server = socket(AF_INET, SOCK_DGRAM)
        server.bind(('10.10.2.1',26414))
        
        
        while True:
                
                
                ## receive message from servers
                msg, addr = server.recvfrom(1000)
                #print(msg)
                server.sendto(msg, ('10.10.5.2', 26412)) 
                
        server.close()

def r2receiver():

        ## create socket between servers, bind them to s
        server = socket(AF_INET, SOCK_DGRAM)
        server.bind(('10.10.5.1',26410))
        
        
        while True:
                
                
                ## receive message from servers
                msg, addr = server.recvfrom(1000)
                #print(msg)
                server.sendto(msg, ('10.10.2.2', 26412)) 
                


t1 = Thread(target=r2sender, args=())
t12 = Thread(target=r2receiver, args=())
t1.daemon=True
t12.daemon=True
t1.start()
t12.start()
t1.join()
t12.join()