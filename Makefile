CC=gcc
CFLAGS=-Wall -Wextra -Wformat-security
DFLAGS=-DMG_ENABLE_SSL -DMG_ENABLE_OPENSSL=1 lib/slre/slre.c lib/libmongoose/mongoose.c lib/sha256/sha256.c lib/BSD/strsec.c lib/frozen/frozen.c 
HARDENING= -fstack-protector-all -pie -fPIE -ftrapv 
DIR=src/
DIR_HEADERS=src/headers/
DIROUT=bin/
UNAME_S := $(shell uname -s)
# todo: hardening that makefile

ifeq ($(UNAME_S),Darwin)
 LDFLAGS=-lpthread -lcrypto -lssl -lm 
else
 LDFLAGS=-Wl,-z,relro,-z,now,-z,noexecstack -z,noexecheap -lpthread -lm -lcrypto -lssl
endif


warrior: $(DIR)main.c 
	@echo "Compile... "
	$(CC) $(CFLAGS) $(DFLAGS) -c $(DIR)*.c  -I$(DIR_HEADERS) -Ilib/
	$(CC) -o $(DIROUT)warrior *.o $(LDFLAGS) $(HARDENING)
	$(shell make clean)
	@echo "  "
	@echo " Next step, execute \"bin/warrior\" to start...  "   
clean:
	rm -f *.o warrior
