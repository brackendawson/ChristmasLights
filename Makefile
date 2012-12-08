CC=msp430-gcc
CFLAGS=-O2 -Wall -g -mmcu=msp430g2231

NAME=lights

OBJS=main.o
INCLUDES=colours.h

all: $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME).elf $(OBJS)
	msp430-size $(NAME).elf

%.o: %.c colours.h
	$(CC) $(CFLAGS) -c $<

.PHONY : clean
clean:
	rm -fr $(NAME).elf $(OBJS)

.PHONY : prog
prog: all
	echo "prog $(NAME).elf" | mspdebug rf2500
