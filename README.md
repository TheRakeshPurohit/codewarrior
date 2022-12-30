# Codewarrior

## Etimology
Codewarrior is a tool for static code analysis. Codewarrior is a tool to help you in manual code review for different programming languages. You can load external rules for each context of programming language to detect sinks of SQL injection, SSTI, XXE, LFI, XSS and soon. Following another point, yes, you can load rules to extract secrets of files following a recursive approach to extract resources, for example, to show tokens of AWS, GCP, and Azure accounts.
<img align="right" width="240" height="220" src="https://github.com/CoolerVoid/codewarrior/blob/master/doc/img/frogcode.png">
The tool uses Moongose HTTPd resources with TLS, design pattern follows the KISS principle.(https://en.wikipedia.org/wiki/KISS_principle)

## Video demo 
https://www.youtube.com/watch?v=VmTQSGZ1QaU

## Console version
* https://github.com/CoolerVoid/Mosca

## Features
* Resources to load and search custom rules
* Resources to recursive search in files following sink point by regex rule and file extension
* Options to edit rules on the fly
* Options to list files by extension
* [**Rules to extract credentials (Azure, AWS, GCP, twilio and soon**)](https://github.com/CoolerVoid/codewarrior/blob/master/eggs/all_langs.egg)
* HTTPd view with web socket resources and effects with prismjs,fancybox and jquery
* Syntax highlight by programming language
* Resource to control IP address to access codewarrior by allow list.

## Compile and run

You don't need to install it, but it must compile and run. 
So it has full support for Linux and *BSD.

So to run, you need to install OpenSSL-dev, libpcre3-dev or OpenSSL-devel,libpcre3-devel package
```
Install libs
$ sudo apt install libssl-dev libpcre3-dev make gcc
Get content of repository
$ git clone https://github.com/CoolerVoid/codewarrior/; 
Enter and compile the context
$ cd CodeWarrior
$ make clean; make
```
For example, if you need to create your cert and load, remember, following production context, **I don`t recommend self-signed cert.**
```
$ cd cert; openssl req  -nodes -new -x509  -keyout key.pem -out cert.pem
$ cd ..
```
Note: use the name key.pem and cert.pem, because codewarrior use this file names to load resources in directory cert/.
**For secure cert use letsencrypt service https://letsencrypt.org**


Execute don't do "cd bin; ./warrior", follow example:
```
$ bin/warrior 
```

Open browser at *https://127.0.0.1:1345/index.html*   (dont write localhost use 127.0.0.1)

## Notes:
>* if screen stock, you can refresh the browser with F5 and try again.
>* If return error, port already in use... close it with this following:
``` 
$ fuser -k -n tcp 1345
```

* If you want to use the network, allowlist to access server you can edit at "config/allowlist.conf". 
* Use chrome browser to run.
* Use 127.0.0.1 addr don't use localhost name.

## Test in video demo
uses repository php-security-pitfalls to find points of attention:  
*https://github.com/joostvanveen/php-security-pitfalls*


## Tricks:
```
* All HTML code and web sockets + javascript code you can view at path "web/".
* If you change the default port, you need to edit port in web/ at web sockets connection.
```

## Directories overview:
```
> web/ = local of javascripts and html and css sources
> src/ = C source code, this code talking with web socket 
> eggs/ = external modules to search codes using regex
> conf/allowlist.conf = list of IPs that have  access to the HTTPd server
> bin/ = file to execute...
> doc/ = at construction...
> lib/ = External libraries
> cert/ = loads your certificates for TLS here
``` 

## Extra, Two way TLS
So,you can use TWO way TLS in codewarrior, you need create CA file in cert/ca.pem, so codewarrior
detect existence of file and load him to use Two way TLS.



## Future TODO:
* Add external Automatons for each language case
* Add load module with dlopen()
* Add ReDOS validator at regex calls
* Machine learning, so try use bag of Words with KNN

## Contact
* Author: CoolerVoid
* Template by Pr0teus https://github.com/Pr0teus
* contact:coolerlair[at]gmail[dot]com

# Point of attention
The purpose of this tool is to use in pentest, take attention if you have a proper authorization before to use that. I do not have responsibility for your actions. You can use a hammer to construct a house or destroy it, choose the law path, don't be a bad guy, remember.

