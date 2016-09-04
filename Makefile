CC=gcc
CFLAGS=-Wall -Wextra  
DFLAGS=-DMG_ENABLE_SSL -DMG_DISABLE_PFS lib/slre/slre.c lib/krypton/krypton.c lib/libmongoose/mongoose.c lib/sha256/sha256.c lib/BSD/strsec.c lib/frozen/frozen.c -llib/krypton
DIR=src/
DIROUT=bin/
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	LDFLAGS=-lpthread 
else
	LDFLAGS=-Wl,-z,relro,-z,now -lpthread  
endif

warrior: $(DIR)warrior.c 
	@echo "Compile... "
	$(CC) $(CFLAGS) $(DFLAGS) -c $(DIR)*.c 
	$(CC) -o $(DIROUT)warrior *.o $(LDFLAGS)
	$(shell make clean)
	@echo "  "
	@echo " Next step, execute \"bin/warrior\" to start...  "   
clean:
	rm -f *.o warrior
