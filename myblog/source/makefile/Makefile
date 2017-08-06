CC = gcc
OBJ = main.o mylib.o

CPPFLAGS = -Iinclude

vpath %.h include
vpath %.c source

prog : $(OBJ)
	$(CC) -o $@ $(OBJ)

%.o : %.c
	$(CC) $(CPPFLAGS) -c $<

main.o : mylib.h

.PHONY : clean
clean :
	rm main main.o mylib.o
