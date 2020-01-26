## Zeynep Erdoğan 2171577
## Ayşenur Bülbül 2171403


Steps:

1. First connect to each node via ssh:
	for node d:	$ ssh -i ~/.ssh/id_geni_ssh_rsa e2171403@pc2.instageni.gpolab.bbn.com -p 25810
	for node r1: $ ssh -i ~/.ssh/id_geni_ssh_rsa e2171403@pc2.instageni.gpolab.bbn.com -p 25811
	for node r2: $ ssh -i ~/.ssh/id_geni_ssh_rsa e2171403@pc2.instageni.gpolab.bbn.com -p 25812
	for node r3: $ ssh -i ~/.ssh/id_geni_ssh_rsa e2171403@pc2.instageni.gpolab.bbn.com -p 25813
	for node s: $ ssh -i ~/.ssh/id_geni_ssh_rsa e2171403@pc2.instageni.gpolab.bbn.com -p 25814

2. Create and copy content of d.py to node d, r1.py to node r1, r2.py to noed r2, r3.py to node r3 and s.py to node s. Run the given configure files(configureR1.sh and configureR2.sh) for r1 and r2.

3. Synchronize each node using following commands:
	$ sudo service ntp stop
	$ sudo ntpdate -s ops.instageni.gpolab.bbn.com
	$ sudo service ntp start

4. For "discovery" part run the python scripts in the given order: s.py, d.py, r2.py, r1.py, r3.py. The reasoning behind this is we applied:
		a) r1 and r3 routers as servers for s, d and r2. 
		b) s and d as clients for r1, r2 and r3.
		c) r2 as server for s and d but client for r1 and r3.
Thus, we run the ones that are clients first. The scripts give a text named "link_cost" that shows each links costs.

5. For experiment part we configure each node according to their connection eth between them. To find which eth they share we use the following approach. To find the ip address between s and r3 we run this command on r3 (getent ahosts "s" | cut -d " " -f 1 | uniq) and get the ip. Then we run the following command "ip route get $ip" the ip here is what get from the former command. We applied this method on s, d and r3. We found that between r3 and destinatin we use eth3, between r3 and s we use eth2. Then to configure them for experiment we used the following commands:
		For experiment 1:
		a) on r3: 	sudo tc qdisc add dev eth2 root netem delay 20ms 5ms distribution normal
					sudo tc qdisc add dev eth3 root netem delay 20ms 5ms distribution normal
		b) on s:	sudo tc qdisc add dev eth2 root netem delay 20ms 5ms distribution normal
		c) on d:	sudo tc qdisc add dev eth3 root netem delay 20ms 5ms distribution normal
		For experiment 2:
		a) on r3: 	sudo tc qdisc add dev eth2 root netem delay 40ms 5ms distribution normal
					sudo tc qdisc add dev eth3 root netem delay 40ms 5ms distribution normal
		b) on s:	sudo tc qdisc add dev eth2 root netem delay 40ms 5ms distribution normal
		c) on d:	sudo tc qdisc add dev eth3 root netem delay 40ms 5ms distribution normal
		For experiment 3:
		a) on r3: 	sudo tc qdisc add dev eth2 root netem delay 50ms 5ms distribution normal
					sudo tc qdisc add dev eth3 root netem delay 50ms 5ms distribution normal
		b) on s:	sudo tc qdisc add dev eth2 root netem delay 50ms 5ms distribution normal
		c) on d:	sudo tc qdisc add dev eth3 root netem delay 50ms 5ms distribution normal
Then run python files in the order: experimentD.py, experimentR3.py, experimentS.py
End-to-end delay is printed to node d's terminal.

NOTE: To reset configurations between each experiment we used the following command: 
	sudo tc qdisc del dev ethX root netem