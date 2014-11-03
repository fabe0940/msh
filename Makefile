CC = gcc
CFLAGS = $(WARNINGS) $(DEBUG) $(DEFINE)
DEBUG = -g
#DEFINE = -DPIPING
DEFINE = -DNOPIPING
WARNINGS = -ansi -pedantic -Wall -Wextra -D__USE_FIXED_PROTOTYPES__ --std=gnu89
OBJ = clean.o err.o eval.o exec.o hist.o init.o list.o main.o makearg.o read.o update.o
#LIBS = -lncurses
APPLICATION_NAME = msh

.PHONY : all rebuild clean

all : $(APPLICATION_NAME)

rebuild :
	make clean
	make all

$(APPLICATION_NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(APPLICATION_NAME) $(LIBS)

clean.o : clean.c clean.h cmd.h env.h hist.h status.h
	$(CC) $(CFLAGS) -c clean.c $(LIBS)

err.o : err.h
	$(CC) $(CFLAGS) -c err.c $(LIBS)

eval.o : eval.c eval.h status.h
	$(CC) $(CFLAGS) -c eval.c $(LIBS)

exec.o : exec.c cmd.h err.h exec.h makearg.h status.h
	$(CC) $(CFLAGS) -c exec.c $(LIBS)

hist.o : hist.c err.h hist.h
	$(CC) $(CFLAGS) -c hist.c $(LIBS)

init.o : init.c err.h hist.h init.h status.h
	$(CC) $(CFLAGS) -c init.c $(LIBS)

list.o : list.c err.h list.h
	$(CC) $(CFLAGS) -c list.c $(LIBS)

main.o : main.c clean.h cmd.h init.h read.h status.h update.h
	$(CC) $(CFLAGS) -c main.c $(LIBS)

makearg.o : makearg.c makearg.h
	$(CC) $(CFLAGS) -c makearg.c $(LIBS)

read.o : read.c err.h hist.h list.h read.h status.h
	$(CC) $(CFLAGS) -c read.c $(LIBS)

update.o : update.c err.h hist.h status.h update.h
	$(CC) $(CFLAGS) -c update.c $(LIBS)

clean :
	rm -f $(APPLICATION_NAME) $(OBJ)
