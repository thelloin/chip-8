CCC      = g++
CCFLAGS  = -std=c++14 -pedantic -Wall -Wextra

theChip: main.o chip8.o
	${CCC} ${CCFLAGS} -o theChip main.o chip8.o

main.o: chip8.h main.c
	${CCC} ${CCFLAGS} -c main.c

chip8.o: chip8.h chip8.cpp
	${CCC} ${CCFLAGS} -c chip8.cpp

clean :
	rm theChip *.o
