from socket import*
from threading import Thread
from time import*


#to d ips
dTable = [0]*3
dTable[0] = '10.10.4.2' #r1
dTable[1] = '10.10.5.2' #r2
dTable[2] = '10.10.7.1' #r3


dT = [0]*3
dT[0] = '10.10.4.1' #r1
dT[1] = '10.10.5.1' #r2
dT[2] = '10.10.7.2' #r3

portTable = [0]*3
portTable[0] = 26411 #r1 port
portTable[1] = 26412 #r2 port
portTable[2] = 26413 #r3 port

acks = [0]*6250
inp = [0]*6250
fileFlag =0

def checksum(data):
	result = 0
	for i in data:
		result += ord(i)
	return ~result  #return the complement

def dest(i):
        global fileFlag

        ## create socket between servers, bind them to s
        server = socket(AF_INET, SOCK_DGRAM)
        server.bind((dTable[i],portTable[i])) #to r3 s port------ to d rportts
        j = 0
        
        while True:
                if(sum(acks) == 6250):
                	print("finish")
                        if(fileFlag==0):
                                with open('output.txt', 'w') as f:
                                for da in inp:
                                        f.write("%s" % da)
                                fileFlag = fileFlag + 1
                
                ## receive message from servers
                msg, addr = server.recvfrom(1000)
                index = msg.find('*-*')
                index2 = msg.rfind('*-*')
                packNo = msg[:index]
                mess = msg[index2+3:]


                if(int(msg[(index+3):index2]) == checksum(msg[index2+3:])):
	                feedback = packNo
                        acks[int(packNo)]=1
                        inp[int(packNo)]=mess
	                server.sendto(feedback.encode(), (dT[i], 26410))
	                j = j+1
                else:
                	server.sendto("".encode(), (dT[i], 26410))

        server.close()

t3 = Thread(target=dest, args=([2]))

t3.start()

t3.join()
