CC=gcc
CFLAGS=-std=c11 -Wno-unused-result -Wno-unused-variable -Wextra  -O2 $(shell pkg-config gtk+-3.0 --cflags)

LDFLAGS= $(shell pkg-config gtk+-3.0 --libs)

NAME=gtk-talk
NAME2=menu

SRC=gtk-talk.c board.c datastruct.c lin-fifo.c
SRC2=menu.c
OBJS2=menu.o

OBJS=$($SRC.c=.o) 
all: $(NAME) $(NAME2)
$(NAME):$(OBJS)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME) $(LDFLAGS)
$(NAME2):$(OBJS2)
	$(CC) $(CFLAGS) $(SRC2) -o $(NAME2) $(LDFLAGS)
clean:
	rm -f $(OBJS) $(NAME) $(NAME2) AtoB BtoA

