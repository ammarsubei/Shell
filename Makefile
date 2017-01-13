CC = gcc
CFLAGS = -g -Wall
TARGET = myShell

all: $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c
clean:
	$(RM) $(TARGET) 