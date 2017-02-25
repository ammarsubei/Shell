CC := gcc 
SRCDIR := src
BUILDDIR := build
TARGET := bin/myShell
 
SRCEXT := c
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
CFLAGS := -g -Wall -lreadline

$(TARGET): $(SOURCES)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET) $(CFLAGS)"; $(CC) $^ -o $(TARGET) $(CFLAGS)

all: $(OBJECTS)
	$(CC) $^ -o $(TARGET) $(CFLAGS)
	./$(TARGET)

run:
	./$(TARGET)

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

.PHONY: clean