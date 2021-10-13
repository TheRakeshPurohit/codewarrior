# codewarrior
Just another manual code analysis tool and static analysis tool 
![Alt text](https://github.com/CoolerVoid/codewarrior/blob/master/doc/img/img1.png)
Codewarrior runs at HTTPd with TLS, uses the KISS principle(https://en.wikipedia.org/wiki/KISS_principle)

Video demo: https://youtu.be/tZkllJ9mieU

#Directories:
> web/ = local of javascripts and html and css sources

> src/ = C source code, this code talking with web socket 

> eggs/ = external modules to search codes using regex

> conf/whitelist.conf = list of IPs that have  access to the HTTPd server

> bin/ = file to execute...

> doc/ = at construction...

> lib/ = External libraries

> cert/ = loads your certificates for TLS here

You don't need to install, compile and run... runs at *Linux,* *BSD* and *MacOS.*
need to install OpenSSL-dev or OpenSSL-devel package because need OpenSSL lib.
```
$ git clone https://github.com/CoolerVoid/codewarrior/; cd CodeWarrior
$ make
```
For example, if you need to create your cert and load...
```
$ cd cert; openssl req  -nodes -new -x509  -keyout key.pem -out cert.pem
$ cd ..
```
Note: use the name key.pem and cert.pem...because codewarrior use this file names to load resources.

Execute don't do "cd bin; ./warrior", follow example:
```
$ bin/warrior 
```

Open browser at *https://127.0.0.1:1345/index.html*   (dont write localhost use 127.0.0.1)

#Notes:
>* if screen stock, you refresh the browser with F5...
>* If return error, port already in use... close it with this following:
``` 
$ fuser -k -n tcp 1345
```

>* If you want to use the network, Whitelist to access server you can edit at "config/whitelist.conf". 
>* Use chrome browser to run.
>* Use 127.0.0.1 addr don't use localhost name.

#Tested at path:  
*https://github.com/joostvanveen/php-security-pitfalls*


#Tricks:
>* if you want to change cert "cert/certpem.pem" generate with OpenSSL certificate and key and concatenate both...
>* all HTML code and web sockets + javascript code you can view at path "web/".
>* if you change the default port, you need to edit port in web/ at web sockets connection.

#TODO:
>* Add external Automatons for each language case
>* Add load module with dlopen()
>* Add ReDOS validator at regex calls
>* Machine learning, so try use bag of Words with KNN

# Author: CoolerVoid
# Template by Pr0teus https://github.com/Pr0teus
# contact:coolerlair@gmail.com

