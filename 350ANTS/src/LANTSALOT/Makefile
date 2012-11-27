CC=g++
#CFLAGS=-O3 -funroll-loops -c
CFLAGS=-c
#LDFLAGS=-O2 -lm
LDFLAGS=-lm
SOURCES=Bot.C MyBot.C State.C Ant.C MyGrid.C
OBJECTS=$(addsuffix .o, $(basename ${SOURCES}))
EXECUTABLE=MyBot

#Uncomment the following to enable debugging
CFLAGS+=-g -DDEBUG

all: $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.C.o: *.H
	$(CC) $(CFLAGS) $< -o $@

clean: 
	-rm -f ${EXECUTABLE} ${OBJECTS} *.d
	-rm -f debug.txt

run:
	./play_one_game.sh
.PHONY: all clean

