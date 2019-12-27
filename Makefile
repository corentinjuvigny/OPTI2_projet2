SYSTEM     = x86-64_linux
LIBFORMAT  = static_pic
CPLEXDIR   = /home/kodra/Documents/RO/OPT2/cplex
CPLEXLIBDIR  = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CPLEXFLAGS = -lcplex -lm -pthread
CFLAGS= -Wall -Wextra -g#-O3
CC=gcc
CCPP=g++
OBJECTSLIB=  graph.o roy_marshall.o tools.o list.o fifo.o
CPPOBJECTS= maincpp.o minimal_cut.o
OBJECTS= main.o $(OBJECTSLIB) #liste de tous les .o a creer a partir des .c

all: b.out

a.out : $(OBJECTS)
	$(CC) -I$(CPLEXDIR)/include/ilcplex -L$(CPLEXLIBDIR) $(OBJECTS) $(CPLEXFLAGS)

b.out : librairies $(CPPOBJECTS)
	$(CCPP) $(CPLEXFLAGS) -I$(CPLEXDIR)/include/ilcplex -L$(CPLEXLIBDIR) $(CPPOBJECTS) $(OBJECTSLIB) -o $@

maincpp.o : main.cpp
	$(CCPP) $(CFLAGS) -I$(CPLEXDIR)/include/ilcplex -L$(CPLEXLIBDIR) -c $< -o $@

minimal_cut.o : minimal_cut.cpp
	$(CCPP) $(CFLAGS) -I$(CPLEXDIR)/include/ilcplex -L$(CPLEXLIBDIR) -c $< -o $@

librairies : $(OBJECTSLIB)

.c.o :
	$(CC) $(CFLAGS) -I$(CPLEXDIR)/include/ilcplex -L$(CPLEXLIBDIR) -c $< -o $@

clean :
	rm *.o
