ARMGNU = aarch64-linux-gnu
INC_DIR = include
SRC_DIR = src
BUILD_DIR = build

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:.c=.o)
CFLAGS = -I$(INC_DIR) -Wall -ffreestanding -nostdinc -nostdlib -nostartfiles -fno-stack-protector -g

.PHONY: all clean kernel8.img

all: clean kernel8.img

start.o: $(SRC_DIR)/start.S
	$(ARMGNU)-gcc $(CFLAGS) -c $(SRC_DIR)/start.S -o $(SRC_DIR)/start.o

%.o: %.c
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

kernel8.img: start.o $(OBJS)
	$(ARMGNU)-ld -nostdlib -nostartfiles $(SRC_DIR)/start.o $(OBJS) -T $(SRC_DIR)/linker.ld -o $(BUILD_DIR)/kernel8.elf
	$(ARMGNU)-objcopy -O binary $(BUILD_DIR)/kernel8.elf $(BUILD_DIR)/kernel8.img

clean:
	rm $(BUILD_DIR)/kernel8.elf $(BUILD_DIR)/kernel8.img $(SRC_DIR)/*.o >/dev/null 2>/dev/null || true

run:
	qemu-system-aarch64 -M raspi3b -kernel $(BUILD_DIR)/kernel8.img -display none -serial null -serial stdio

debug: all
	qemu-system-aarch64 -M raspi3b -kernel $(BUILD_DIR)/kernel8.img -display none -serial null -serial stdio -s -S

