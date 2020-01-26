## Zeynep Erdoğan 2171577
## Ayşenur Bülbül 2171403


Steps:

1. First connect to each node via ssh:
	for node d:	$ ssh -i ~/.ssh/id_geni_ssh_rsa e2171403@pc3.instageni.stanford.edu -p 26410
	for node r1: $ ssh -i ~/.ssh/id_geni_ssh_rsa e2171403@pc3.instageni.stanford.edu -p 26411
	for node r2: $ ssh -i ~/.ssh/id_geni_ssh_rsa e2171403@pc3.instageni.stanford.edu -p 26412
	for node r3: $ ssh -i ~/.ssh/id_geni_ssh_rsa e2171403@pc3.instageni.stanford.edu -p 26413
	for node s: $ ssh -i ~/.ssh/id_geni_ssh_rsa e2171403@pc3.instageni.stanford.edu -p 26414

2. Create and copy content of ex1dest.py and ex2dest.py to node d, r1new.py to node r1, r2new.py to noed r2, r3new.py to node r3 and ex1source.py and ex2source.py to node s. Also send input.txt file.

3. Synchronize each node using following commands:
	$ sudo service ntp stop
	$ sudo ntpdate -s ops.instageni.gpolab.bbn.com
	$ sudo service ntp start


4. We configure each link according to their connection eth between them. To find which eth they share we use the following approach. To find the ip address between s and r3 we run this command on r3 (getent ahosts "s" | cut -d " " -f 1 | uniq) and get the ip. Then we run the following command "ip route get $ip" the ip here is what get from the former command. We applied this method on s, d, r1, r2, r3. Then to configure them for experiment we used the following commands: 
For experiment 1:
	at node s:
		sudo tc qdisc add dev eth2 root netem loss 5% delay 3ms
		sudo tc qdisc change dev eth2 root netem loss 15% delay 3ms
		sudo tc qdisc change dev eth2 root netem loss 38% delay 3ms
	at node r3:
		sudo tc qdisc add dev eth2 root netem loss 5% delay 3ms
		sudo tc qdisc add dev eth3 root netem loss 5% delay 3ms
		sudo tc qdisc change dev eth2 root netem loss 15% delay 3ms
		sudo tc qdisc change dev eth3 root netem loss 15% delay 3ms
		sudo tc qdisc change dev eth2 root netem loss 38% delay 3ms
		sudo tc qdisc change dev eth3 root netem loss 38% delay 3ms
	at node d:
		sudo tc qdisc add dev eth3 root netem loss 5% delay 3ms
		sudo tc qdisc change dev eth3 root netem loss 15% delay 3ms
		sudo tc qdisc change dev eth3 root netem loss 38% delay 3ms
 	run the python scripts in the given order after each configuration command: ex1dest.py, r3new.py, ex1source.py. The reasoning behind this is d and r3 waits s serves.

 For experiment 2:
	at node s:
		sudo tc qdisc add dev eth1 root netem loss 5% delay 3ms
		sudo tc qdisc add dev eth3 root netem loss 5% delay 3ms
		sudo tc qdisc change dev eth1 root netem loss 15% delay 3ms
		sudo tc qdisc change dev eth3 root netem loss 15% delay 3ms
		sudo tc qdisc change dev eth1 root netem loss 38% delay 3ms
		sudo tc qdisc change dev eth3 root netem loss 38% delay 3ms
	at node r1:
		sudo tc qdisc add dev eth2 root netem loss 5% delay 3ms
		sudo tc qdisc add dev eth3 root netem loss 5% delay 3ms
		sudo tc qdisc change dev eth2 root netem loss 15% delay 3ms
		sudo tc qdisc change dev eth3 root netem loss 15% delay 3ms
		sudo tc qdisc change dev eth2 root netem loss 38% delay 3ms
		sudo tc qdisc change dev eth3 root netem loss 38% delay 3ms
	at node r2:
		sudo tc qdisc add dev eth1 root netem loss 5% delay 3ms
		sudo tc qdisc add dev eth4 root netem loss 5% delay 3ms
		sudo tc qdisc change dev eth1 root netem loss 15% delay 3ms
		sudo tc qdisc change dev eth4 root netem loss 15% delay 3ms
		sudo tc qdisc change dev eth1 root netem loss 38% delay 3ms
		sudo tc qdisc change dev eth4 root netem loss 38% delay 3ms
	at node d:
		sudo tc qdisc add dev eth1 root netem loss 5% delay 3ms
		sudo tc qdisc add dev eth2 root netem loss 5% delay 3ms
		sudo tc qdisc change dev eth1 root netem loss 15% delay 3ms
		sudo tc qdisc change dev eth2 root netem loss 15% delay 3ms
		sudo tc qdisc change dev eth1 root netem loss 38% delay 3ms
		sudo tc qdisc change dev eth2 root netem loss 38% delay 3ms
 	run the python scripts in the given order after each configuration command: ex2dest.py, r1new.py, r2new.py, ex2source.py. The reasoning behind this is d and r3 waits s serves.


NOTE: To reset configurations between each experiment we used the following command: 
	sudo tc qdisc del dev ethX root netem