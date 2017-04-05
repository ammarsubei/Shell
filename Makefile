CC := gcc
SRCDIR := src
SRCEXT := c
TARGET := bin/myShell
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
CFLAGS := -g -Wall -lreadline

$(TARGET):
	$(CC) -o $@ $(SOURCES) $(CFLAGS)

run:
	./$(TARGET)

clean:
	@echo "Cleaning..."
	$(RM) $(TARGET)

.PHONY: clean