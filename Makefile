CC=msp430-gcc
CFLAGS=-Os -Wall -g -mmcu=msp430g2231
OPT=s

NAME=lights1

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
