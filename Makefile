CC := gcc
SRCDIR := src
SRCEXT := c
TARGET := bin/myShell
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
CFLAGS := -g -Wall -lreadline

$build:
	@echo "$(CC) -o $(TARGET) $(SOURCES) $(CFLAGS)"; $(CC) -o $(TARGET) $(SOURCES) $(CFLAGS)

all:
	$(CC) -o $(TARGET) $(SOURCES) $(CFLAGS)
	./$(TARGET)

run:
	./$(TARGET)

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) $(TARGET)"; $(RM) $(TARGET)

.PHONY: clean