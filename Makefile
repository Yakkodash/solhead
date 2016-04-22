TARGET   = head
CC       = gcc
CFLAGS   = -std=gnu99 -Werror
LINKER   = gcc -o
LFLAGS   = -Wall
SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin
SOURCES:= $(wildcard $(SRCDIR)/*.c)
OBJECTS:= $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
all: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJECTS) | $(BINDIR)
	$(LINKER) $@ $(LFLAGS) $(OBJECTS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir $(OBJDIR)

$(BINDIR):
	mkdir $(BINDIR)

clean:
	rm -f $(OBJECTS)
	rm -f $(BINDIR)/$(TARGET)
