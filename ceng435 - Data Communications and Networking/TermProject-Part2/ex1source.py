from socket import*
from threading import Thread
from time import*

windowSize = 100
windowLocation = 0

sPort = 26414


sTable = [0]*3
sTable[0] = '10.10.1.1' #link between s-r1, to s ip, and r1 port
sTable[1] = '10.10.2.2' #link between s-r2, to s ip, and r2 port
sTable[2] = '10.10.3.1' #link between s-r3, to s ip, and r3 port

xTable = [0]*3
xTable[0] = '10.10.1.2' #link between s-r1, to r1 ip
xTable[1] = '10.10.2.1' #link between s-r2, to r2 ip
xTable[2] = '10.10.3.2' #link between s-r3, to r3 ip

ports = [0]*3
ports[0] = 26411 #r1 port
ports[1] = 26412 #r2 port
ports[2] = 26413 #r3 port

pos = 0
f = open('input.txt', 'r')
inp = f.read()

datas = [0]*6250  #data in a packet 800 bytes thus message divide to 6250 fragments
packetSize = 800
acks = [0]*6250
startTime = time()


## divide file into packets

for i in range(0, 6250):
	datas[i] = inp[i*800:(i+1)*800]

def checksum(data):
	result = 0
	for i in data:
		result += ord(i)
	return ~result  #return the complement

def timer(i):
	global pos
	global windowLocation
	while True:
		sleep(1.5)
		pos = windowLocation
		if( sum(acks) == 6250):
			break
		

#generic sender func
def sender(i):
	global windowLocation
	global pos
	j=0
	while True:
		if( sum(acks) == 6250):
			break
		clientSocket = socket(AF_INET, SOCK_DGRAM)
		##send packets
		if(pos<6250 and (pos-windowLocation<windowSize) and (acks[pos]==0)):
			msg = str(pos) + str('*-*') + str(checksum(datas[pos])) + str('*-*') + str(datas[pos]) 
			clientSocket.sendto(msg,(xTable[i], sPort))
		if(pos-windowLocation<windowSize):
			pos = pos+1
		clientSocket.close()
		

def receiver(i):
	global windowLocation
	global startTime
	server = socket(AF_INET, SOCK_DGRAM) 
	server.bind((sTable[i], ports[i]))  #link between s-r1, to s ip, and r1 port
	j=0
	while True:
		feedback, serverAddress = server.recvfrom(1000)
		if(feedback==""):
			continue
		j = j+1
		firstNack = acks.index(0)
		packNo = int(feedback)
		acks[packNo] = 1
		print(sum(acks))
		if( sum(acks) == 6250 ):
			server.close()
			endTime = time()
			print(endTime - startTime)
			break
		nextNack = acks.index(0)
		if(packNo == firstNack):
			windowLocation = nextNack




senderR3 = Thread(target=sender, args=([2]))
receiverR3 = Thread(target=receiver, args=([2]))
timerR3 = Thread(target=timer, args=([2]))


senderR3.start()
receiverR3.start()
timerR3.start()

senderR3.join()
receiverR3.join()
timerR3.join()


