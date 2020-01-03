SYSTEM     = x86-64_linux
LIBFORMAT  = static_pic
CPLEXDIR   = /home/kodra/Documents/RO/OPT2/cplex
CPLEXLIBDIR  = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CPLEXFLAGS = -lcplex -ldl -lm -pthread
CFLAGS= -Wall -Wextra -O3
CC=gcc
CCPP=g++
OBJECTSLIB=  graph.o roy_marshall.o tools.o list.o fifo.o pl.o main.o
CPPOBJECTS= minimal_cut.o
OBJECTS= main.o $(OBJECTSLIB) #liste de tous les .o a creer a partir des .c

all: b.out

a.out : $(OBJECTS)
	$(CC) -I$(CPLEXDIR)/include -L$(CPLEXLIBDIR) $(OBJECTS) $(CPLEXFLAGS)

b.out : librairies $(CPPOBJECTS)
	$(CCPP) -I$(CPLEXDIR)/include -L$(CPLEXLIBDIR) $(CPPOBJECTS) $(OBJECTSLIB) -o $@ $(CPLEXFLAGS)
maincpp.o : main.cpp
	$(CCPP) $(CFLAGS) -I$(CPLEXDIR)/include -L$(CPLEXLIBDIR) -c $< -o $@

minimal_cut.o : minimal_cut.cpp
	$(CCPP) $(CFLAGS) -I$(CPLEXDIR)/include -L$(CPLEXLIBDIR) -c $< -o $@

librairies : $(OBJECTSLIB)

.c.o :
	$(CC) $(CFLAGS) -I$(CPLEXDIR)/include -L$(CPLEXLIBDIR) -c $< -o $@

clean :
	rm *.o
