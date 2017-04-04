CC := gcc
SRCDIR := src
SRCEXT := c
TARGET := bin/myShell
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
CFLAGS := -g -Wall -lreadline

$build:
	$(CC) -o $(TARGET) $(SOURCES) $(CFLAGS)

all:
	$(CC) -o $(TARGET) $(SOURCES) $(CFLAGS)
	./$(TARGET)

run:
	./$(TARGET)

clean:
	@echo "Cleaning..."
	$(RM) $(TARGET)

.PHONY: clean