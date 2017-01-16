CCC      = g++
CCFLAGS  = -std=c++14 -pedantic -Wall -Wextra #`pkg-config --cflags --libs sdl2`
LDFLAGS	 = -lSDL2

theChip: main.o chip8.o
	${CCC} ${CCFLAGS} -o theChip main.o chip8.o $(LDFLAGS)

main.o: chip8.h main.c
	${CCC} ${CCFLAGS} -c main.c $(LDFLAGS)

chip8.o: chip8.h chip8.cpp
	${CCC} ${CCFLAGS} -c chip8.cpp

clean :
	rm theChip *.o
