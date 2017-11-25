CC = i686-elf-gcc
AS = nasm
LD = i686-elf-ld
AR = i686-elf-ar

CFLAGS = -ffreestanding -Wall -Wextra -Werror -c
ASFLAGS =
LDFLAGS = -nostdlib

.PHONY: all run clean
.EXPORT_ALL_VARIABLES:

MODULES = \
program

all: os.iso

libk.a:
	$(MAKE) -C libc

kernel.elf: libk.a
	$(MAKE) -C kernel

$(MODULES):
	$(MAKE) -C userspace
	
os.iso: kernel.elf $(MODULES)
	cp kernel/bin/kernel.elf iso/boot/kernel.elf
	$(foreach module,$(MODULES),cp userspace/bin/$(module) iso/modules/)
	genisoimage -R                              \
				-b boot/grub/stage2_eltorito    \
				-no-emul-boot                   \
				-boot-load-size 4               \
				-A os                           \
				-input-charset utf8             \
				-quiet                          \
				-boot-info-table                \
				-o os.iso                       \
				iso

run: os.iso
	bochs -f bochsrc.txt -rc bochsdb.txt -q

clean:
	rm -f os.iso
	$(MAKE) -C libc clean
	$(MAKE) -C kernel clean
	$(MAKE) -C userspace clean
