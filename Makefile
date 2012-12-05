CC=msp430-gcc
CFLAGS=-Os -Wall -g -mmcu=msp430x2012
OPT=2

NAME=lights

OBJS=main.o

all: $(OBJS)
	$(CC) $(CFLAGS) -O$(OPT) -o $(NAME).elf $(OBJS)
	msp430-size $(NAME).elf

%.o: %.c
	$(CC) $(CFLAGS) -O$(OPT) -c $<

.PHONY : clean
clean:
	rm -fr $(NAME).elf $(OBJS)

.PHONY : prog
prog: all
	echo "prog $(NAME).elf" | mspdebug rf2500
