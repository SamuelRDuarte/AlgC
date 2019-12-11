# makefile para o TDA Fraction Simplificado
CC = gcc
CFLAGS = -Wall

OBJM = fraction.o
HEADERS = fraction.h

OBJECTS = $(OBJM) testfraction.o
EXES = tf

all:	$(EXES)

tf:	testfraction.o $(OBJM)
	$(CC) $(CFLAGS) testfraction.o $(OBJM) -o tf

$(OBJM):	$(HEADERS)

clean:
	rm -f $(OBJECTS) *~

cleanall:	clean
	rm -f $(EXES)
