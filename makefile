# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -D_GNU_SOURCE -D_XOPEN_SOURCE=600
LDFLAGS = 

# Program name
PROGRAM = stree

# Source files
SOURCES = stree.c
OBJECTS = $(SOURCES:.c=.o)

# Installation directories
PREFIX = /usr/local
BINDIR = $(PREFIX)/bin
MANDIR = $(PREFIX)/share/man/man1

# Default target
all: $(PROGRAM)

# Build the program
$(PROGRAM): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(PROGRAM) $(LDFLAGS)

# Compile source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Install the program
install: $(PROGRAM)
	install -d $(BINDIR)
	install -m 755 $(PROGRAM) $(BINDIR)

# Uninstall the program
uninstall:
	rm -f $(BINDIR)/$(PROGRAM)

# Clean build files
clean:
	rm -f $(OBJECTS) $(PROGRAM)

# Clean everything including the program
distclean: clean
	rm -f $(PROGRAM)

# Show help
help:
	@echo "Available targets:"
	@echo "  all        - Build the program (default)"
	@echo "  install    - Install the program to $(BINDIR)"
	@echo "  uninstall  - Remove the program from $(BINDIR)"
	@echo "  clean      - Remove object files"
	@echo "  distclean  - Remove all build files"
	@echo "  help       - Show this help message"

# Phony targets
.PHONY: all install uninstall clean distclean help





