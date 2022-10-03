# Makefile

TARGET = breaker 
OBJECTS = main.o render.o anims.o sprites.o dust.o frame.o mst.o animspr.o fire.o menu.o preca.o breaker.o font.o monsters.o sfx.o transit.o

CFLAGS = -O3 -Wall -I/usr/include/SDL -L/usr/lib -s -DNDEBUG 
LIBS = -lSDL
CC = gcc

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) 

clean:
	rm $(TARGET) $(OBJECTS)

