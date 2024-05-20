OBJDIR:=./obj
SRCDIR:=./src
INCDIR:=./raylib/include

SRC=$(wildcard $(SRCDIR)/*.c)
OBJ=$(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

CC:=gcc
# CFLAGS:=-L./raylib/lib/ -lraylib -Wall -Wextra -g -I$(INCDIR)
CFLAGS:=-lraylib -Wall -Wextra -g -I$(INCDIR)

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
