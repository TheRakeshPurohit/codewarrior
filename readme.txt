                                 
	               _                              _            
	              | |                            (_)           
	  ___ ___   __| | _____      ____ _ _ __ _ __ _  ___  _ __ 
	 / __/ _ \ / _` |/ _ \ \ /\ / / _` | '__| '__| |/ _ \| '__|
	| (_| (_) | (_| |  __/\ V  V / (_| | |  | |  | | (_) | |   
	 \___\___/ \__,_|\___| \_/\_/ \__,_|_|  |_|  |_|\___/|_|   
	=========================================================
		  DATE: Dom Jun 26 03:27:50 BRT 2016


 			How you can run
			-----------------

$ make
$ cd cert; openssl req -x509 -sha256 -nodes -days 365 -newkey rsa:2048 -keyout privateKey.key -out certificate.crt
$ cat certificate.crt privateKey.key > certkey.pem 
$ cd ..
$ /bin/warrior 
Open browser at https://127.0.0.1:1345/index.html  (put IP not "localhost", chrome have bug)

Notes:
* if screen stock you reflesh browser with F5...
* If return error, port already in use... close it with...
$ fuser -k -n tcp 1345
* If you want use at network, Whitelist to access server you can edit at "config/whitelist.conf". 

test of path  https://github.com/joostvanveen/php-security-pitfalls

NOTES:
*if you want change cert "cert/certpem.pem" generate with openssl cartificate and key and concatenate both...
* all html code and web sockets + javascript code you can view at path "web/"
* if you change the default port you need edit port in web/ at web sockets connection.

TODO:
* Add Automatons for each language case
* Add load module with dlopen()
* Add ReDOS validator at regex calls


Author: CoolerVoid
contact:coolerlair@gmail.com

