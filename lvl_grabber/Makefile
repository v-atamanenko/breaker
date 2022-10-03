# Makefile

TARGET = lvlgrab
OBJECTS = main.o

CFLAGS = -O3 -Wall -I/usr/include/SDL -L/usr/lib -s -DNDEBUG 
LIBS = -lSDL
CC = gcc

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) 

clean:
	rm $(TARGET) $(OBJECTS)

