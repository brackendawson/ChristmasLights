CC=msp430-gcc
MCU=msp430g2452
CFLAGS=-Os -Wall -g -mmcu=$(MCU) -std=c99 -Wl,-Map=lights.map

NAME=lights

OBJS=main.o

.PHONY: all
all: $(NAME).elf

$(NAME).elf: $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME).elf $(OBJS)
	msp430-size $(NAME).elf

%.o: %.c *.h
	$(CC) $(CFLAGS) -c $<

.PHONY : clean
clean:
	rm -fr $(NAME).elf $(OBJS) lights.map

.PHONY : prog
prog: all
	echo "prog $(NAME).elf" | mspdebug rf2500
