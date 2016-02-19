SDCC=sdcc
SDLD=sdld
STM8FLASH=~/Programy/stm8flash/stm8flash
OBJECTS=serial.ihx
OBJECT=serial

.PHONY: all clean flash

all: $(OBJECTS)

clean:
	rm -f $(OBJECTS)

flash: $(OBJECT).ihx
	$(STM8FLASH) -c stlink -p stm8s003?3 -s flash -w $(OBJECT).ihx

%.ihx: %.c
	$(SDCC) -lstm8 -mstm8 --std-sdcc99 --out-fmt-ihx $(CFLAGS) $(LDFLAGS) $<
