CFLAGS := $(CFLAGS) -I include -I "../libc/include"
ASFLAGS := $(ASFLAGS) -f elf
LDFLAGS := $(LDFLAGS) -T link.ld

SRCDIR := src
OBJDIR := obj
BINDIR := bin

OBJECTS = \
crti.o \
crtbegin.o \
loader.o \
kmain.o \
mem/gdt.o \
mem/gdt_asm.o \
mem/idt.o \
mem/idt_asm.o \
debug/logging.o \
fb/fb.o \
fb/fb_scroll.o \
fb/fb_printf.o \
fb/fb_vprintf.o \
fb/fb_putchar.o \
fb/fb_puts.o \
serial/serial.o \
interrupts/pic_remap.o \
interrupts/pic_sendEoi.o \
interrupts/pic_irrIsr.o \
interrupts/pic_isRealIrq.o \
interrupts/irq_mask.o \
interrupts/isr.o \
libk.a \
crtend.o \
crtn.o
OBJECTS := $(OBJECTS:%.o=$(OBJDIR)/%.o)

BINARIES = \
kernel.elf
BINARIES := $(BINARIES:%=$(BINDIR)/%)

.PHONY: all clean

all: $(BINARIES)

$(BINDIR)/kernel.elf: $(OBJECTS) libk.a
	$(if ifndef $(@D), mkdir -p $(@D))
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

$(OBJDIR)/crtbegin.o $(OBJDIR)/crtend.o:
	OBJ=`$(CC) $(CFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(if ifndef $(@D), mkdir -p $(@D))
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.s
	$(if ifndef $(@D), mkdir -p $(@D))
	$(AS) $(ASFLAGS) $< -o $@

libk.a: ../libc/libk.a
	cp ../libc/libk.a libk.a

clean:
	rm -rf $(OBJDIR) $(BINDIR)
