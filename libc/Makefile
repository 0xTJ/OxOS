CFLAGS := $(CFLAGS) -I include -I "../kernel/include"
ASFLAGS := $(ASFLAGS) -f elf

SRCDIR = src
OBJDIR = obj

OBJECTS = \
stdio/printf.o \
stdio/vprintf.o \
stdio/putchar.o \
stdio/puts.o \
stdlib/abort.o \
string/strlen.o
OBJECTS := $(OBJECTS:%.o=$(OBJDIR)/%.o)

.PHONY: all clean

all: libk.a

libk.a: $(OBJECTS)
	$(AR) rcs $@ $(OBJECTS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(if ifndef $(@D), mkdir -p $(@D))
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.s
	$(if ifndef $(@D), mkdir -p $(@D))
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf $(OBJDIR) libk.a
