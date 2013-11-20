CC=msp430-gcc
CFLAGS=-Os -Wall -g -mmcu=msp430g2452

NAME=lights

OBJS=main.o

all: $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME).elf $(OBJS)
	msp430-size $(NAME).elf

%.o: %.c *.h
	$(CC) $(CFLAGS) -c $<

.PHONY : clean
clean:
	rm -fr $(NAME).elf $(OBJS)

.PHONY : prog
prog: all
	echo "prog $(NAME).elf" | mspdebug rf2500
