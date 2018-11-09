#
CXX=gcc
RM= \rm -rf
GZIP=gzip
TAR=tar cvf

INCS= -I. 
#INCS= -I. -I/usr/include # to use HPUX with gcc
OPTFLAGS=  -O4
OBJECTS= errexit.o
OBJECTS1= connectTCP.o connectsock.o 
OBJECTS2= passivesock.o passiveTCP.o
OBJECTS3= blockchain.o

DISTRIB = blockchainMain.c blockchain.c connectTCP.c connectsock.c errexit.c

CFLAGS = $(INCS) $(OPTFLAGS)

.SUFFIXES: .c 

all: blockchainMain

.c.o:
	$(CXX) $(CFLAGS) -c $<

blockchainMain: blockchainMain.o $(OBJECTS2) $(OBJECTS1) $(OBJECTS) $(OBJECTS3)
	$(CXX) $(CFLAGS) blockchainMain.o $(OBJECTS2) $(OBJECTS1) $(OBJECTS) $(OBJECTS3) \
	-o blockchainMain

clean:
	rm -f *.o core *~ blockchainMain blockchainMaind
