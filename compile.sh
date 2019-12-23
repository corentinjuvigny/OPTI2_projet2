#!/bin/sh

SYSTEM=x86-64_linux
LIBFORMAT=static_pic
CPLEXDIR=/home/kodra/Documents/RO/OPT2/cplex #Path to your cplex directory
CPLEXLIBDIR=$CPLEXDIR/lib/$SYSTEM/$LIBFORMAT 
CPLEXINCLUDEDIR=$CPLEXDIR/include/ilcplex

gcc -Wall -Wextra -O3 -lcplex -lm -pthread -I$CPLEXINCLUDEDIR -L$CPLEXLIBDIR main.c graph.c roy_marshall.c tools.c
