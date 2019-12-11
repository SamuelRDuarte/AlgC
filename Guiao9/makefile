# makefile para a arvore ABP (Arvore Binaria de Pesquisa)
CC = gcc
CFLAGS = -Wall

OBJM = queue.o stack.o intabp.o
HEADERS = queue.h stack.h intabp.h

OBJECTS = $(OBJM) simabp.o
EXES = sabp

all:	$(EXES)

sabp:	simabp.o $(OBJM)
	$(CC) $(CFLAGS) simabp.o $(OBJM) -o sabp

$(OBJM):	$(HEADERS)

clean:
	rm -f $(OBJECTS) *~

cleanall:	clean
	rm -f $(EXES)
