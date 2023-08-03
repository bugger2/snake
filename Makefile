CC:=clang
CFLAGS:=-O2 -lraylib -Wall -Wextra -g

OBJDIR:=obj
SRCDIR:=src
INCDIR:=include

SRC=$(wildcard $(SRCDIR)/*.c)
OBJ=$(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

all: snake

snake: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $@

clean:
	rm -rf $(OBJDIR) snake

install: snake
	cp snake /usr/bin

remake: clean
	make
