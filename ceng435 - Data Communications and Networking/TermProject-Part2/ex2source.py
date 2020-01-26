from socket import*
from threading import Thread
from time import*



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


########### used for selective repeat pipeline control
pos = 0
windowSize = 100
windowLocation = 0
startTime = time()

j=0

f = open('input.txt', 'r')
inp = f.read()

datas = [0]*6250 #data in a packet 800 bytes thus message divide to 6250 fragments
packetSize = 800 #!!!!!not the total paket size, only the text message part is 800 bytes long
acks = [0]*6250  

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
		sleep(0.75)
		pos = windowLocation #timeout
		if(ones() == 6250):
			print("finish timer",i)
			break
		
def ones():
	total=0
	for i in acks:
		if(i==1):
			total=total+1
	return total

#generic sender func
def sender(i):
	global windowLocation
	global pos
	j=0
	ones2 = 0
	ones3 = 0
	while True:
		ones2 = ones()
		if( ones2==6250 ):
			print("finish sender", i)
			break
		clientSocket = socket(AF_INET, SOCK_DGRAM)
		##send packets
		if(pos<6250 and (pos-windowLocation<windowSize) and (acks[pos]==0)):
			print(pos, i)
			msg = str(pos) + str('*-*') + str(checksum(datas[pos])) + str('*-*') + str(datas[pos]) 
			clientSocket.sendto(msg,(xTable[i], sPort))
		if(pos-windowLocation<windowSize):
			pos = pos+1
		clientSocket.close()
		ones3 = ones()
		if( ones3==6250 ):
			print("finish sender", i)
			break

of = 0

def receiver(i):
	global windowLocation
	global startTime
	server = socket(AF_INET, SOCK_DGRAM) 
	server.bind((sTable[i], ports[i]))  #link between s-r1, to s ip, and r1 port
	global j
	while True:

		feedback, serverAddress = server.recvfrom(1000)
		if(feedback==""):
			continue
		j = j+1
		firstNack = acks.index(0)
		packNo = int(feedback)
		acks[packNo] = 1

		try:
			nextNack = acks.index(0)
		except ValueError:
			print("no where to go")
		#print(ones())

		if(packNo == firstNack):
			windowLocation = nextNack
		if( ones()>=6249 ):
			print("finish rec", i)
			endTime = time()
			print(endTime - startTime)
			server.close()
			break
	

senderR1 = Thread(target=sender, args=([0]))
senderR2 = Thread(target=sender, args=([1]))

receiverR1 = Thread(target=receiver, args=([0]))
receiverR2 = Thread(target=receiver, args=([1]))

timerR1 = Thread(target=timer, args=([0]))

senderR1.daemon=True
senderR2.daemon=True

receiverR1.daemon=True
receiverR2.daemon=True

timerR1.daemon=True

senderR1.start()
senderR2.start()

receiverR1.start()
receiverR2.start()

timerR1.start()


senderR1.join()
senderR2.join()

receiverR1.join()
receiverR2.join()

timerR1.join()


