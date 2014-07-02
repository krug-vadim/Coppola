PROGRAM      = Coppola

CC           = /usr/share/mspgcc-ti/bin/msp430-elf-gcc

CPU          = msp430
DEVICE       = msp430g2553

DEVICE_FLAGS = -mcpu=$(CPU) -mmcu=$(DEVICE)

GIT_VERSION := $(shell git rev-parse HEAD)
BUILD_DATE  := $(shell date --iso=seconds)

BUILD_FLAGS := -DBUILD_DATETIME=\"$(BUILD_DATE)\" -DGITSHA=\"$(GIT_VERSION)\"

CFLAGS       =  $(DEVICE_FLAGS) -Os -Wall -pedantic -Werror $(BUILD_FLAGS)
INCLUDE_PATH = /usr/share/mspgcc-ti/msp430-elf/include
LINKER_PATH  = /usr/share/mspgcc-ti/msp430-elf/include
LFLAGS      = $(DEVICE_FLAGS) -I$(INCLUDE_PATH) -L$(LINKER_PATH)

SOURCES     := src/main.c \
               src/dumb.c \
               src/calibration.c \
               src/protocol.c \
               src/crc.c \
               src/uart.c \
               src/queue.c \
               src/washer.c \
               src/washer_hw.c \
               src/washer_commands.c

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
