CC = gcc
CFLAGS = $(WARNINGS) $(DEBUG) $(DEFINE)
DEBUG = -g
#DEFINE = -DVAR=VAL
WARNINGS = -ansi -pedantic -Wall -Wextra -D__USE_FIXED_PROTOTYPES__ --std=c89
OBJ = main.o makearg.o list.o
#LIBS = -lncurses
APPLICATION_NAME = msh

.PHONY : all rebuild clean

all : $(APPLICATION_NAME)

rebuild :
	make clean
	make all

$(APPLICATION_NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(APPLICATION_NAME) $(LIBS)

main.o : main.c list.h
	$(CC) $(CFLAGS) -c main.c $(LIBS)

list.o : list.c list.h
	$(CC) $(CFLAGS) -c list.c $(LIBS)

makearg.o : makearg.c makearg.h
	$(CC) $(CFLAGS) -c makearg.c $(LIBS)

clean :
	rm -f $(APPLICATION_NAME) $(OBJ)
