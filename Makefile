TOOLPREFIX=riscv-none-elf-

CC = $(TOOLPREFIX)gcc
AS = $(TOOLPREFIX)as
LD = $(TOOLPREFIX)ld

OBJS = \
	entry.o \
	kalloc.o \
	start.o \
	main.o \
	mem.o \
	printf.o \
	strings.o \
	swtch.o \
	trampoline.o \
	uart.o

ASFLAGS = -march=rv64gc -mabi=lp64d

CFLAGS = -std=gnu2x -march=rv64gc -mabi=lp64d
CFLAGS += -Wall -Werror -Og -fno-omit-frame-pointer -ggdb -gdwarf-2
CFLAGS += -MD 
CFLAGS += -mcmodel=medany 
CFLAGS += -ffreestanding -fno-common -nostdlib -mno-relax 
CFLAGS += -I.
CFLAGS += $(shell $(CC) -fno-stack-protector -E -x c /dev/null >/dev/null 2>&1 && echo -fno-stack-protector) 

LDFLAGS = -b elf64-littleriscv -z max-page-size=4096

kernel: kernel.ld $(OBJS)
	$(LD) $(LDFLAGS) -T kernel.ld -o kernel $(OBJS)

QEMUOPTS = -machine virt -bios none -kernel kernel -m 128M -smp 1 -nographic
QEMUOPTS += -global virtio-mmio.force-legacy=false

qemu: kernel
	qemu-system-riscv64 $(QEMUOPTS)

gdb: kernel
	qemu-system-riscv64 $(QEMUOPTS) -S -gdb tcp::26000

clean:
	rm -f *.o *.d
