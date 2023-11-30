ARMGNU = aarch64-linux-gnu
INCDIR = include
SRCDIR = src
BUILD_DIR = build

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(SRCS:.c=.o)
CFLAGS = -I$(INCDIR) -Wall -ffreestanding -nostdinc -nostdlib -nostartfiles -fno-stack-protector -g

.PHONY: all clean kernel8.img

all: clean kernel8.img

start.o: $(SRCDIR)/start.S
	$(ARMGNU)-gcc $(CFLAGS) -c $(SRCDIR)/start.S -o $(SRCDIR)/start.o

%.o: %.c
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

kernel8.img: start.o $(OBJS)
	$(ARMGNU)-ld -nostdlib -nostartfiles $(SRCDIR)/start.o $(OBJS) -T $(SRCDIR)/linker.ld -o $(BUILD_DIR)/kernel8.elf
	$(ARMGNU)-objcopy -O binary $(BUILD_DIR)/kernel8.elf $(BUILD_DIR)/kernel8.img

clean:
	rm $(BUILD_DIR)/kernel8.elf $(BUILD_DIR)/kernel8.img $(SRCDIR)/*.o >/dev/null 2>/dev/null || true

run:
	qemu-system-aarch64 -M raspi3b -kernel $(BUILD_DIR)/kernel8.img -display none -serial null -serial stdio

debug: all
	qemu-system-aarch64 -M raspi3b -kernel $(BUILD_DIR)/kernel8.img -display none -serial null -serial stdio -s -S

