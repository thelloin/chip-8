CCC      = g++
CCFLAGS  = -std=c++14 -pedantic -Wall -Wextra #`pkg-config --cflags --libs sdl2`
LDFLAGS	 = -lSDL2

theChip: main.o chip8.o IOHandler.o
	${CCC} ${CCFLAGS} -o theChip main.o chip8.o IOHandler.o $(LDFLAGS)

main.o: chip8.h IOHandler.h main.c
	${CCC} ${CCFLAGS} -c main.c $(LDFLAGS)

chip8.o: chip8.h chip8.cpp
	${CCC} ${CCFLAGS} -c chip8.cpp

IOHandler.o: IOHandler.h IOHandler.cpp
	${CCC} ${CCFLAGS} -c IOHandler.cpp

clean :
	rm theChip *.o
