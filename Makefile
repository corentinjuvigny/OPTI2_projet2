SYSTEM     = x86-64_linux
LIBFORMAT  = static_pic
CPLEXDIR   = /home/kodra/Documents/RO/OPT2/cplex
CPLEXLIBDIR  = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CPLEXFLAGS = -lcplex -lm -pthread
CFLAGS= -Wall -Wextra -O3
CC=gcc
CCPP=g++
OBJECTSLIB=  graph.o roy_marshall.o tools.o
CPPOBJECTS= maincpp.o
OBJECTS= main.o $(OBJECTSLIB) #liste de tous les .o a creer a partir des .c

all: a.out

a.out : $(OBJECTS)
	$(CC) -I$(CPLEXDIR)/include/ilcplex -L$(CPLEXLIBDIR) $(OBJECTS) $(CPLEXFLAGS)

cpp : librairies $(CPPOBJECTS)
	$(CCPP) $(CPLEXFLAGS) -I$(CPLEXDIR)/include/ilcplex -L$(CPLEXLIBDIR) $(CPPOBJECTS) $(OBJECTSLIB) -o b.out

maincpp.o : main.cpp
	$(CCPP) $(CFLAGS) -I$(CPLEXDIR)/include/ilcplex -L$(CPLEXLIBDIR) -c $< -o $@

librairies : $(OBJECTSLIB)

.c.o :
	$(CC) $(CFLAGS) -I$(CPLEXDIR)/include/ilcplex -L$(CPLEXLIBDIR) -c $< -o $@

clean :
	rm *.o
