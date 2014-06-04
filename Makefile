PROGRAM      = Coppola

CC           = /usr/share/mspgcc-ti/bin/msp430-elf-gcc

CPU          = msp430
DEVICE       = msp430g2553

DEVICE_FLAGS = -mcpu=$(CPU) -mmcu=$(DEVICE)

CFLAGS       =  $(DEVICE_FLAGS) -Os -Wall -pedantic -Werror

INCLUDE_PATH = /usr/share/mspgcc-ti/msp430-elf/include
LINKER_PATH  = /usr/share/mspgcc-ti/msp430-elf/include
LFLAGS      = $(DEVICE_FLAGS) -I$(INCLUDE_PATH) -L$(LINKER_PATH)

#SOURCES     := $(wildcard src/*.c)
#SOURCES     := $(wildcard src/*.c)
SOURCES     := src/main.c \
               src/protocol.c \
               src/crc.c \
               src/uart.c \
               src/queue.c
OBJECTS     := $(patsubst %.c, %.o, $(SOURCES))

# top-level rule to create the program.
all: $(PROGRAM)

$(PROGRAM) : $(PROGRAM).elf

%.elf: $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# cleaning everything that can be automatically recreated with "make".
clean:
	rm $(PROG) *.o
